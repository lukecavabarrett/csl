cc_library(
    name = "uint",
    hdrs = ["uint.h"],
    visibility = ["//visibility:public"],
    deps = ["@gmp//:lib"],
)

cc_test(
    name = "uint_test",
    srcs = ["uint_test.cc"],
    deps = [
        ":uint",
        "@googletest//:gtest_main",
    ],
)
