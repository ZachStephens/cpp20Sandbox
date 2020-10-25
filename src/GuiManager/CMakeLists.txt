# imgui test executable, with full warnings enabled
project(GuiManager CXX)

add_library(GuiManager STATIC GuiManager.cpp GuiManager.hpp )
# include_directories()
target_include_directories(GuiManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(GuiManager PUBLIC project_warnings project_options CONAN_PKG::imgui-sfml RunnableManager)
