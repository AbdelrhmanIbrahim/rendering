cmake_minimum_required(VERSION 3.17)

#project name
project(rendering_jo)

#c++17
set(CMAKE_CXX_STANDARD 17)

#headers
set(HEADERS
	App/App.h
	Engine/Engine.h
	Engine/renderers/Phong.h
	Engine/renderers/Phong_Shadow.h
	Engine/renderers/PBR.h
	Engine/renderers/Skybox.h
	Geometry/Vertex.h
	Geometry/Mesh.h
	GL/glgpu.h
	GL/gl_context.h
	GL/glew.h
	GL/wglew.h
	IO/Input.h
	IO/Image.h
	IO/Stb_Image.h
	IO/Stb_Image_Write.h
	IO/STL_Reader.h
	Math/Vector.h
	Math/Matrix.h
	Math/Quaternion.h
	Math/Transform.h
	Math/Gfx.h
	Window/Window.h
	World/World.h
	World/Camera.h
	World/3Dobject.h
	Defs/Defs.h
	ImGui/imgui.h
	ImGui/imconfig.h
	ImGui/imgui_impl_opengl3.h
	ImGui/imgui_impl_win32.h
	ImGui/imgui_internal.h
	ImGui/imstb_rectpack.h
	ImGui/imstb_textedit.h
	ImGui/imstb_truetype.h
	)

#src
set(SOURCES
	App/App.cpp
	Engine/Engine.cpp
	Engine/renderers/Phong.cpp
	Engine/renderers/Phong_Shadow.cpp
	Engine/renderers/PBR.cpp
	Engine/renderers/Skybox.cpp
	Geometry/Mesh.cpp
	GL/glgpu.cpp
	GL/gl_context.cpp
	GL/glew.c
	IO/Input.cpp
	IO/Image.cpp
	IO/STL_Reader.cpp
	Window/window.cpp
	World/World.cpp
	ImGui/imgui.cpp
	ImGui/imgui_demo.cpp
	ImGui/imgui_draw.cpp
	ImGui/imgui_impl_opengl3.cpp
	ImGui/imgui_impl_win32.cpp
	ImGui/imgui_widgets.cpp
	main/main.cpp
	)

#resources
set(RESOURCES
	Engine/shaders/cube.vertex
	Engine/shaders/depth.vertex
	Engine/shaders/depth.pixel
	Engine/shaders/pbr.vertex
	Engine/shaders/pbr.pixel
	Engine/shaders/equarectangular_to_cubemap.pixel
	Engine/shaders/diffuse_irradiance_convolution.pixel
	Engine/shaders/specular_BRDF_convolution.pixel
	Engine/shaders/specular_prefiltering_convolution.pixel
	Engine/shaders/phong.vertex
	Engine/shaders/phong.pixel
	Engine/shaders/phong_shadow.vertex
	Engine/shaders/phong_shadow.pixel
	Engine/shaders/quad_ndc.vertex
	Engine/shaders/skybox.vertex
	Engine/shaders/skybox.pixel
	readme.md
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

#macros for project directory path to load shaders from, for now till we have a res file
target_compile_definitions(${PROJECT_NAME} PRIVATE DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

#windows macro
if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE OS_WINDOWS=1)
endif()