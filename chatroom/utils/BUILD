cc_library(
    name = "token_util",
    hdrs = "token_util.h",
    srcs = "token_util.cc",
    deps = [
        "//thirdparty/jwt-cpp:jwt-cpp",
        "//flare/base:never_destroyed",
        "//flare/base:string",
    ],
    visibility = "//chatroom/..."
)

cc_test(
    name = "jwt_test",
    srcs = "jwt_test.cc",
    deps = [
        ":token_util",
        "//thirdparty/jwt-cpp:jwt-cpp",
    ]
)

cc_library(
    name = "param_parse",
    hdrs = "param_parse.h",
    srcs = "param_parse.cc",
    visibility = "//chatroom/..."
)

