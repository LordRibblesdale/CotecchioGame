cmake_minimum_required(VERSION 3.15)
project(CotecchioGame)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE VectorFolder "Vector/*.cpp" "Vector/*.h")
file(GLOB_RECURSE MatrixFolder "Matrix/*.cpp" "Matrix/*.h")
file(GLOB_RECURSE UtilitiesFolder "Utilities/*.cpp" "Utilities/*.h")
file(GLOB_RECURSE GraphicsFolder "Graphics/*.h" "Graphics/*.cpp")
file(GLOB_RECURSE ModelFolder "Model/*.cpp" "Model/*.h")
file(GLOB_RECURSE PlayerFolder "Player/*.cpp" "Player/*.h")
file(GLOB_RECURSE AnimationFolder "Animation/*.cpp" "Animation/*.h")
file(GLOB_RECURSE ExceptionFolder "Exception/*.h")
file(GLOB_RECURSE XMLFolder "IO/rapidxml-1.13/*.hpp")
file(GLOB_RECURSE MainFile "Main.cpp")

file(GLOB_RECURSE GladLibraries "Graphics/glad/include/*.h" "Graphics/glad/src/*.c")
file(GLOB_RECURSE GLFWLibraries "Graphics/glfw-3.3.2.bin.WIN64/include/GLFW/*.h")

add_executable(${PROJECT_NAME}
        ${VectorFolder}
        ${MatrixFolder}
        ${ExceptionFolder}
        ${UtilitiesFolder}
        ${GraphicsFolder}
        ${ModelFolder}
        ${AnimationFolder}
        ${GladLibraries}
        ${GLFWLibraries}
        ${XMLFolder}
        ${MainFile})

target_include_directories(${PROJECT_NAME} PRIVATE "Graphics/glad/include/glad")
target_include_directories(${PROJECT_NAME} PRIVATE "Graphics/glad/include/KHR")
target_include_directories(${PROJECT_NAME} PRIVATE "IO/rapidxml-1.13")

if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE "Graphics/glfw-3.3.2.bin.WIN64/include/GLFW")

    if (MSVC)
        target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/Graphics/glfw-3.3.2.bin.WIN64/lib-vc2019/libglfw3.a")
    else ()
        target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/Graphics/glfw-3.3.2.bin.WIN64/lib-mingw-w64/libglfw3.a")
    endif()

    #target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/Graphics/freetype/libfreetype.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/Graphics/assimp/libassimp.dll.a")
endif()

if(UNIX)
    target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} "/usr/lib/x86_64-linux-gnu/libdl.so")
endif()

