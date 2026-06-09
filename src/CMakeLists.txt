aux_source_directory(. SRC_LIST)
add_library(mymprpc ${SRC_LIST})
target_link_libraries(mymprpc mymuduo zookeeper_mt)