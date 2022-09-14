//
// Created by MRV on 9/13/2022.
//

#ifndef RESTINOBACKEND_SUPPORT_H
#define RESTINOBACKEND_SUPPORT_H
#include <nlohmann/json.hpp>
#include <tao/pq/result.hpp>
#include "items/SystemItem"
inline nlohmann::json parse_SystemItem_to_json(const tao::pq::row& row){
    nlohmann::json object = nlohmann::json::value_t::object;
    object["id"]=row.get(I_id);
    if (!row.is_null(I_url)) object["url"] = row.get(I_url);
    else object["url"];
    if (!row.is_null(I_parentId)) object["parentId"] = row.get(I_parentId);
    else object["parentId"];
    object["date"] = row.get(I_date);
    object["size"] = row.get(I_size);
    object["type"] = row.get(I_type);
    return object;
}
#endif //RESTINOBACKEND_SUPPORT_H
