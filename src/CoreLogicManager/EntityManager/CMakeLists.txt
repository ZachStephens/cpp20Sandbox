# imgui test executable, with full warnings enabled
project(EntityManager CXX)

add_library(EntityManager STATIC  EntityManager.hpp EntityManager.cpp)

target_include_directories(EntityManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_ROOT})

target_link_libraries(EntityManager
    PUBLIC  project_warnings 
            project_options 
            CONAN_PKG::docopt.cpp
            CONAN_PKG::fmt
            CONAN_PKG::spdlog
            CONAN_PKG::imgui-sfml

)
