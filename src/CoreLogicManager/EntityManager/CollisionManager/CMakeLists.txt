# imgui test executable, with full warnings enabled
project(CollisionManager CXX)

add_library(CollisionManager INTERFACE  CollisionManager.hpp)

target_include_directories(CollisionManager INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_ROOT})

target_link_libraries(CollisionManager
    INTERFACE  project_warnings 
            project_options 
            CONAN_PKG::fmt
            CONAN_PKG::spdlog
            CONAN_PKG::imgui-sfml

)
