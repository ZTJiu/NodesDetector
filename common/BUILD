package(default_visibility = ["//visibility:public"])

cc_library(
  name = "common",
  includes = ["src"],
  copts = [
    "-g",
    "-O2",
    "-Wall",
  ],
  linkopts = [
    "-lrt",
  ],
  nocopts = "-fPIC",
  linkstatic = 1,
  srcs = glob([
    "src/common/*.cc",
    "src/common/*.h",
  ]),
  deps = [],
)
