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
		../drawable/mesh/mesh.h
		../drawable/mesh/mesh.cpp
		../drawable/model/model.h
		../drawable/model/model.cpp
		../textureLoader/textureLoader.h
		../textureLoader/textureLoader.cpp
		../scene/scene.h
		../drawable/drawable.h
		../scene/scene.cpp
		../third_party/headers/stb_image.h
		../drawable/drawable.cpp
		../camera/camera.h
		../camera/camera.cpp)

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
		"../drawable"
		"../drawable/mesh"
		"../drawable/model"
		"../textureLoader"
		"../camera")

find_package(OpenGL REQUIRED)

target_link_libraries(CG4_Scene PRIVATE
		glfw glad ${OPENGL_LIBRARIES}
		assimp::assimp)

set_property(TARGET CG4_Scene PROPERTY CXX_STANDARD 20)

add_custom_command(TARGET CG4_Scene POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/shadersSources ${PROJECT_BINARY_DIR}/shaders
	COMMENT "Copying shaders")

add_custom_command(TARGET CG4_Scene POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/models ${PROJECT_BINARY_DIR}/models
	COMMENT "Copying models")
