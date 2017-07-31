package(default_visibility = ["//visibility:public"])
licenses(["notice"])

cc_binary(
  name = "pinger",
  includes = ["src"],
  copts = [
    "-g",
    "-O2",
    "-Wall",
    "-std=c++11"
  ],
  nocopts = "-fPIC",
  linkstatic = 1,
  srcs = glob([
	"src/pinger/*.h",
	"src/pinger/*.cc"
  ]),
  deps = [
    "@bebase//slog",
    "@third_party//gflags-221",
    "@third_party//libev-422"
  ],
)


