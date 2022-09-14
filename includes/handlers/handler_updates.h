//
// Created by MRV on 9/13/2022.
//

#ifndef RESTINOBACKEND_HANDLER_UPDATES_H
#define RESTINOBACKEND_HANDLER_UPDATES_H

#include "include_handlers"
#include "items/SystemItem"

auto handler_updates(std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params, std::shared_ptr<tao::pq::connection> bd){
    try{
        auto query = restinio::parse_query<restinio::parse_query_traits::relaxed>(req->header().query());
        if (!query.has("date")){
            return answer_validation_falied(req);
        }
        std::string_view date = query["date"];
        std::string request = "SELECT * from get_updates($1)";
        auto result = bd->execute(request, date);
        if (result.empty()){
            return answer_item_not_found(req);
        }
        nlohmann::json result_json = nlohmann::json::value_t::object;
        auto& json_items = result_json["items"] = nlohmann::json::value_t::array;
        for (auto& row : result){
            json_items.emplace_back(parse_SystemItem_to_json(row));
        }
        auto response = req->create_response();
        response.set_body(nlohmann::to_string(json_items));
        response.header().status_code(restinio::status_code::ok);
        return response.done();
    }
    catch (...){
        return answer_validation_falied(req);
    }
}
#endif //RESTINOBACKEND_HANDLER_UPDATES_H
