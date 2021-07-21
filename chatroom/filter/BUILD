cc_library(
    name = "login_filter",
    hdrs = ["login_filter.h"], 
    srcs = ["login_filter.cc"], 
    deps = [
        "//chatroom/utils:token_util",
        "//thirdparty/jwt-cpp:jwt-cpp",
        "//flare/rpc:http",
        "//flare/base:string",
        "//flare/base/encoding:base64",
    ],
    visibility = '//chatroom/...',
)

