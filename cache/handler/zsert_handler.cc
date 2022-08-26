#include "cache/handler/zset_handler.h"
#include "cache/manager/zset_manager.h"
#include "cache/model/request_model.h"
#include "cache/utils/param_parse.h"
#include "flare/base/logging.h"
#include "flare/rpc/http_handler.h"

namespace zhi {
namespace cache {

/**
 * @brief /zadd/{key}
 *
 * @param request @see ZaddRequest
 * @param response 200
 * @param context
 */
void ZaddHandler::OnPost(const flare::HttpRequest& request,
                         flare::HttpResponse* response,
                         flare::HttpServerContext* context) {
  ParamParse path_param_parse(request.uri());
  auto path_param_map = path_param_parse.GetPathParam("/zadd/{key}$");
  auto key = path_param_map["key"];
  if (!key) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  ZaddRequest zadd_request{};
  auto body = request.body();
  xpack::json::decode(*body, zadd_request);

  if (!ZsetManager::Instance()->Add(std::move(*key), zadd_request.score,
                                    std::move(zadd_request.value))) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  response->set_status(flare::HttpStatus::OK);
}

/**
 * @brief /zrange/{key}
 *
 * @param request @see ZrangeRequest
 * @param response List of @see ZaddRequest
 * @param context
 */
void ZrangeHandler::OnPost(const flare::HttpRequest& request,
                           flare::HttpResponse* response,
                           flare::HttpServerContext* context) {
  ParamParse path_param_parse(request.uri());
  auto path_param_map = path_param_parse.GetPathParam("/zrange/{key}$");
  auto key = path_param_map["key"];
  if (!key) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  ZrangeRequest zrange_request{};
  auto body = request.body();
  xpack::json::decode(*body, zrange_request);
  auto range_res = ZsetManager::Instance()->Range(
      *key, zrange_request.min_score, zrange_request.max_score);
  if (!range_res.first) {
    response->set_status(flare::HttpStatus::NotFound);
    return;
  }
  response->set_body(xpack::json::encode(range_res.second));
  response->set_status(flare::HttpStatus::OK);
}

/**
 * @brief /zrmv/{key}/{value}
 *
 * @param request
 * @param response
 * @param context
 */
void ZrmvHandler::OnGet(const flare::HttpRequest& request,
                        flare::HttpResponse* response,
                        flare::HttpServerContext* context) {
  ParamParse path_param_parse(request.uri());
  auto path_param_map = path_param_parse.GetPathParam("/zrmv/{key}/{value}$");
  auto key = path_param_map["key"];
  auto value = path_param_map["value"];
  if (!key || !value) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  ZsetManager::Instance()->Del(*key, *value);
  response->set_status(flare::HttpStatus::OK);
}
}  // namespace cache
}  // namespace zhi
