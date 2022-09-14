//
// Created by MRV on 9/13/2022.
//

#ifndef RESTINOBACKEND_SERVER_H
#define RESTINOBACKEND_SERVER_H
#include "restinio/router/express.hpp"
#include <tao/pq.hpp>
#include <memory>
#include "handlers/all_handlers.h"

auto make_router(std::shared_ptr<tao::pq::connection> conn){
    auto router = std::make_unique<restinio::router::express_router_t<>>();
    router->http_post( R"(/imports)",
                       [=]( std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params ){
                           return handler_imports(req, std::move(params), conn);
                       } );
    router->http_delete(R"(/delete/:id(.*))",
                        [=]( std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params ){
                            return handler_delete_id(req, std::move(params), conn);
                        } );
    router->http_get(R"(/node/:id(.*)/history)",
                     [=]( std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params ) {
                         return handler_node_id_history(req, std::move(params), conn);
                     });
    router->http_get(
            R"(/nodes/:id(.*))",
            [=](std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params) {
                return handler_nodes_id(req, std::move(params), conn);
            });
    router->http_get(
            R"(/updates(.*))",
            [=](std::shared_ptr<restinio::request_t> req, restinio::router::route_params_t params) {
                return handler_updates(req, std::move(params), conn);
            });
    router->non_matched_request_handler(
            [](std::shared_ptr<restinio::request_t> req){
                return handler_not_matched(req);
            });
    return router;
}
#endif //RESTINOBACKEND_SERVER_H
