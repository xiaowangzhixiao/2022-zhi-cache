# Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
#
# Licensed under the BSD 3-Clause License (the "License"); you may not use this
# file except in compliance with the License. You may obtain a copy of the
# License at
#
# https://opensource.org/licenses/BSD-3-Clause
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

load('//flare/tools/build_rules.bld', 'cc_flare_library')

proto_library(
  name = 'echo_proto',
  srcs = 'echo.proto',
)

cc_flare_library(
  name = 'echo_proto_flare',
  srcs = 'echo.proto',
  deps = ':echo_proto'
)

cc_binary(
  name = 'baidu_std_echo_server',
  srcs = 'baidu_std_echo_server.cc',
  deps = [
    ':echo_proto_flare',
    '//flare:init',
    '//flare/base:exposed_var',
    '//flare/fiber:fiber',
    '//flare/init:override_flag',
    '//flare/rpc:rpc',
    '//thirdparty/gperftools:tcmalloc_and_profiler',
  ]
)
