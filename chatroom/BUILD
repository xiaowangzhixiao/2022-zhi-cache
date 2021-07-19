cc_binary(
  name = 'server',
  srcs = [
    'server.cc',
  ],
  deps = [
    '//chatroom/filter:login_filter',
    '//flare:init',
    '//flare/fiber:fiber',
    '//flare/init:override_flag',
    '//flare/rpc:rpc',
    '//thirdparty/gflags:gflags',
  ],
)