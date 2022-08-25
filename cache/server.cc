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

#include "cache/handler/init_handler.h"
#include "cache/handler/kv_handler.h"
#include "cache/handler/zset_handler.h"
#include "cache/manager/kv_manager.h"
#include "cache/manager/zset_manager.h"
#include "cache/manager/localdb_manager.h"
#include "flare/fiber/this_fiber.h"
#include "flare/init.h"
#include "flare/fiber/async.h"
#include "flare/init/override_flag.h"
#include "flare/rpc/http_handler.h"

using namespace std::literals;

FLARE_OVERRIDE_FLAG(logtostderr, true);
FLARE_OVERRIDE_FLAG(logbufsecs, 0);
FLARE_OVERRIDE_FLAG(flare_rpc_server_no_builtin_pages, false);

namespace zhi {
namespace cache {
int Entry(int argc, char** argv) {
  KvManager::Instance()->Init();
  ZsetManager::Instance()->Init();
  flare::Server server;
  server.AddProtocol("http");
  server.AddHttpHandler("/init", std::make_unique<InitQueryHandler>());
  server.AddHttpHandler(std::regex("^/query/(.+?)$"),
                        std::make_unique<QueryKeyHandler>());
  server.AddHttpHandler("/add", std::make_unique<AddKeyHandler>());
  server.AddHttpHandler(std::regex("^/del/(.+?)$"),
                        std::make_unique<DelKeyHandler>());
  server.AddHttpHandler("/list", std::make_unique<BatchQueryKeyHandler>());
  server.AddHttpHandler("/batch", std::make_unique<BatchAddKeyHandler>());
  
  server.AddHttpHandler(std::regex("^/zadd/(.+?)$"),
                        std::make_unique<ZaddHandler>());
  server.AddHttpHandler(std::regex("^/zrange/(.+?)$"),
                        std::make_unique<ZrangeHandler>());
  server.AddHttpHandler(std::regex("^/zrmv/(.+?)$"),
                        std::make_unique<ZrmvHandler>());

  server.ListenOn(flare::EndpointFromIpv4("0.0.0.0", 8080));
  FLARE_CHECK(server.Start());
  flare::fiber::Async([] {
    LocaldbManager::Instance()->Init("/mydata");
  });

  while (!flare::CheckForQuitSignal()) {
    flare::this_fiber::SleepFor(1s);
  }
  server.Stop();
  server.Join();
  return 0;
}
}  // namespace cache

}  // namespace zhi

int main(int argc, char** argv) {
  return flare::Start(argc, argv, zhi::cache::Entry);
}
