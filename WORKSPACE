local_repository(
    name = "third_party",
    path = "/home/jiu/Projects/cpp/third_party"
)
local_repository(
    name = "bebase",
    path = "/home/jiu/Projects/cpp/bebase"
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#http_archive(
#    name = "com_google_protobuf",
#    sha256 = "cef7f1b5a7c5fba672bec2a319246e8feba471f04dcebfe362d55930ee7c1c30",
#    strip_prefix = "protobuf-3.5.0",
#    urls = ["https://github.com/google/protobuf/archive/v3.5.0.zip"],
#)

http_archive(
    name   = "com_github_google_glog",
    build_file = "//bazel:glog.BUILD",
    sha256 = "9e1b54eb2782f53cd8af107ecf08d2ab64b8d0dc2b7f5594472f3bd63ca85cdc",
    strip_prefix = "glog-0.4.0",
    urls = ["https://github.com/google/glog/archive/v0.4.0.zip"],
)

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
git_repository(
    name = "com_github_gflags_gflags",
    remote = "https://github.com/gflags/gflags.git",
    #tags = ["v2.2.2"],
    commit = "e171aa2d15ed9eb17054558e0b3a6a413bb01067",
)
#git_repository(
#    name = "com_google_protobuf",
#    commit = "6973c3a5041636c1d8dc5f7f6c8c1f3c15bc63d6",
#    remote = "https://github.com/protocolbuffers/protobuf.git",
#)
bind(
    name = "gflags",
    actual = "@com_github_gflags_gflags//:gflags",
)

http_archive(
    name = "com_google_protobuf",
    strip_prefix = "protobuf-master",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/master.zip"],
)

http_archive(
    name = "bazel_skylib",
    sha256 = "bbccf674aa441c266df9894182d80de104cabd19be98be002f6d478aaa31574d",
    strip_prefix = "bazel-skylib-2169ae1c374aab4a09aa90e65efe1a3aad4e279b",
    urls = ["https://github.com/bazelbuild/bazel-skylib/archive/2169ae1c374aab4a09aa90e65efe1a3aad4e279b.tar.gz"],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

load("@bazel_skylib//lib:versions.bzl", "versions")

versions.check(minimum_bazel_version = "0.5.4")

