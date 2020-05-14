cmake_minimum_required(VERSION 3.17)

#project name
project(rendering_jo)

#c++17
set(CMAKE_CXX_STANDARD 17)

#ui
set(UI_FILES
	gui/ui/MainWindow.ui
	)
set(UI_RESOURCES
	gui/ui/ui.qrc
	)
set(UI_HEADERS
	gui/MainWindow.h
	gui/Palette.h
	)
set(UI_SOURCES
	gui/MainWindow.cpp
	gui/Palette.cpp
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
	engine/shaders/colored.vertex
	engine/shaders/colored.pixel
	engine/shaders/point.vertex
	engine/shaders/point.pixel	
	engine/shaders/line.vertex
	engine/shaders/line.pixel
	engine/shaders/edge.vertex
	engine/shaders/edge.pixel
	engine/shaders/cquad.vertex
	engine/shaders/cquad.pixel
	engine/shaders/hiddenline.vertex
	engine/shaders/hiddenline.geo
	engine/shaders/hiddenline.pixel
	engine/shaders/skybox.vertex
	engine/shaders/skybox.pixel
	readme.md

	${UI_FILES}
	)

#headers
set(HEADERS
	app/Painter.h
	engine/Engine.h
	engine/renderer/Phong.h
	engine/renderer/Phong_Shadow.h
	engine/renderer/PBR.h
	engine/renderer/Colored.h
	engine/renderer/Point.h
	engine/renderer/Line.h
	engine/renderer/Edge.h
	engine/renderer/Quad.h
	engine/renderer/Skybox.h
	engine/renderer/Hiddenline.h
	gl/glgpu.h
	gl/gl_context.h
	gl/glew.h
	gl/wglew.h
	io/Input.h
	io/Image.h
	io/Stb_Image.h
	io/Stb_Image_Write.h
	io/STL_Reader.h
	io/Event.h
	math/Vector.h
	math/Matrix.h
	math/Quaternion.h
	math/Euler_Angles.h
	math/Gfx.h
	window/Window.h
	world/Universe.h
	world/component/Transform.h
	world/component/Camera.h
	world/component/Vertex.h
	world/component/Mesh.h
	world/component/Material.h
	world/component/Sun.h
	world/component/Lamp.h
	world/component/Flash.h
	world/system/rendering/Phong.h
	world/system/rendering/PBR.h
	world/system/rendering/Colored.h
	world/system/rendering/Skybox.h
	world/system/rendering/Point.h
	world/system/rendering/Line.h
	world/system/rendering/Hiddenline.h
	world/system/rendering/Background.h
	world/system/updating/Camera.h
	world/system/updating/Mesh.h
	world/system/querying/Pick.h
	world/system/scripting/Script.h
	infra/mem/chunck.h
	infra/managers/Selector.h
	defs/Defs.h
	imgui/imgui.h
	imgui/imconfig.h
	imgui/imgui_impl_opengl3.h
	imgui/imgui_impl_win32.h
	imgui/imgui_internal.h
	imgui/imstb_rectpack.h
	imgui/imstb_textedit.h
	imgui/imstb_truetype.h
	utils/Defer.h

	${UI_HEADERS}
	)

#src
set(SOURCES
	app/Painter.cpp
	engine/Engine.cpp
	engine/renderer/Phong.cpp
	engine/renderer/Phong_Shadow.cpp
	engine/renderer/PBR.cpp
	engine/renderer/Colored.cpp
	engine/renderer/Point.cpp
	engine/renderer/Line.cpp
	engine/renderer/Edge.cpp
	engine/renderer/Quad.cpp
	engine/renderer/Skybox.cpp
	engine/renderer/Hiddenline.cpp
	world/Universe.cpp
	world/system/rendering/Phong.cpp
	world/system/rendering/PBR.cpp
	world/system/rendering/Colored.cpp
	world/system/rendering/Skybox.cpp
	world/system/rendering/Point.cpp
	world/system/rendering/Line.cpp
	world/system/rendering/Hiddenline.cpp
	world/system/rendering/Background.cpp
	world/system/updating/Camera.cpp
	world/system/updating/Mesh.cpp
	world/system/querying/Pick.cpp
	world/system/scripting/Script.cpp
	gl/glgpu.cpp
	gl/gl_context.cpp
	gl/glew.c
	io/Input.cpp
	io/Image.cpp
	io/STL_Reader.cpp
	window/window.cpp
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_impl_opengl3.cpp
	imgui/imgui_impl_win32.cpp
	imgui/imgui_widgets.cpp
	main/main.cpp

	${UI_SOURCES}
	)

#make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RESOURCES})

#include the source directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/ecs)

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

#wrap resources for qrc 
qt5_add_resources(QT5_RCC_SOURCES ${UI_RESOURCES})

add_executable(${PROJECT_NAME}
				${HEADERS}
				${SOURCES}
				${RESOURCES}
				${QT5_WRAPPED_UIS}
				${QT5_WRAPPED_MOCS}
				${QT5_RCC_SOURCES}
				)

#link to external packages
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui)

#link to external submodules
add_subdirectory(external/ecs)
add_subdirectory(external/fmt EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

#macros for project directory path to load shaders from, for now till we have a res file
target_compile_definitions(${PROJECT_NAME} PRIVATE DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

#windows macro
if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE OS_WINDOWS=1)
endif()
