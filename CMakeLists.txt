cmake_minimum_required (VERSION 3.3)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (CG4_Scene  LANGUAGES CXX C)

add_subdirectory ("src")
add_subdirectory ("third_party/glfw")
add_subdirectory ("third_party/glad")
add_subdirectory ("third_party/glm")
add_subdirectory ("third_party/assimp")