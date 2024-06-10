cmake_minimum_required(VERSION 3.16)

project(QtForeignWindow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets ActiveQt)

qt_standard_project_setup()

qt_add_executable(QtForeignWindow
    app.manifest # manifest to unlock Win10 features
    Main.cpp
    LayeredWindow.hpp
    TransparentWindow.hpp
)

target_link_libraries(QtForeignWindow PRIVATE Qt6::Widgets Qt6::ActiveQt)

set_target_properties(QtForeignWindow PROPERTIES
    WIN32_EXECUTABLE ON # hide console window
)

qt_generate_deploy_app_script(
    TARGET QtForeignWindow
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
