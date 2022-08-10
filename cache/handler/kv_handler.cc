#include "cache/handler/kv_handler.h"

#include <string>
#include <vector>

#include "cache/manager/kv_manager.h"
#include "cache/model/request_model.h"
#include "cache/utils/param_parse.h"
#include "flare/rpc/http_handler.h"
#include "flare/base/logging.h"

namespace zhi {
namespace cache {

/**
 * @brief /query/{key}
 *
 * @param request
 * @param response
 * @param context
 */
void QueryKeyHandler::OnGet(const flare::HttpRequest& request,
                            flare::HttpResponse* response,
                            flare::HttpServerContext* context) {
  ParamParse path_param_parse(request.uri());
  auto path_param_map = path_param_parse.GetPathParam("/query/{key}$");
  auto key = path_param_map["key"];
  if (!key) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  auto* value = KvManager::Instance()->Query(*key);
  if (value == nullptr) {
    response->set_status(flare::HttpStatus::NotFound);
    return;
  }
  response->set_body(*value);
  response->set_status(flare::HttpStatus::OK);
  return;
}

/**
 * @brief /add
 *
 * @param request
 * @param response
 * @param context
 */
void AddKeyHandler::OnPost(const flare::HttpRequest& request,
                           flare::HttpResponse* response,
                           flare::HttpServerContext* context) {
  auto body = request.body();
  InsertRequest insert_req;
  xpack::json::decode(*body, insert_req);
  if (insert_req.key.empty() || insert_req.value.empty()) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  if (!KvManager::Instance()->Add(std::move(insert_req.key),
                                 std::move(insert_req.value))) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  response->set_status(flare::HttpStatus::OK);
  return;
}

/**
 * @brief /del/{key}
 *
 * @param request
 * @param response
 * @param context
 */
void DelKeyHandler::OnGet(const flare::HttpRequest& request,
                          flare::HttpResponse* response,
                          flare::HttpServerContext* context) {
  ParamParse path_param_parse(request.uri());
  auto path_param_map = path_param_parse.GetPathParam("/del/{key}$");
  auto key = path_param_map["key"];
  if (!key) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  KvManager::Instance()->Del(*key);
  response->set_status(flare::HttpStatus::OK);
  return;
}

/**
 * @brief /list
 *
 * @param request
 * @param response
 * @param context
 */
void BatchQueryKeyHandler::OnPost(const flare::HttpRequest& request,
                                  flare::HttpResponse* response,
                                  flare::HttpServerContext* context) {
  auto body = request.body();
  std::vector<std::string> key_vec;
  xpack::json::decode(*body, key_vec);
  if (key_vec.empty()) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  std::vector<InsertRequest> result_vec;
  for (auto&& key : key_vec) {
    auto* value = KvManager::Instance()->Query(key);
    if (value != nullptr) {
      InsertRequest result;
      result.key = std::move(key);
      result.value = *value;
      result_vec.push_back(std::move(result));
    }
  }
  if (result_vec.empty()) {
    response->set_status(flare::HttpStatus::NotFound);
    return;
  }
  response->set_body(xpack::json::encode(result_vec));
  response->set_status(flare::HttpStatus::OK);
  return;
}

/**
 * @brief /batch
 *
 * @param request
 * @param response
 * @param context
 */
void BatchAddKeyHandler::OnPost(const flare::HttpRequest& request,
                                flare::HttpResponse* response,
                                flare::HttpServerContext* context) {
  auto body = request.body();
  std::vector<InsertRequest> insert_req_vec;
  xpack::json::decode(*body, insert_req_vec);
  for (auto&& insert_vec: insert_req_vec) {
    KvManager::Instance()->Add(std::move(insert_vec.key), std::move(insert_vec.value));
  }
  response->set_status(flare::HttpStatus::OK);
  return;
}
}  // namespace cache
}  // namespace zhi
