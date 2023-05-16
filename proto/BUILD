load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_proto_grpc//cpp:defs.bzl", "cpp_grpc_library")

proto_library(
    name = "dictionary_proto",
    srcs = ["dictionary.proto"],
    visibility = ["//visibility:public"],
    deps = ["@com_google_protobuf//:any_proto"],
)

cpp_grpc_library(
    name = "dictionary_library",
    protos = ["//proto:dictionary_proto"],
    visibility = ["//visibility:public"],
)
