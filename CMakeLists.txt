cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(SAVANNAH_IMGUI_DEV "Dear ImGui development library" ON)

# set the project name
project(savannah)

# global strings
set(SAVANNAH_BASE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
message("set SAVANNAH_BASE_SOURCE_DIR to ${SAVANNAH_BASE_SOURCE_DIR}")

set(SAVANNAH_EXAMPLES_DIR 
    ${SAVANNAH_BASE_SOURCE_DIR}/examples)

set(SAVANNAH_INCLUDES_DIR 
    ${SAVANNAH_BASE_SOURCE_DIR}/include
    ${SAVANNAH_BASE_SOURCE_DIR}/external
    ${SAVANNAH_BASE_SOURCE_DIR}/external/imgui
    ${SAVANNAH_BASE_SOURCE_DIR}/external/implot
    ${SAVANNAH_BASE_SOURCE_DIR}/external/glad/include
    ${SAVANNAH_BASE_SOURCE_DIR}/
    )
include_directories(${SAVANNAH_INCLUDES_DIR})

# build SAVANNAH as a static library
file(GLOB_RECURSE SAVANNAH_SOURCES 
    ${SAVANNAH_BASE_SOURCE_DIR}/src/*.cpp
    ${SAVANNAH_BASE_SOURCE_DIR}/external/implot/implot.cpp
#    ${SAVANNAH_BASE_SOURCE_DIR}/external/implot/implot_demo.cpp
    ${SAVANNAH_BASE_SOURCE_DIR}/external/implot/implot_internal.cpp
    ${SAVANNAH_BASE_SOURCE_DIR}/external/implot/implot_items.cpp
    ${SAVANNAH_BASE_SOURCE_DIR}/external/stb_image.cpp
    ${SAVANNAH_BASE_SOURCE_DIR}/external/imgui_filedialog/src/imgui_filedialog.cpp
    )
file(GLOB_RECURSE EXTERNAL_SOURCES 
    ${SAVANNAH_BASE_SOURCE_DIR}/external/*.cpp
    ${SAVANNAH_BASE_SOURCE_DIR}/external/*.c)

add_library(${PROJECT_NAME} STATIC 
    ${SAVANNAH_SOURCES} 
    )
target_include_directories(${PROJECT_NAME} PUBLIC
    ${SAVANNAH_INCLUDES_DIR}
    ${SAVANNAH_BASE_SOURCE_DIR}/external
    ${SAVANNAH_BASE_SOURCE_DIR}/external/imgui
    ${SAVANNAH_BASE_SOURCE_DIR}/external/implot
    ${SAVANNAH_BASE_SOURCE_DIR}/external/glad/include
    ${SAVANNAH_BASE_SOURCE_DIR}/
    )
target_compile_options(${PROJECT_NAME} PRIVATE 
	-Wall
	-Werror
	-Wfatal-errors
	-DSAVANNAH_LOGGER_CLASS
	)
	
# link external libraries
add_library(glfw3 STATIC IMPORTED)
set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION ${SAVANNAH_BASE_SOURCE_DIR}/bin/linux/libglfw3.a)
target_include_directories(glfw3 INTERFACE ${SAVANNAH_BASE_SOURCE_DIR}/external/glfw)

add_library(glad STATIC IMPORTED)
set_target_properties(glad PROPERTIES IMPORTED_LOCATION ${SAVANNAH_BASE_SOURCE_DIR}/bin/linux/libglad.a)
target_include_directories(glad INTERFACE ${SAVANNAH_BASE_SOURCE_DIR}/external/glad/include)

add_library(dear-imgui STATIC IMPORTED)
if(SAVANNAH_IMGUI_DEV)
	set_target_properties(dear-imgui PROPERTIES IMPORTED_LOCATION ${SAVANNAH_BASE_SOURCE_DIR}/bin/linux/libimgui-opengl-dev.a)
else()
	set_target_properties(dear-imgui PROPERTIES IMPORTED_LOCATION ${SAVANNAH_BASE_SOURCE_DIR}/bin/linux/libimgui-opengl.a)
endif()
target_include_directories(dear-imgui INTERFACE ${SAVANNAH_BASE_SOURCE_DIR}/external/imgui)

add_library(yaml-cpp STATIC IMPORTED)
set_target_properties(yaml-cpp PROPERTIES IMPORTED_LOCATION ${SAVANNAH_BASE_SOURCE_DIR}/bin/linux/libyaml-cpp.a)
target_include_directories(yaml-cpp INTERFACE ${SAVANNAH_BASE_SOURCE_DIR}/external/yaml_cpp/include)

target_link_libraries(${PROJECT_NAME} PUBLIC 
	glfw3
	glad
	dear-imgui
    yaml-cpp
    )
# target_link_libraries(${PROJECT_NAME} PRIVATE -lpthread "-Wl,-rpath,.")

install(TARGETS ${PROJECT_NAME} 
	RUNTIME DESTINATION ${SAVANNAH_BASE_SOURCE_DIR}/release
	LIBRARY DESTINATION ${SAVANNAH_BASE_SOURCE_DIR}/release
	ARCHIVE DESTINATION ${SAVANNAH_BASE_SOURCE_DIR}/release
	)

include(${SAVANNAH_BASE_SOURCE_DIR}/cmake/build-script-linux.cmake)

# build examples
add_subdirectory(${SAVANNAH_BASE_SOURCE_DIR}/examples)
