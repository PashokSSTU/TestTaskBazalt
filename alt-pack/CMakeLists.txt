cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 17)
project(alt-package-analyser VERSION 0.1)

add_library(libaltpack SHARED private/pack.cpp)

set_target_properties (libaltpack PROPERTIES
	PUBLIC_HEADER public/pack.hpp
	SOVERSION ${PROJECT_VERSION} 
)

set(CMAKE_INSTALL_INCLUDEDIR "/usr/local/include")
set(CMAKE_INSTALL_LIBDIR "/usr/local/lib")

target_link_libraries(libaltpack curl)

if(NOT EXISTS "${CMAKE_INSTALL_INCLUDEDIR}/alt-pack")
    file(MAKE_DIRECTORY "${CMAKE_INSTALL_INCLUDEDIR}/alt-pack")
endif()

install(TARGETS libaltpack
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/alt-pack/"
)
