# imgui test executable, with full warnings enabled
project(Entity CXX)

add_library(Entity INTERFACE  FixedEntity.hpp Entity.hpp IEntity.hpp)

target_include_directories(Entity INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_ROOT})

target_link_libraries(Entity
    INTERFACE  project_warnings 
            project_options 
            CONAN_PKG::docopt.cpp
            CONAN_PKG::fmt
            CONAN_PKG::spdlog
            CONAN_PKG::imgui-sfml

)
