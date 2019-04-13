package(default_visibility = ["//visibility:public"])


# Convention:
# A cc_proto_library that wraps a proto_library named foo_proto
# should be called foo_cc_proto.
cc_proto_library(
    name = "nodes_detector_proto",
    deps = [":pinger_proto"],
)

# Conventions:
# 1. One proto_library rule per .proto file.
# 2. A file named foo.proto will be in a rule named foo_proto.
proto_library(
    name = "pinger_proto",
    srcs = glob(["src/*.proto"]),
    deps = [
        # Well known protos should be included as deps in the
        # proto_library rules of the source files importing them.
        # A list of all @com_google_protobuf well known protos can
        # seen with:
        # `bazel query 'kind(proto_library, @com_google_protobuf//:all)'`
        "@com_google_protobuf//:any_proto",
    ],
)

