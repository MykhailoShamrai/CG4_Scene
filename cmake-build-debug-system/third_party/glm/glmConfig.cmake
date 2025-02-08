set(GLM_VERSION "0.9.8")
set(GLM_INCLUDE_DIRS "C:/Users/Mike Shamrai/CLionProjects/CG4_Scene/third_party/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
