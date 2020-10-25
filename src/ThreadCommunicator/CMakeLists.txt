# imgui test executable, with full warnings enabled
project(ThreadCommunicator CXX)

add_library(ThreadCommunicator INTERFACE)
target_include_directories(ThreadCommunicator INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(RunnableManager  RunnableManager.hpp  project_warnings project_options)
