cmake_minimum_required(VERSION 4.0)


project(mini-jam)

add_subdirectory(vendor/raylib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC 
	src/main.cpp
	src/Global.cpp
    src/Rhythm.cpp
    src/Level.cpp
	src/TextureAtlas.cpp
    src/SFXAtlas.cpp
    src/Rhythm.cpp
    src/Scoring.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/raygui/src)
