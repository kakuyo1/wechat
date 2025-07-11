# 设置最低 CMake 版本要求
cmake_minimum_required(VERSION 3.12)

# 项目名称
project(StatusServer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Boost 库，包含 system, url, beast
find_package(Boost 1.81 REQUIRED COMPONENTS system url)

# 查找 Protobuf 和 gRPC 和 jsoncpp
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(spdlog REQUIRED)

# 自动收集源文件
file(GLOB SOURCES "source/*.cpp")

# 加入 proto 生成的文件
set(PROTO_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/message.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/message.grpc.pb.cc
)
list(APPEND SOURCES ${PROTO_SRCS})

#config.ini 复制到 build 目录
configure_file(${CMAKE_SOURCE_DIR}/config.ini ${CMAKE_BINARY_DIR}/config.ini COPYONLY)

# 添加可执行文件
add_executable(StatusServer ${SOURCES})


# 添加头文件搜索路径
target_include_directories(StatusServer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/proto
    ${PROJECT_SOURCE_DIR}/source
    ${Boost_INCLUDE_DIRS}
    /usr/include
    /home/kakuyo/libbcrypt/include  # ← 这里加上 bcrypt 头文件目录
)

# 链接 Boost 库
target_link_libraries(StatusServer PRIVATE
    Boost::system
    Boost::url
)

# 链接 gRPC 和 Protobuf
target_link_libraries(StatusServer PRIVATE
    gRPC::grpc++_reflection
    gRPC::grpc++
    protobuf::libprotobuf
    jsoncpp_lib
    hiredis
    mysqlcppconn # 如果使用 MySQL Connector/C++
    bcrypt  # ← 这里加上 bcrypt 库
    spdlog::spdlog
)

# 链接 pthread
find_package(Threads REQUIRED)
target_link_libraries(StatusServer PRIVATE Threads::Threads)