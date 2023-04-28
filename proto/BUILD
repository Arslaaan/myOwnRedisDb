proto_library(
    name = "dictionary_proto",
    srcs = ["dictionary.proto"],
    visibility = ["//visibility:public"],
    deps = ["@com_google_protobuf//:any_proto"],
)

load("@rules_proto_grpc//cpp:defs.bzl", "cpp_grpc_library")

cpp_grpc_library(
    name = "dictionary_library",
    protos = ["//proto:dictionary_proto"],
)