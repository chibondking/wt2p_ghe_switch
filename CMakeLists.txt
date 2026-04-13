cmake_minimum_required(VERSION 3.21)

include(cmake/GHEControlVersion.cmake)

project(GHEControl
    VERSION ${GHECONTROL_VERSION}
    DESCRIPTION "Green Heron Engineering Antenna Switch Controller"
    LANGUAGES CXX
)

# ── C++ standard ──────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Qt6 ───────────────────────────────────────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
qt_standard_project_setup()

# ── Sources ───────────────────────────────────────────────────────────────────
set(GHECONTROL_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/config/AppConfig.cpp
    src/config/AppConfig.h
    src/config/ServerConfig.h
    src/core/GheClient.cpp
    src/core/GheClient.h
    src/core/GheParser.cpp
    src/core/GheParser.h
    src/core/AntennaModel.cpp
    src/core/AntennaModel.h
    src/core/SwitchController.cpp
    src/core/SwitchController.h
    src/ui/AntennaButton.cpp
    src/ui/AntennaButton.h
    src/ui/SetupDialog.cpp
    src/ui/SetupDialog.h
)

# ── Executable ────────────────────────────────────────────────────────────────
if(WIN32)
    qt_add_executable(GHEControl WIN32 ${GHECONTROL_SOURCES})
else()
    qt_add_executable(GHEControl ${GHECONTROL_SOURCES})
endif()

# ── Include paths ─────────────────────────────────────────────────────────────
target_include_directories(GHEControl PRIVATE
    src
    src/config
    src/core
    src/ui
)

# ── Link libraries ────────────────────────────────────────────────────────────
target_link_libraries(GHEControl PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)

# ── Compile options ───────────────────────────────────────────────────────────
target_compile_options(GHEControl PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# ── Version definition available to source ────────────────────────────────────
target_compile_definitions(GHEControl PRIVATE
    GHECONTROL_VERSION_STR="${GHECONTROL_VERSION}"
)
