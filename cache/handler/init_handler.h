#pragma once

#include "flare/rpc/http_handler.h"

namespace zhi {
namespace cache {
class InitQueryHandler : public flare::HttpHandler {
 public:
  void OnGet(const flare::HttpRequest& request, flare::HttpResponse* response,
             flare::HttpServerContext* context) override;
};
}  // namespace cache
}  // namespace zhi
