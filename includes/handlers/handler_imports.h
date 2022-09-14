//
// Created by MRV on 9/12/2022.
//

#ifndef RESTINOBACKEND_HANDLER_IMPORTS_H
#define RESTINOBACKEND_HANDLER_IMPORTS_H
#include "include_handlers"
#include <vector>
#include "unordered_map"
#include <queue>

#include <unordered_map>
auto handler_imports(std::shared_ptr <restinio::request_t> req, restinio::router::route_params_t params,
                     std::shared_ptr <tao::pq::connection> bd) {
    std::string req_body = req->body();
    auto json = nlohmann::json::parse(req_body, nullptr, false);
    if (json.is_discarded()){
        return answer_validation_falied(req);
    }
    if (json.type() != nlohmann::json::value_t::object){
        return answer_validation_falied(req);
    }
    if (json.size() != 2){
        return answer_validation_falied(req);
    }
    auto json_items_it = json.find("items");
    if (json.end() == json_items_it || !json_items_it->is_array()){
        return answer_validation_falied(req);
    }
    auto json_date_it = json.find("updateDate");
    if (json.end() == json_date_it || !json_date_it->is_string()){
        return answer_validation_falied(req);
    }
    std::string_view date;
    json_date_it->get_to(date);
    //проверим дату на корректность
    auto checkDate = bd->execute("SELECT is_tswtz($1)", date);
    std::string_view sv = checkDate.get(0,0);
    if (sv != "t"){
        return answer_validation_falied(req);
    }
    bool ok = true;
    //граф зависимостей, ключ - id, значение - родитель
    std::unordered_map<std::string_view, std::string_view> graph;
    //ключ - id
    std::unordered_map<std::string_view , std::optional<SystemItem>> system_items;
    //после одного прохода у нас бдует словарь, который позволяет определить, если у элемента зависимость от тех, который остались
    for (auto& json_item : *json_items_it){
        auto system_item = SystemItem::from_json(json_item);
        if (system_item.has_value()){
            //если нет парента, то сразу импортим
            if (!system_item->get_parent_id().has_value()){
                ok = system_item->import(bd, date) && ok;
            }
            //в противном случае заносим в очередь
            else {
                graph.emplace(system_item->get_id().data(), system_item->get_parent_id()->data());
                system_items.emplace(system_item->get_id().data(), std::move(system_item));
            }
        }
        else ok = false;
    }
    //пока в графах что-то есть
    //проходим их
    while (!graph.empty()){
        auto it = graph.begin();
        auto end = graph.end();
        //идём до конца графа
        while (it!=end){
            //если parentId нет в графе, то импортим его
            if (graph.find(it->second) == end){
                auto it_elem = system_items.find(it->first);
                ok = it_elem->second->import(bd, date) && ok;
                auto it_next = it;
                ++it_next;
                graph.erase(it);
                system_items.erase(it_elem);
                it = it_next;
            }
            else ++it;
        }
    }
    if (!ok){
        return answer_validation_falied(req);
    }
    else{
        auto response = req->create_response();
        response.header().status_code(restinio::status_code::ok);
        return response.done();
    }
}
#endif //RESTINOBACKEND_HANDLER_IMPORTS_H
