#include "cache/handler/zset_handler.h"
#include "flare/rpc/http_handler.h"

namespace zhi {
namespace cache {

void ZaddHandler::OnPost(const flare::HttpRequest& request,
                         flare::HttpResponse* response,
                         flare::HttpServerContext* context) {
  response->set_status(flare::HttpStatus::OK);
}

void ZrangeHandler::OnPost(const flare::HttpRequest& request,
                           flare::HttpResponse* response,
                           flare::HttpServerContext* context) {
  response->set_status(flare::HttpStatus::OK);
}

void ZrmvHandler::OnGet(const flare::HttpRequest& request,
                        flare::HttpResponse* response,
                        flare::HttpServerContext* context) {
  response->set_status(flare::HttpStatus::OK);
}
}  // namespace cache
}  // namespace zhi
