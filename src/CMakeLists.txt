# CMakeList.txt : CMake project for CUDA-Fishes, assimp source and define
# project specific logic here.
#

# Add source to this project's executable.


set(SOURCE_FILES
		# ImGui files
		../third_party/imgui/imgui_impl_glfw.cpp
		../third_party/imgui/imgui_impl_opengl3.cpp
		../third_party/imgui/imgui_draw.cpp
		../third_party/imgui/imgui_stdlib.cpp
		../third_party/imgui/imgui.cpp
		../third_party/imgui/imgui_demo.cpp
		../third_party/imgui/imgui_tables.cpp
		../third_party/imgui/imgui_widgets.cpp
		# Another files
		../window/window.cpp
		../window/window.h
		../shader/shader.h
		../shader/shader.cpp
		../mesh/mesh.h
		../mesh/mesh.cpp
		../model/model.h
		../model/model.cpp
		../textureLoader/textureLoader.h
		../textureLoader/textureLoader.cpp)

add_executable (CG4_Scene "main.cpp" ${SOURCE_FILES})

target_include_directories(CG4_Scene PRIVATE
		"../third_party/glfw/include"
		"../third_party/glad/include"
		"../third_party/imgui"
		"../third_party/assimp"
		"../third_party/glm"
		"../third_party/headers"
		"../window"
		"../shader"
		"../mesh"
		"../model"
		"../textureLoader")

find_package(OpenGL REQUIRED)

target_link_libraries(CG4_Scene PRIVATE
		glfw glad ${OPENGL_LIBRARIES}
		${PROJECT_SOURCE_DIR}/third_party/assimp/assimp-vc143-mt.lib)

set_property(TARGET CG4_Scene PROPERTY CXX_STANDARD 20)

add_custom_command(TARGET CG4_Scene POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/shadersSources ${PROJECT_BINARY_DIR}/shaders
	COMMENT "Copying shaders")
