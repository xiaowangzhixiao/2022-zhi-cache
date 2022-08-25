cc_binary(
  name = 'server',
  srcs = [
    'server.cc',
  ],
  deps = [
    '//cache/handler:handler',
    '//cache/manager:manager',
    '//flare:init',
    '//flare/fiber:fiber',
    '//flare/init:override_flag',
    '//flare/rpc:rpc',
    '//flare/rpc/builtin/detail:uri_matcher',
    '//thirdparty/gflags:gflags',
  ],
)

package(
  name = 'cache_server_package',
  shell = True,
  type = 'tgz',
  srcs = [
      ('script/start.sh','start.sh'),
      ('$(location //cache:server)', 'cache_server'),
  ],
)
