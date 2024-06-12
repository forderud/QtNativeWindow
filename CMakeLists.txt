cmake_minimum_required(VERSION 3.16)

project(QtNativeWindow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable unicode for all projects
add_definitions(-DUNICODE -D_UNICODE)

# Output DLL, LIB & PDB files to out/$(Configuration)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}")

file(TOUCH ${CMAKE_BINARY_DIR}/MfcOleControl.dll.manifest) # placeholder file until MfcOleControl is built

include_external_msproject(MfcOleControl ${CMAKE_CURRENT_SOURCE_DIR}/MfcOleControl/MfcOleControl.vcxproj PLATFORM x64)
add_subdirectory(QtNativeWindow)

# build MfcOleControl before QtNativeWindow
add_dependencies(QtNativeWindow MfcOleControl)

# make QtNativeWindow default project in VS solution
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT QtNativeWindow)
