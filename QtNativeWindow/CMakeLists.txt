find_package(Qt6 REQUIRED COMPONENTS Widgets ActiveQt)

qt_standard_project_setup()

qt_add_executable(QtNativeWindow
    app.manifest # manifest to unlock Win10 features
    Main.cpp
    LayeredWindow.hpp
    TransparentWindow.hpp
)

target_link_libraries(QtNativeWindow PRIVATE Qt6::Widgets Qt6::ActiveQt)

set_target_properties(QtNativeWindow PROPERTIES
    WIN32_EXECUTABLE ON # hide console window
)

qt_generate_deploy_app_script(
    TARGET QtNativeWindow
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
