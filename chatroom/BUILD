cc_binary(
  name = 'server',
  srcs = [
    'server.cc',
  ],
  deps = [
    '//chatroom/filter:login_filter',
    '//chatroom/handler:handler',
    '//flare:init',
    '//flare/fiber:fiber',
    '//flare/init:override_flag',
    '//flare/rpc:rpc',
    '//flare/rpc/builtin/detail:uri_matcher',
    '//thirdparty/gflags:gflags',
  ],
)