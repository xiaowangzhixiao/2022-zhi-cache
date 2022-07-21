#include "cache/handler/init_handler.h"

#include "flare/rpc/http_handler.h"

namespace zhi {
namespace cache {
void InitQueryHandler::OnGet(const flare::HttpRequest& request,
                             flare::HttpResponse* response,
                             flare::HttpServerContext* context) {
  response->set_body("ok");
  response->set_status(flare::HttpStatus::OK);
}
}  // namespace cache
}  // namespace zhi
