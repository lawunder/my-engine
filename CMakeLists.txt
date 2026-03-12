cmake_minimum_required(VERSION 3.20)
project(MyEngine CXX)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)


# Explicitly list source files instead of GLOB
add_executable(MyEngine
    src/main.cpp
    src/game.cpp
)

# Include directories
target_include_directories(MyEngine PRIVATE 
    src/
)

# Link SDL3
target_link_libraries(MyEngine PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

foreach(DLL
    SDL3.dll
    SDL3_ttf.dll
    libstdc++-6.dll
    libgcc_s_seh-1.dll
    libwinpthread-1.dll
    libintl-8.dll
    libiconv-2.dll
    libfreetype-6.dll
    libharfbuzz-0.dll
    libbrotlidec.dll
    libbz2-1.dll
    zlib1.dll
    libpng16-16.dll
    libbrotlicommon.dll
    libgraphite2.dll
    libglib-2.0-0.dll
    libpcre2-8-0.dll
)
    add_custom_command(TARGET MyEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_newer
        "${CMAKE_SOURCE_DIR}/dlls/${DLL}"
        $<TARGET_FILE_DIR:MyEngine>
    )
endforeach()