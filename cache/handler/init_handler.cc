#include "cache/handler/init_handler.h"

#include "flare/rpc/http_handler.h"
#include "cache/manager/localdb_manager.h"

namespace zhi {
namespace cache {
void InitQueryHandler::OnGet(const flare::HttpRequest& request,
                             flare::HttpResponse* response,
                             flare::HttpServerContext* context) {
  if (!LocaldbManager::Instance()->Query()) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }

  response->set_body("ok");
  response->set_status(flare::HttpStatus::OK);
}
}  // namespace cache
}  // namespace zhi
