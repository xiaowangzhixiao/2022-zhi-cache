cc_binary(
  name = 'db_repl_stress',
  srcs = 'db_repl_stress.cc',
  deps = [
    '//thirdparty/rocksdb/6.1.2:rocksdb-6.1.2',
  ],
)

cc_binary(
  name = 'blob_dump',
  srcs = 'blob_dump.cc',
  incs = ['.', '../', '../include/', '../../../snappy'],
  defs = ['OS_LINUX', 'ROCKSDB_PLATFORM_POSIX', 'ROCKSDB_LIB_IO_POSIX',
          'SNAPPY', 'GFLAGS', 'ROCKSDB_MALLOC_USABLE_SIZE',
          'ROCKSDB_PTHREAD_ADAPTIVE_MUTEX', 'ROCKSDB_BACKTRACE'],
  deps = [
    '//thirdparty/rocksdb/6.1.2:rocksdb-6.1.2',
    '//thirdparty/rocksdb/6.1.2/tools/dump:db_dump_tool',
  ],
)

cc_binary(
  name = 'sst_dump_tool',
  srcs = [
    'sst_dump_tool.cc',
    'sst_dump.cc',
  ],
  incs = ['.', '../', '../include/', '../../../snappy'],
  defs = ['OS_LINUX', 'ROCKSDB_PLATFORM_POSIX', 'ROCKSDB_LIB_IO_POSIX',
          'SNAPPY', 'GFLAGS', 'ROCKSDB_MALLOC_USABLE_SIZE',
          'ROCKSDB_PTHREAD_ADAPTIVE_MUTEX', 'ROCKSDB_BACKTRACE'],
  deps = [
    '//thirdparty/rocksdb/6.1.2:rocksdb-6.1.2',
    '//thirdparty/rocksdb/6.1.2/tools/dump:db_dump_tool',
  ],
)

cc_binary(
  name = 'ldb_tool',
  srcs = [
    'ldb_tool.cc',
    'ldb.cc',
    'ldb_cmd.cc',
  ],
  incs = ['.', '../', '../include/', '../../../snappy'],
  defs = ['OS_LINUX', 'ROCKSDB_PLATFORM_POSIX', 'ROCKSDB_LIB_IO_POSIX',
          'SNAPPY', 'GFLAGS', 'ROCKSDB_MALLOC_USABLE_SIZE',
          'ROCKSDB_PTHREAD_ADAPTIVE_MUTEX', 'ROCKSDB_BACKTRACE'],
  deps = [
    '//thirdparty/rocksdb/6.1.2:rocksdb-6.1.2',
  ],
)

cc_binary(
  name = 'db_bench_tool',
  srcs = [
    'db_bench_tool.cc',
    'db_bench.cc',
  ],
  incs = ['.', '../', '../include/', '../../../snappy'],
  defs = ['OS_LINUX', 'ROCKSDB_PLATFORM_POSIX', 'ROCKSDB_LIB_IO_POSIX',
          'SNAPPY', 'GFLAGS', 'ROCKSDB_MALLOC_USABLE_SIZE',
          'ROCKSDB_PTHREAD_ADAPTIVE_MUTEX', 'ROCKSDB_BACKTRACE'],
  deps = [
    '//thirdparty/rocksdb/6.1.2:rocksdb-6.1.2',
  ],
)

cc_binary(
  name = 'trace_analyzer_tool',
  srcs = [
    'trace_analyzer_tool.cc',
    'trace_analyzer.cc',
  ],
  incs = ['.', '../', '../include/', '../../../snappy'],
  defs = ['OS_LINUX', 'ROCKSDB_PLATFORM_POSIX', 'ROCKSDB_LIB_IO_POSIX',
          'SNAPPY', 'GFLAGS', 'ROCKSDB_MALLOC_USABLE_SIZE',
          'ROCKSDB_PTHREAD_ADAPTIVE_MUTEX', 'ROCKSDB_BACKTRACE'],
  deps = [
    '//thirdparty/rocksdb/6.1.2:rocksdb-6.1.2',
  ],
)

cc_binary(
  name = 'write_stress',
  srcs = [
    'write_stress.cc',
  ],
  incs = ['.', '../', '../include/', '../../../snappy'],
  defs = ['OS_LINUX', 'ROCKSDB_PLATFORM_POSIX', 'ROCKSDB_LIB_IO_POSIX',
          'SNAPPY', 'GFLAGS', 'ROCKSDB_MALLOC_USABLE_SIZE',
          'ROCKSDB_PTHREAD_ADAPTIVE_MUTEX', 'ROCKSDB_BACKTRACE'],
  deps = [
    '//thirdparty/rocksdb/6.1.2:rocksdb-6.1.2',
  ],
)
