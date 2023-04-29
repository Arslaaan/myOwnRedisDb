load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

cc_library(
    name = "redis-server-library",
    srcs = [
        "Server.cpp"
    ],
    hdrs = [
        "Server.h"
    ],
    deps = [
        "//proto:dictionary_library"
    ]
)

cc_binary(
    name = "redis-server-run",
    srcs = [
        "ServerRun.cpp"
    ],
    deps = [
        ":redis-server-library",
    ],
)