# CMakeList.txt : CMake project for CUDA-Fishes, include source and define
# project specific logic here.
#

# Add source to this project's executable.


set(IMGUI_SOURCE_FILES	../third_party/imgui/imgui_impl_glfw.cpp
		../third_party/imgui/imgui_impl_opengl3.cpp
		../third_party/imgui/imgui_draw.cpp
		../third_party/imgui/imgui_stdlib.cpp
		../third_party/imgui/imgui.cpp
		../third_party/imgui/imgui_demo.cpp
		../third_party/imgui/imgui_tables.cpp
		../third_party/imgui/imgui_widgets.cpp
		../window/window.cpp
		../window/window.h
		../shader/shader.h
		../shader/shader.cpp)

add_executable (CG4_Scene "main.cpp"
		${IMGUI_SOURCE_FILES}
		"../window/window.cpp")

target_include_directories(CG4_Scene PRIVATE
		"../third_party/glfw/include"
		"../third_party/glad/include"
		"../third_party/imgui"
		"../third_party/glm"
		"../window"
		"../shader")

find_package(OpenGL REQUIRED)

target_link_libraries(CG4_Scene PRIVATE
		glfw glad ${OPENGL_LIBRARIES} ${CMAKE_CUDA_LIBRARIES})

set_property(TARGET CG4_Scene PROPERTY CXX_STANDARD 20)

add_custom_command(TARGET CG4_Scene POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/shadersSources ${PROJECT_BINARY_DIR}/shaders
	COMMENT "Copying shaders")

