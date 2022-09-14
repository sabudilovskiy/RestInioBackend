//
// Created by MRV on 9/12/2022.
//
#ifndef RESTINOBACKEND_HANDLER_NODES_ID_H
#define RESTINOBACKEND_HANDLER_NODES_ID_H
#include <nlohmann/json.hpp>
#include <restinio/all.hpp>
#include <string>
#include "items/SystemItem"
#include "answers/answers.h"

auto handler_nodes_id(std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params, std::shared_ptr<tao::pq::connection> bd){
    std::string_view id = params["id"];
    BDError bd_error;
    if (id.empty() || id == "null"){
        return answer_validation_falied(req);
    }
    auto system_item = SystemItem::get_by_id(id, bd, bd_error);
    if (bd_error == BDError::BAD_FORMAT){
        return answer_validation_falied(req);
    }
    else if (bd_error == BDError::NOT_FOUND){
        return answer_item_not_found(req);
    }
    nlohmann::json json;
    SystemItem::to_json_with_children(json, system_item);
    auto response = req->create_response();
    std::string body = to_string(json);
    restinio::http_status_code_t code = restinio::status_code::ok;
    response.header().status_code(code);
    response.set_body(std::move(body));
    return response.done();
}
#endif //RESTINOBACKEND_HANDLER_NODES_ID_H
