package(default_visibility = ["//visibility:public"])
licenses(["notice"])

cc_binary(
  name = "pinger",
  includes = ["src"],
  copts = [
    "-g",
    "-O2",
    "-Wall",
    "-std=c++11",
    "-pthread",
    "-lpthraed",
  ],
  nocopts = "-fPIC",
  linkstatic = 1,
  srcs = glob([
	"src/pinger/*.h",
	"src/pinger/*.cc"
  ]),
  deps = [
    #"@bebase//slog",
    "@com_github_gflags_gflags//:gflags",
    "@com_github_google_glog//:glog",
    "@third_party//libev-422",
    "//ccbase",
    "//common",
    #"//grpc:grpc",
    #"//grpc:grpc++",
    #"//grpc:grpc_cpp_plugin",
    "//proto:nodes_detector_proto"
    #"//hyperudp",
  ],
)

#cc_binary(
#    name="leveldb_test",
#    copts = [
#        "-g",
#        "-O2",
#        #"-static",
#        "-std=c++11"
#    ],
#    linkstatic=1,
#    srcs = [
#        #"test/pinger/leveldb_test.cc"
#    ],
#    deps = [
#            "@third_party//leveldb-120"
#    ],
#)


