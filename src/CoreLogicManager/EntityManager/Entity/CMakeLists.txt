# imgui test executable, with full warnings enabled
project(Entity CXX)

add_library(Entity STATIC  Entity.hpp Entity.cpp)

target_include_directories(Entity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_ROOT})

target_link_libraries(Entity
    PUBLIC  project_warnings 
            project_options 
            CONAN_PKG::docopt.cpp
            CONAN_PKG::fmt
            CONAN_PKG::spdlog
            CONAN_PKG::imgui-sfml

)
