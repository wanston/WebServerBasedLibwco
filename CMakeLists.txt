cmake_minimum_required(VERSION 2.8)

project(WebServerBasedLibwco)


set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "-D_GNU_SOURCE")

link_directories(${CMAKE_SOURCE_DIR}/lib)

include_directories(include include/libwco)


set(SRC main.cpp Response.cpp HttpParser.cpp Server.cpp Worker.cpp)

add_executable(WebServer ${SRC})
target_link_libraries(WebServer pthread wco dl)