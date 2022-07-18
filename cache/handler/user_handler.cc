#include "user_handler.h"

#include <regex>
#include <unordered_map>

#include "cache/utils/param_parse.h"
#include "thirdparty/xpack/json.h"

namespace cache {
void UserHandler::OnPost(const flare::HttpRequest& request,
                         flare::HttpResponse* response,
                         flare::HttpServerContext* context) {
  auto body = request.body();
  User user;
  xpack::json::decode(*body, user);
  if (user.username.empty()) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  if (UserManager::Instance()->CreateUser(std::move(user))) {
    response->set_status(flare::HttpStatus::OK);
  } else {
    response->set_status(flare::HttpStatus::BadRequest);
  }
}

void UserHandler::OnGet(const flare::HttpRequest& request,
                        flare::HttpResponse* response,
                        flare::HttpServerContext* context) {
  ParamParse path_param_parse(request.uri());
  auto path_param_map = path_param_parse.GetPathParam("/user/{username}");
  auto username = path_param_map["username"];
  if (!username) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  auto user = UserManager::Instance()->Get(*username);
  if (!user) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }

  UserResponse user_response;
  user_response.firstName = user->firstName;
  user_response.lastName = user->lastName;
  user_response.email = user->email;
  user_response.phone = user->phone;

  response->set_body(xpack::json::encode(user_response));
  response->set_status(flare::HttpStatus::OK);
}

void UserLoginHandler::OnGet(const flare::HttpRequest& request,
                             flare::HttpResponse* response,
                             flare::HttpServerContext* context) {
  ParamParse param_parse(request.uri());
  auto username = param_parse.Get("username");
  auto password = param_parse.Get("password");
  if (!username || !password ||
      !UserManager::Instance()->Check(*username, *password)) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  auto token = TokenUtil::Instance()->GenerateToken(*username);
  response->set_body(token);
  response->set_status(flare::HttpStatus::OK);
}

void UserEnterRoomHandler::OnPut(const flare::HttpRequest& request,
                                 flare::HttpResponse* response,
                                 flare::HttpServerContext* context) {
  auto username = request.headers()->TryGet("username");
  if (!username) {
    response->set_status(flare::HttpStatus::BadRequest);
  }
  ParamParse path_param_parse(request.uri());
  auto path_param_map = path_param_parse.GetPathParam("/room/{roomId}/enter");
  auto roomId = path_param_map["roomId"];
  if (!roomId) {
    response->set_status(flare::HttpStatus::BadRequest);
    return;
  }
  // TODO: check roomId
  auto user = UserManager::Instance()->Get(std::string(*username));
  if (user) {
    (*user).roomId = *roomId;
  }

  response->set_status(flare::HttpStatus::OK);
}

}  // namespace cache
