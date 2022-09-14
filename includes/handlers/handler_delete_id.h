//
// Created by MRV on 9/13/2022.
//

#ifndef RESTINOBACKEND_HANDLER_DELETE_ID_H
#define RESTINOBACKEND_HANDLER_DELETE_ID_H

#include "include_handlers"

auto handler_delete_id(std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params, std::shared_ptr<tao::pq::connection> bd){
    try{
        std::string_view id = params["id"];
        auto query = restinio::parse_query<restinio::parse_query_traits::relaxed>(req->header().query());
        if (!query.has("date")){
            return answer_validation_falied(req);
        }
        std::string_view date = query["date"];
        if (id.empty()){
            return answer_validation_falied(req);
        }
        std::string req_bd = "SELECT delete_file($1, $2)";
        auto result = bd->execute(req_bd, id, date);
        std::string_view sv = result.get(0,0);
        if (sv == "t") {
            return answer_ok(req);
        }
        else return answer_item_not_found(req);
    }
    catch(...){
        return answer_validation_falied(req);
    }
}
#endif //RESTINOBACKEND_HANDLER_DELETE_ID_H
