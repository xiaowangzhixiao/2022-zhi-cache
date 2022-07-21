#pragma once

#include "flare/rpc/http_handler.h"

namespace zhi {
namespace cache {
class ZaddHandler : public flare::HttpHandler {
 public:
  void OnPost(const flare::HttpRequest& request, flare::HttpResponse* response,
              flare::HttpServerContext* context) override;
};

class ZrangeHandler : public flare::HttpHandler {
 public:
  void OnPost(const flare::HttpRequest& request, flare::HttpResponse* response,
              flare::HttpServerContext* context) override;
};

class ZrmvHandler : public flare::HttpHandler {
 public:
  void OnGet(const flare::HttpRequest& request, flare::HttpResponse* response,
             flare::HttpServerContext* context) override;
};
}  // namespace cache
}  // namespace zhi
