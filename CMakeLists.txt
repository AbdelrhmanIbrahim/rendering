cmake_minimum_required(VERSION 3.17)

#project name
project(rendering_jo)

#c++17
set(CMAKE_CXX_STANDARD 17)

#ui
set(UI_FILES
	gui/ui/MainWindow.ui
	)
set(UI_HEADERS
	gui/MainWindow.h
	gui/NativeWindow.h
	)
set(UI_SOURCES
	gui/MainWindow.cpp
	gui/NativeWindow.cpp
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
	readme.md

	${UI_FILES}
	)

#headers
set(HEADERS
	App/App.h
	engine/Engine.h
	engine/renderers/Phong.h
	engine/renderers/Phong_Shadow.h
	engine/renderers/PBR.h
	engine/renderers/Skybox.h
	geometry/Vertex.h
	geometry/Mesh.h
	gL/glgpu.h
	gL/gl_context.h
	gL/glew.h
	gL/wglew.h
	iO/Input.h
	iO/Image.h
	iO/Stb_Image.h
	iO/Stb_Image_Write.h
	iO/STL_Reader.h
	math/Vector.h
	math/Matrix.h
	math/Quaternion.h
	math/Transform.h
	math/Gfx.h
	window/Window.h
	world/World.h
	world/Camera.h
	world/3Dobject.h
	defs/Defs.h
	imGui/imgui.h
	imGui/imconfig.h
	imGui/imgui_impl_opengl3.h
	imGui/imgui_impl_win32.h
	imGui/imgui_internal.h
	imGui/imstb_rectpack.h
	imGui/imstb_textedit.h
	imGui/imstb_truetype.h
	utils/Defer.h

	${UI_HEADERS}
	)

#src
set(SOURCES
	App/App.cpp
	engine/Engine.cpp
	engine/renderers/Phong.cpp
	engine/renderers/Phong_Shadow.cpp
	engine/renderers/PBR.cpp
	engine/renderers/Skybox.cpp
	geometry/Mesh.cpp
	gL/glgpu.cpp
	gL/gl_context.cpp
	gL/glew.c
	iO/Input.cpp
	iO/Image.cpp
	iO/STL_Reader.cpp
	window/window.cpp
	world/World.cpp
	imGui/imgui.cpp
	imGui/imgui_demo.cpp
	imGui/imgui_draw.cpp
	imGui/imgui_impl_opengl3.cpp
	imGui/imgui_impl_win32.cpp
	imGui/imgui_widgets.cpp
	main/main.cpp

	${UI_SOURCES}
	)

#make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RESOURCES})

#include the source directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#add sources and build to the include path to search within (uics/mocs generated in the build dir)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#required packages
find_package(OpenGL REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

#qt
#wrap uis for uic
qt5_wrap_ui(QT5_WRAPPED_UIS ${UI_FILES})

#wrap headers for moc
qt5_wrap_cpp(QT5_WRAPPED_MOCS ${UI_HEADERS})

add_executable(${PROJECT_NAME}
				${HEADERS}
				${SOURCES}
				${RESOURCES}
				${QT5_WRAPPED_UIS}
				${QT5_WRAPPED_MOCS}
				)

#link to external packages
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui)

#link to external submodules
add_subdirectory(external/fmt EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

#macros for project directory path to load shaders from, for now till we have a res file
target_compile_definitions(${PROJECT_NAME} PRIVATE DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

#windows macro
if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE OS_WINDOWS=1)
endif()
