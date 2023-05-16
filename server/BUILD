load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

cc_library(
    name = "redis-server-library",
    srcs = [
        "StorageService.cpp"
    ],
    hdrs = glob(["CallData/*.h"]) + ["StorageService.h"],
    deps = [
        "//proto:dictionary_library"
    ]
)

cc_binary(
    name = "redis-server-run",
    srcs = [
        "ServerRun.cpp"
    ],
    copts = ["-Iserver/CallData"],
    deps = [
        ":redis-server-library",
    ],
)