cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 17)
project(branch_cli VERSION 0.1)

set(CMAKE_INSTALL_RPATH "/usr/local/lib")

add_executable(branch_cli main.cpp)

target_link_libraries(branch_cli PRIVATE altpack curl)

install(TARGETS branch_cli
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)