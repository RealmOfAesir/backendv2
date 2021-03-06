cmake_minimum_required(VERSION 3.12)
project(roa_backend)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE CXX STANDARD REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#SET(CMAKE_CXX_COMPILER /usr/local/gcc-9.1/bin/g++-9.1)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(TOP_DIR ${CMAKE_SOURCE_DIR})
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# only support linux/g++ for now
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DXXH_INLINE_ALL -DXXH_CPU_LITTLE_ENDIAN=1 -DRAPIDJSON_SSE42 -DSPDLOG_COMPILED_LIB -DCATCH_CONFIG_FAST_COMPILE -DSPDLOG_NO_EXCEPTIONS -DASIO_STANDALONE -Wall -Wextra -Wno-unused-variable -Wno-long-long -Wno-unused-parameter -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -pedantic -std=c++17 ") #-fsanitize=undefined -fsanitize=thread -fstack-protector-strong -fno-omit-frame-pointer ")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb -mavx")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -mavx")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -ggdb -Og -mavx")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -mavx")

file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB SPDLOG_SOURCE ${EXTERNAL_DIR}/spdlog/src/spdlog.cpp)
add_executable(roa_backend ${SPDLOG_SOURCE} ${PROJECT_SOURCES})

file(GLOB_RECURSE PROJECT_SOURCES_WITHOUT_MAIN ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM PROJECT_SOURCES_WITHOUT_MAIN "${PROJECT_SOURCE_DIR}/src/main.cpp")
file(GLOB_RECURSE PROJECT_TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(roa_test ${SPDLOG_SOURCE} ${PROJECT_SOURCES_WITHOUT_MAIN} ${PROJECT_TEST_SOURCES})
add_test(NAME MyTest COMMAND roa_test)

file(GLOB_RECURSE PROJECT_SOURCES_WITHOUT_MAIN ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM PROJECT_SOURCES_WITHOUT_MAIN "${PROJECT_SOURCE_DIR}/src/main.cpp")
file(GLOB PROJECT_BENCHMARK_SOURCES ${PROJECT_SOURCE_DIR}/benchmark/*.cpp ${PROJECT_SOURCE_DIR}/benchmark/benchmark_helpers/*.cpp)
add_executable(roa_benchmark ${SPDLOG_SOURCE} ${PROJECT_SOURCES_WITHOUT_MAIN} ${PROJECT_BENCHMARK_SOURCES})

target_compile_definitions(roa_test PRIVATE TEST_CODE=1)

find_library(PQXX_LIBRARY pqxx HINTS ${EXTERNAL_DIR}/libpqxx/src/.libs)
find_library(SODIUM_LIBRARY sodium HINTS ${EXTERNAL_DIR}/libsodium/src/libsodium/.libs)
find_library(XXHASH_LIBRARY xxhash HINTS ${EXTERNAL_DIR}/xxHash)

if(NOT PQXX_LIBRARY)
    message(FATAL_ERROR "libpqxx not found")
endif()

if(NOT SODIUM_LIBRARY)
    message(FATAL_ERROR "libsodium not found")
endif()

if(NOT XXHASH_LIBRARY)
    message(FATAL_ERROR "libxxhash not found")
endif()

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories("${TOP_DIR}/src")
include_directories("${EXTERNAL_DIR}/spdlog/include")
include_directories("${EXTERNAL_DIR}/rapidjson/include")
include_directories("${EXTERNAL_DIR}/websocketpp")
include_directories("${EXTERNAL_DIR}/asio/asio/include")
include_directories("${EXTERNAL_DIR}/libpqxx/include")
include_directories("${EXTERNAL_DIR}/range-v3/include")
include_directories("${EXTERNAL_DIR}/Catch2/single_include")
include_directories("${ZLIB_INCLUDE_DIRS}")
include_directories("${EXTERNAL_DIR}/libsodium/src/libsodium/include")
include_directories("${EXTERNAL_DIR}/xxHash")
include_directories("${EXTERNAL_DIR}/robin-hood-hashing/src/include")
include_directories("${EXTERNAL_DIR}/entt/src")
include_directories("${EXTERNAL_DIR}/pcg-cpp/include")
include_directories("${EXTERNAL_DIR}/concurrentqueue")


target_link_libraries(roa_backend PUBLIC ${PQXX_LIBRARY} -lpq)
target_link_libraries(roa_backend PUBLIC -lpthread -lstdc++fs)
target_link_libraries(roa_backend PUBLIC ${ZLIB_LIBRARIES} )
target_link_libraries(roa_backend PUBLIC ${OPENSSL_LIBRARIES})
target_link_libraries(roa_backend PUBLIC ${SODIUM_LIBRARY})
target_link_libraries(roa_backend PUBLIC ${XXHASH_LIBRARY})
target_link_libraries(roa_backend PUBLIC -static-libgcc -static-libstdc++)

target_link_libraries(roa_test PUBLIC ${PQXX_LIBRARY} -lpq)
target_link_libraries(roa_test PUBLIC -lpthread -lstdc++fs)
target_link_libraries(roa_test PUBLIC ${ZLIB_LIBRARIES} )
target_link_libraries(roa_test PUBLIC ${OPENSSL_LIBRARIES})
target_link_libraries(roa_test PUBLIC ${SODIUM_LIBRARY})
target_link_libraries(roa_test PUBLIC ${XXHASH_LIBRARY})
target_link_libraries(roa_test PUBLIC -static-libgcc -static-libstdc++)

target_link_libraries(roa_benchmark PUBLIC ${PQXX_LIBRARY} -lpq)
target_link_libraries(roa_benchmark PUBLIC -lpthread -lstdc++fs)
target_link_libraries(roa_benchmark PUBLIC ${ZLIB_LIBRARIES} )
target_link_libraries(roa_benchmark PUBLIC ${OPENSSL_LIBRARIES})
target_link_libraries(roa_benchmark PUBLIC ${SODIUM_LIBRARY})
target_link_libraries(roa_benchmark PUBLIC ${XXHASH_LIBRARY})
target_link_libraries(roa_benchmark PUBLIC -static-libgcc -static-libstdc++)


add_custom_command(
        TARGET roa_backend POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/
        ${CMAKE_BINARY_DIR}/assets)

add_custom_command(
        TARGET roa_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/
        ${CMAKE_BINARY_DIR}/assets)
