find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUF REQUIRED protobuf)

set(SRC_LIST UserService.cc ../user.pb.cc)

add_executable(provider ${SRC_LIST})

target_include_directories(provider PRIVATE ${PROTOBUF_INCLUDE_DIRS})
target_link_directories(provider PRIVATE ${PROTOBUF_LIBRARY_DIRS})
target_link_libraries(provider mymprpc ${PROTOBUF_LIBRARIES})