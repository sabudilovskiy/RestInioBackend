//
// Created by MRV on 9/14/2022.
//

#ifndef RESTINOBACKEND_HANDLER_NOT_MATCHED_H
#define RESTINOBACKEND_HANDLER_NOT_MATCHED_H
#include "include_handlers"
auto handler_not_matched(std::shared_ptr <restinio::request_t> req) {
    auto response = req->create_response();
    response.set_body("not matched");
    response.header().status_code(restinio::status_code::no_content);
    return response.done();
}
#endif //RESTINOBACKEND_HANDLER_NOT_MATCHED_H
