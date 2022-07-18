cc_binary(
  name = 'server',
  srcs = [
    'server.cc',
  ],
  deps = [
    '//cache/filter:login_filter',
    '//cache/handler:handler',
    '//flare:init',
    '//flare/fiber:fiber',
    '//flare/init:override_flag',
    '//flare/rpc:rpc',
    '//flare/rpc/builtin/detail:uri_matcher',
    '//thirdparty/gflags:gflags',
  ],
)