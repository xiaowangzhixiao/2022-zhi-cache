// Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of the
// License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

#include "flare/rpc/server.h"

#include "chatroom/filter/login_filter.h"
#include "chatroom/handler/user_handler.h"
#include "flare/fiber/this_fiber.h"
#include "flare/init.h"
#include "flare/init/override_flag.h"
#include "flare/rpc/http_handler.h"

using namespace std::literals;

FLARE_OVERRIDE_FLAG(logtostderr, true);
FLARE_OVERRIDE_FLAG(flare_rpc_server_no_builtin_pages, false);

namespace chatroom {

int Entry(int argc, char** argv) {
  flare::Server server;
  server.AddProtocol("http");
  server.AddHttpFilter(std::make_unique<LoginFilter>(flare::detail::UriMatcher(std::regex("^/user(Login([/\?#](.*?))?)?$"))));
  server.AddHttpHandler(std::regex("^/user(/.+)?$"), std::make_unique<UserHandler>());
  server.AddHttpPrefixHandler("/userLogin", std::make_unique<UserLoginHandler>());
  server.AddHttpHandler(std::regex("^/room/(.+?)/enter$"), std::make_unique<UserEnterRoomHandler>());
  server.ListenOn(flare::EndpointFromIpv4("0.0.0.0", 8080));
  FLARE_CHECK(server.Start());

  while (!flare::CheckForQuitSignal()) {
    flare::this_fiber::SleepFor(1s);
  }
  server.Stop();
  server.Join();
  return 0;
}

}  // namespace chatroom

int main(int argc, char** argv) {
  return flare::Start(argc, argv, chatroom::Entry);
}