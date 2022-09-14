//
// Created by MRV on 9/13/2022.
//

#ifndef RESTINOBACKEND_ANSWERS_H
#define RESTINOBACKEND_ANSWERS_H
#include "restinio/http_headers.hpp"
#include <memory>
auto answer_validation_falied(std::shared_ptr<restinio::request_t> req) -> decltype(req->create_response().done()) {
    auto response = req->create_response();
    response.set_body("Validation failed");
    response.header().status_code(restinio::status_code::bad_request);
    return response.done();
}
auto answer_item_not_found(std::shared_ptr<restinio::request_t> req) -> decltype(req->create_response().done()) {
    auto response = req->create_response();
    response.set_body("Item not found");
    response.header().status_code(restinio::status_code::not_found);
    return response.done();
}
auto answer_ok(std::shared_ptr<restinio::request_t> req) -> decltype(req->create_response().done()){
    auto response = req->create_response();
    response.header().status_code(restinio::status_code::ok);
    return response.done();
}

#endif //RESTINOBACKEND_ANSWERS_H
