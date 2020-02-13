cmake_minimum_required(VERSION 3.17)

#project name
project(rendering_jo)

#c++17
set(CMAKE_CXX_STANDARD 17)

#headers
set(HEADERS
	app/app.h
	engine/Engine.h
	engine/renderers/Phong_Renderer.h
	engine/renderers/Phong_Shadow_Renderer.h
	engine/renderers/PBR_Renderer.h
	engine/renderers/Skybox_Renderer.h
	geometry/vertex.h
	geometry/Mesh.h
	gpu_gl/glgpu.h
	gpu_gl/gl_context.h
	gpu_gl/glew.h
	gpu_gl/wglew.h
	IO/Input.h
	IO/Image.h
	IO/Stb_Image.h
	IO/Stb_Image_Write.h
	IO/STL_Reader.h
	math/Vector.h
	math/Matrix.h
	math/Quaternion.h
	math/Transform.h
	math/Gfx.h
	window/window.h
	world/World.h
	world/Camera.h
	world/3Dobject.h
	Defs/Defs.h
	)

#src
set(SOURCES
	app/app.cpp
	engine/Engine.cpp
	engine/renderers/Phong_Renderer.cpp
	engine/renderers/Phong_Shadow_Renderer.cpp
	engine/renderers/PBR_Renderer.cpp
	engine/renderers/Skybox_Renderer.cpp
	geometry/Mesh.cpp
	gpu_gl/glgpu.cpp
	gpu_gl/gl_context.cpp
	gpu_gl/glew.c
	IO/Input.cpp
	IO/Image.cpp
	IO/STL_Reader.cpp
	window/window.cpp
	world/World.cpp
	world/3Dobject.h
	main/main.cpp
	)

#resources
set(RESOURCES
	engine/shaders/cube.vertex
	engine/shaders/depth.vertex
	engine/shaders/depth.pixel
	engine/shaders/pbr.vertex
	engine/shaders/pbr.pixel
	engine/shaders/equarectangular_to_cubemap.pixel
	engine/shaders/diffuse_irradiance_convolution.pixel
	engine/shaders/specular_BRDF_convolution.pixel
	engine/shaders/specular_prefiltering_convolution.pixel
	engine/shaders/phong.vertex
	engine/shaders/phong.pixel
	engine/shaders/phong_shadow.vertex
	engine/shaders/phong_shadow.pixel
	engine/shaders/quad_ndc.vertex
	engine/shaders/skybox.vertex
	engine/shaders/skybox.pixel
	)

#make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RESOURCES})

#include the source directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
				${HEADERS}
				${SOURCES}
				${RESOURCES})

#link to opengl
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

#windows macro
if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE OS_WINDOWS=1)
endif()