# evanOS CMake Build Configuration
# Project: evanOS - Enterprise System Monitoring and Maintenance Platform
# Version: 1.0
# Date: 2026-01-02

# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(evanOS VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

# Source files
set(CORE_SOURCES
    src/core/main.cpp
    src/core/system_monitor.cpp
    src/core/configuration.cpp
    src/core/gpu_monitor.cpp
)

set(I18N_SOURCES
    src/i18n/internationalization.cpp
)

set(UI_SOURCES
    src/ui/console_ui.cpp
    src/ui/display_utils.cpp
)

set(UTILS_SOURCES
    src/utils/command_line.cpp
    src/utils/string_utils.cpp
)

# Executable target
add_executable(evanOS
    ${CORE_SOURCES}
    ${I18N_SOURCES}
    ${UI_SOURCES}
    ${UTILS_SOURCES}
)

# Link against Windows libraries if on Windows
if(WIN32)
    target_link_libraries(evanOS PRIVATE
        kernel32
        user32
        gdi32
        winspool
        comdlg32
        advapi32
        shell32
        ole32
        oleaut32
        uuid
        odbc32
        odbccp32
        iphlpapi
        psapi
    )
endif()

# 添加GPU监控库依赖检测
if(WIN32)
    # 尝试查找NVIDIA NVML库
    find_file(NVML_DLL "nvidia-ml.dll" 
        PATHS "$ENV{ProgramFiles}/NVIDIA Corporation/NVSMI" "/Windows/System32" "/Windows/SysWOW64"
    )
    
    if(NVML_DLL)
        add_definitions(-DHAS_NVML_SUPPORT=1)
        message(STATUS "NVIDIA NVML library found - GPU monitoring will be enabled")
    else()
        add_definitions(-DHAS_NVML_SUPPORT=0)
        message(WARNING "NVIDIA NVML library not found - GPU monitoring will be limited")
    endif()
endif()

# Set output directory
set_target_properties(evanOS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# Create bin directory if it doesn't exist
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Installation rules
install(TARGETS evanOS
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Enable testing if BUILD_TESTING is ON
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()