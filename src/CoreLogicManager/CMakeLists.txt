# imgui test executable, with full warnings enabled
project(CoreLogicManager CXX)

add_subdirectory(EntityManager)

add_library(CoreLogicManager STATIC  CoreLogicManager.hpp CoreLogicManager.cpp)

target_include_directories(CoreLogicManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(CoreLogicManager 
    PUBLIC  EntityManager
            project_warnings 
            project_options 
            CONAN_PKG::docopt.cpp
            CONAN_PKG::fmt
            CONAN_PKG::spdlog
            CONAN_PKG::imgui-sfml
)
