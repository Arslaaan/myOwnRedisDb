load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "redis-server-library",
    srcs = [
        "StorageService.cpp",
    ],
    hdrs =
        glob(["CallData/*.h"]) +
        glob(["cereal/**/*.hpp"]) +
        [
            "StorageService.h",
            "utils/GprTime.h"
        ],
    copts = [
        "-Iserver",
    ],
    deps = [
        "//proto:dictionary_library",
    ],
)

cc_binary(
    name = "redis-server-run",
    srcs = [
        "ServerRun.cpp",
    ],
    copts = [
        "-Iserver",
        "-Iserver/CallData",
    ],
    deps = [
        ":redis-server-library",
    ],
)
