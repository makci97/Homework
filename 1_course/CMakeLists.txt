cmake_minimum_required(VERSION 2.8)

project(student_common_srcs)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake targets")


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  
set(CONFIGURATION "debug")

else()
  
set(CONFIGURATION "release")

endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_CURRENT_SOURCE_DIR}/bin.${CONFIGURATION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/bin.debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin.release")


add_subdirectory(1_semester)

add_subdirectory(2_semester)

