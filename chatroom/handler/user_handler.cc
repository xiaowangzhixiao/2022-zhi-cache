#include "user_handler.h"
#include "chatroom/utils/token_util.h"
#include "chatroom/utils/param_parse.h"

namespace chatroom {
void UserHandler::OnPost(const flare::HttpRequest& request,
                         flare::HttpResponse* response,
                         flare::HttpServerContext* context) {
  response->set_body("success\n");
  response->set_status(flare::HttpStatus::OK);
}

void UserLoginHandler::OnGet(const flare::HttpRequest& request,
                             flare::HttpResponse* response,
                             flare::HttpServerContext* context) {
  ParamParse param_parse(request.uri());
  std::cout << "request uri:" << request.uri() << std::endl;
  auto username = param_parse.Get("username");
  auto password = param_parse.Get("password");
  if (!username || !password) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  } 
  // TODO: check username and password
  auto token = TokenUtil::Instance()->GenerateToken(username.value());
  response->set_body(token);
  response->set_status(flare::HttpStatus::OK);
}

void UserEnterRoomHandler::OnPut(const flare::HttpRequest& request,
                                 flare::HttpResponse* response,
                                 flare::HttpServerContext* context) {
  response->set_body("enter room\n");
  response->set_status(flare::HttpStatus::OK);
}
}  // namespace chatroom
