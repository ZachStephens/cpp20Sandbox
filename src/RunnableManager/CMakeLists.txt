# imgui test executable, with full warnings enabled
project(RunnableManager CXX)

add_library(RunnableManager STATIC RunnableManager.cpp RunnableManager.hpp)
target_include_directories(RunnableManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
#target_link_libraries(RunnableManager  RunnableManager.hpp  project_warnings project_options)
