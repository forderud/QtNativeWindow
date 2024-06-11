
add_library(MyOleControl SHARED
    module.def
    Main.cpp
    MfcAppCtrl.cpp
    MfcAppCtrl.h
    Resource.h
    targetver.h
    MyOleControl.rc
    MyOleControl.idl
)

set_property(TARGET MyOleControl PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# The MIDL output files (e.g. MyOleControl_i.c) are stored in the intermediate directory,
# and there is no known way to change it from CMake. This is different than a regular
# VS projects, where the MIDL output files are stored in ${ProjectDir}.
#
# As a work-around, we'll add the intermediate directory as an include path,
# and copy the generated files to ${ProjectDir} post-build.
set(INT_DIR "${CMAKE_CURRENT_BINARY_DIR}/MyOleControl.dir/${CMAKE_CFG_INTDIR}")
target_include_directories(MyOleControl PRIVATE "${INT_DIR}")

# Register DLL post-build
add_custom_command(TARGET MyOleControl POST_BUILD
    COMMAND echo ARGS Registering DLL...
    COMMAND regsvr32 ARGS /s "$<TARGET_FILE_DIR:MyOleControl>\\$<TARGET_FILE_NAME:MyOleControl>"
    COMMENT "Register output.")
