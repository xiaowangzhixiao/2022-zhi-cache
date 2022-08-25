include('//thirdparty/foreign_build.bld')

cmake_build(
    name = 'leveldb_build',
    source_package = 'leveldb-1.23.tar.gz',
    package_name = 'leveldb',
    source_dir ='leveldb-1.23',
    lib_names = ['leveldb'],
    cmake_options=[
        '-DCMAKE_BUILD_TYPE=Release',
    ],
    deps=[
        '//thirdparty/googletest:gtest',
        '//thirdparty/googletest:gmock',
        '//thirdparty/benchmark:benchmark',
    ],
    strip_include_prefix='leveldb',
)

foreign_cc_library(
    name = 'leveldb',
    deps = [
      ':leveldb_build',
    ],
    visibility = 'PUBLIC',
)
