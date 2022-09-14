//
// Created by MRV on 9/13/2022.
//

#ifndef RESTINOBACKEND_HANDLERS_NODE_ID_HISTORY_H
#define RESTINOBACKEND_HANDLERS_NODE_ID_HISTORY_H
#include "include_handlers"
#include "items/SystemItem"

auto handler_node_id_history(std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params, std::shared_ptr<tao::pq::connection> bd){
    std::string_view id = params["id"];
    if (id.empty() || id == "null"){
        return answer_validation_falied(req);
    }
    std::optional<std::string> dateStart, dateEnd;
    auto query = restinio::parse_query<restinio::parse_query_traits::relaxed>(req->header().query());
    if (query.has("dateStart")) dateStart = query["dateStart"];
    if (query.has("dateEnd")) dateEnd = query["dateEnd"];
    //пробуем сделать запрос
    try{
        std::string request = "select * from history($1, $2, $3)";
        auto result = bd->execute(std::move(request), id, dateStart, dateEnd);
        if (result.empty()){
            return answer_item_not_found(req);
        }
        nlohmann::json json_result = nlohmann::json::value_t::object;
        auto& items = json_result["items"] = nlohmann::json::value_t::array;
        for (auto& row : result){
            items.emplace_back(parse_SystemItem_to_json(row));
        }
        auto response = req->create_response();
        response.set_body(to_string(json_result));
        response.header().status_code(restinio::status_code::ok);
        return response.done();
    }
    //если эксепшен, то входные данные битые
    catch(...){
        return answer_validation_falied(req);
    }
}
#endif //RESTINOBACKEND_HANDLERS_NODE_ID_HISTORY_H
