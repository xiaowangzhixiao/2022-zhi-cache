#include "chatroom/filter/login_filter.h"

#include "thirdparty/jwt-cpp/jwt-cpp/jwt.h"

namespace chatroom {

flare::HttpFilter::Action LoginFilter::OnFilter(flare::HttpRequest* request,
                             flare::HttpResponse* response,
                             flare::HttpServerContext* context) {
  return LoginFilter::Action::KeepProcessing;
}

}  // namespace chatroom
