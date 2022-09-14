//
// Created by MRV on 9/12/2022.
//

#include "items/SystemItem"
#include <queue>
SystemItem SystemItem::get_by_id(std::string_view id, std::shared_ptr<tao::pq::connection> bd, BDError &bd_error) {
    std::string request_id = "select * FROM files WHERE id = $1";
    try{
        tao::pq::result result_id = bd->execute(request_id, id);
        if (result_id.empty()){
            bd_error = BDError::NOT_FOUND;
            return {};
        }
        bd_error = BDError::OK;
        auto answer = SystemItem(*result_id.begin());
        //очередь для поиска потомков
        std::queue<SystemItem*> queue_find_childrens;
        //если изначальный элемент папка - стартуем
        if (answer.type == SystemItemType::FOLDER)
        {
            queue_find_childrens.emplace(&answer);
            //пока очередь не пуста
            while (!queue_find_childrens.empty()){
                //берём
                auto cur = queue_find_childrens.front();
                queue_find_childrens.pop();
                //собираем запрос для поикска файлов с парентом id равному id элемента
                std::string request_childrens = "select * FROM files WHERE parent_id = $1";
                auto result = bd->execute(request_childrens, cur->id);
                cur->children.reserve(result.size());
                for (const tao::pq::row& row : result){
                    auto& cur_elem = cur->children.emplace_back(row);
                    if (cur_elem.type == SystemItemType::FOLDER) {
                        queue_find_childrens.push(&(cur_elem));
                    }
                }
            }
        }
        return answer;
    }
    //если некорректные данные, то ловим эксепшен
    catch (...){
        bd_error = BDError::BAD_FORMAT;
        return {};
    }
}

SystemItem::SystemItem(const tao::pq::row &result) {
    id = result.get(I_id);
    if (!result.is_null(I_url)) url = result.get(I_url);
    if (!result.is_null(I_parentId)) parentId = result.get(I_parentId);
    date = result.get(I_date);
    size = std::stoull(result.get(I_size));
    type = magic_enum::enum_cast<SystemItemType>(result.get(I_type)).value();
}

bool SystemItem::import(std::shared_ptr<tao::pq::connection> bd, std::string_view cur_date) {
    std::string request = "SELECT import_file($1, $2, $3, $4, $5, $6)";
    try{
        tao::pq::result result = bd->execute(std::move(request),
                                             id,
                                             url,
                                             parentId,
                                             cur_date,
                                             std::to_string(size),
                                             magic_enum::enum_name(type));
        std::string_view sv(result.begin().operator*().get(0));
        if (sv == "true") return true;
        else return false;
    }
    catch (...){
        return false;
    }
}

void SystemItem::to_json_with_children(nlohmann::json &json, SystemItem &main_item) {
    json["id"] = main_item.id;
    if (main_item.parentId.has_value()) json["parentId"] = main_item.parentId.value();
    else json["parentId"] = nlohmann::json::value_t::null;
    if (main_item.url.has_value()) json["url"] = main_item.url.value();
    else json["url"] = nlohmann::json::value_t::null;
    json["size"] = main_item.size;
    json["date"] = main_item.date;
    json["type"] = magic_enum::enum_name(main_item.type);
    auto & main_childrens_json = json["children"] = nlohmann::json::value_t::array;
    if (!main_item.children.empty()){
        //обходим детей
        std::queue<std::pair<nlohmann::json*, SystemItem*>> queue;
        queue.emplace(&main_childrens_json, &main_item);
        while (!queue.empty()){
            auto [cur_json, cur_system_item] = queue.front();
            queue.pop();
            for (auto& child_item : cur_system_item->children){
                nlohmann::json child_json;
                child_json["id"] = child_item.id;
                if (child_item.url.has_value()) child_json["url"] = child_item.url.value();
                else child_json["url"] = nlohmann::json::value_t::null;
                child_json["size"] = child_item.size;
                child_json["date"] = child_item.date;
                child_json["type"] = magic_enum::enum_name(child_item.type);
                if (child_item.parentId.has_value()) child_json["parentId"] = child_item.parentId.value();
                else child_json["parentId"] = nlohmann::json::value_t::null;
                if (!child_item.children.empty()){
                    auto& children_node = child_json["children"] = nlohmann::json::value_t::array;
                    queue.emplace(&children_node, &child_item);
                }
                else child_json["children"] = nlohmann::json::value_t::null;
                cur_json->emplace_back(std::move(child_json));
            }
        }
    }
}

std::optional<SystemItem> SystemItem::from_json(const nlohmann::json &json) {
    SystemItem main_item;
    auto json_it = json.find("id");
    if (json_it == json.end() || !json_it->is_string()){
        return {};
    }
    main_item.id = json_it.value();
    json_it = json.find("url");
    if (json_it != json.end()){
        if (json_it->is_string()){
            main_item.url = json_it.value();
        }
        else if (!json_it->is_null()) return {};
    }
    json_it = json.find("size");
    if (json_it!=json.end()){
        if (json_it->is_number_integer() && json_it->is_number_unsigned()){
            main_item.size = json_it.value();
        }
        else return {};
    }
    else main_item.size = 0;
    json_it = json.find("parentId");
    if (json_it != json.end()){
        if (json_it->is_string()){
            main_item.parentId = json_it.value();
        }
        else if (!json_it->is_null()) return {};
    }
    else return {};
    json_it = json.find("type");
    if (json_it == json.end() || !json_it->is_string()){
        return {};
    }
    std::string temp_type;
    json_it->get_to(temp_type);
    auto check = magic_enum::enum_cast<SystemItemType>(temp_type);
    if (check.has_value()){
        main_item.type = check.value();
        return main_item;
    }
    else return {};
}

SystemItemType SystemItem::get_type() const {
    return type;
}

const std::string &SystemItem::get_id() const {
    return id;
}

const std::optional<std::string> &SystemItem::get_url() const {
    return url;
}

const std::optional<std::string> &SystemItem::get_parent_id() const {
    return parentId;
}

uint64_t SystemItem::get_size() const {
    return size;
}

const std::string &SystemItem::get_date() const {
    return date;
}

const std::vector<SystemItem> &SystemItem::get_childrens() const {
    return children;
}
