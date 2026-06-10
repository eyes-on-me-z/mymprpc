find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUF REQUIRED protobuf)

set(USER_SRC_LIST UserService.cc ../user.pb.cc)
set(FRIEND_SRC_LIST FriendService.cc ../friend.pb.cc)

add_executable(user_provider ${USER_SRC_LIST})
add_executable(friend_provider ${FRIEND_SRC_LIST})

target_include_directories(user_provider PRIVATE ${PROTOBUF_INCLUDE_DIRS})
target_include_directories(friend_provider PRIVATE ${PROTOBUF_INCLUDE_DIRS})

target_link_directories(user_provider PRIVATE ${PROTOBUF_LIBRARY_DIRS})
target_link_directories(friend_provider PRIVATE ${PROTOBUF_LIBRARY_DIRS})

target_link_libraries(user_provider mymprpc ${PROTOBUF_LIBRARIES})
target_link_libraries(friend_provider mymprpc ${PROTOBUF_LIBRARIES})