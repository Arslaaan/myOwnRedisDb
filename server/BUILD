cc_library(
    name = "redis-server-library",
    srcs = [
        "Server.cpp"
    ],
    hdrs = [
        "Server.h"
    ],
)

cc_binary(
    name = "redis-server-run",
    srcs = [
        "ServerRun.cpp"
    ],
    deps = [
        ":redis-server-library",
        "//proto:dictionary_library",
    ],
)