# Copyright (C) 2021 THL A29 Limited, a Tencent company. All rights reserved.
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

cc_library(
  name = 'domain_name_resolver',
  hdrs = 'domain_name_resolver.h',
  srcs = 'domain_name_resolver.cc',
  deps = [
    '//flare/base:logging',
    '//flare/base:string',
    '//flare/base/net:endpoint',
    '#resolv',
  ],
  visibility = [
    '//flare/rpc/name_resolver:domain_name_resolver',
    '//flare/rpc/name_resolver:list',
  ],
)

cc_test(
  name = 'domain_name_resolver_test',
  srcs = 'domain_name_resolver_test.cc',
  deps = [
    ':domain_name_resolver',
  ]
)
