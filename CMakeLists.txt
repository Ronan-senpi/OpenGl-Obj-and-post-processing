cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_STANDARD 20)

project(Phong)

add_executable(Phong vendor/glad/src/glad.c src/main.cpp src/Shader.cpp src/Shader.h src/helpers/FileHelper.h src/helpers/FileHelper.cpp src/Dragon.h src/Mesh.cpp src/Mesh.h src/Application.cpp src/Application.h src/Camera.cpp src/Camera.h)

target_include_directories(Phong PRIVATE
        "vendor/glad/include"
		"vendor/glfw-3.3.2/include"
		"vendor/glm-0.9.9.8")

target_link_directories(Phong PRIVATE
		"vendor/glfw-3.3.2/lib")

target_link_libraries(Phong PRIVATE
		glfw3)

add_custom_command(TARGET Phong PRE_BUILD
		COMMAND robocopy ${PROJECT_SOURCE_DIR}/resources/ ${PROJECT_BINARY_DIR}/resources /E /NS /NC /NFL /NDL /NP /NJH /NJS  || cmd /c "exit /b 0")