cc_library(
  name = 'local_db',
  srcs = [
    'local_db.cc',
  ],
  hdrs = glob('*.h'),
  deps = [
    '//thirdparty/glog:glog',
    '//thirdparty/rocksdb:rocksdb',
  ],
  visibility = "//cache/...",
)

cc_test(
    name = 'local_db_test',
    srcs = 'local_db_test.cc',
    deps = [
        ':local_db',
    ],
)
