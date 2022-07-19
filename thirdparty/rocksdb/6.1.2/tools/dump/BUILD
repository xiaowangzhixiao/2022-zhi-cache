cc_library(
  name = 'db_dump_tool',
  srcs = 'db_dump_tool.cc',
  incs = ['.', '../../include/', '../../../../snappy', '../../'],

  defs = ['OS_LINUX', 'ROCKSDB_PLATFORM_POSIX', 'ROCKSDB_LIB_IO_POSIX',
          'SNAPPY', 'GFLAGS', 'ROCKSDB_MALLOC_USABLE_SIZE',
          'ROCKSDB_PTHREAD_ADAPTIVE_MUTEX', 'ROCKSDB_BACKTRACE'],
  deps = [
    '//thirdparty/rocksdb/6.1.2:rocksdb-6.1.2',
  ],
)

cc_binary(
  name = 'rocksdb_dump',
  srcs = 'rocksdb_dump.cc',
  deps = [
    ':db_dump_tool',
  ],
)

cc_binary(
  name = 'rocksdb_undump',
  srcs = 'rocksdb_undump.cc',
  deps = [
    ':db_dump_tool',
  ],
)
