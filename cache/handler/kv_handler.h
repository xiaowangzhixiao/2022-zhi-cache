#pragma once

#include "flare/rpc/http_handler.h"

namespace zhi {
namespace cache {
class QueryKeyHandler : public flare::HttpHandler {
 public:
  void OnGet(const flare::HttpRequest& request, flare::HttpResponse* response,
             flare::HttpServerContext* context) override;
};

class AddKeyHandler : public flare::HttpHandler {
 public:
  void OnPost(const flare::HttpRequest& request, flare::HttpResponse* response,
              flare::HttpServerContext* context) override;
};

class DelKeyHandler : public flare::HttpHandler {
 public:
  void OnGet(const flare::HttpRequest& request, flare::HttpResponse* response,
             flare::HttpServerContext* context) override;
};

class BatchQueryKeyHandler : public flare::HttpHandler {
 public:
  void OnPost(const flare::HttpRequest& request, flare::HttpResponse* response,
              flare::HttpServerContext* context) override;
};

class BatchAddKeyHandler : public flare::HttpHandler {
 public:
  void OnPost(const flare::HttpRequest& request, flare::HttpResponse* response,
              flare::HttpServerContext* context) override;
};
}  // namespace cache
}  // namespace zhi
