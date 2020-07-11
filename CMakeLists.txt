cmake_minimum_required(VERSION 3.17)

#project name
project(rendering_jo)

#c++17
set(CMAKE_CXX_STANDARD 17)

#ui
set(UI_FILES
	src/gui/ui/MainWindow.ui
	)
set(UI_RESOURCES
	src/gui/ui/ui.qrc
	)
set(UI_HEADERS
	src/gui/MainWindow.h
	src/gui/Palette.h
	)
set(UI_SOURCES
	src/gui/MainWindow.cpp
	src/gui/Palette.cpp
	)

#resources
set(RESOURCES
	src/engine/shaders/cube.vertex
	src/engine/shaders/depth.vertex
	src/engine/shaders/depth.pixel
	src/engine/shaders/pbr.vertex
	src/engine/shaders/pbr.pixel
	src/engine/shaders/equarectangular_to_cubemap.pixel
	src/engine/shaders/diffuse_irradiance_convolution.pixel
	src/engine/shaders/specular_BRDF_convolution.pixel
	src/engine/shaders/specular_prefiltering_convolution.pixel
	src/engine/shaders/phong.vertex
	src/engine/shaders/phong.pixel
	src/engine/shaders/phong_shadow.vertex
	src/engine/shaders/phong_shadow.pixel
	src/engine/shaders/quad_ndc.vertex
	src/engine/shaders/colored.vertex
	src/engine/shaders/colored.pixel
	src/engine/shaders/point.vertex
	src/engine/shaders/point.pixel	
	src/engine/shaders/line.vertex
	src/engine/shaders/line.pixel
	src/engine/shaders/edge.vertex
	src/engine/shaders/edge.pixel
	src/engine/shaders/cquad.vertex
	src/engine/shaders/cquad.pixel
	src/engine/shaders/tquad.vertex
	src/engine/shaders/tquad.pixel
	src/engine/shaders/hiddenline.vertex
	src/engine/shaders/hiddenline.geo
	src/engine/shaders/hiddenline.pixel
	src/engine/shaders/skybox.vertex
	src/engine/shaders/skybox.pixel
	src/engine/shaders/postprocessing/color.pixel
	readme.md

	${UI_FILES}
	)

#headers
set(HEADERS
	src/app/Painter.h
	src/engine/Engine.h
	src/engine/renderer/Phong.h
	src/engine/renderer/Phong_Shadow.h
	src/engine/renderer/PBR.h
	src/engine/renderer/Colored.h
	src/engine/renderer/Point.h
	src/engine/renderer/Line.h
	src/engine/renderer/Edge.h
	src/engine/renderer/CQuad.h
	src/engine/renderer/TQuad.h
	src/engine/renderer/Skybox.h
	src/engine/renderer/Hiddenline.h
	src/engine/renderer/Postprocessor.h
	src/engine/renderer/Outlines3D.h
	src/gl/glgpu.h
	src/gl/gl_context.h
	src/gl/glew.h
	src/gl/wglew.h
	src/io/Input.h
	src/io/Image.h
	src/io/Stb_Image.h
	src/io/Stb_Image_Write.h
	src/io/STL_Reader.h
	src/io/Event.h
	src/math/Vector.h
	src/math/Matrix.h
	src/math/Quaternion.h
	src/math/Euler_Angles.h
	src/math/Gfx.h
	src/window/Window.h
	src/world/Universe.h
	src/world/component/Transform.h
	src/world/component/Camera.h
	src/world/component/Vertex.h
	src/world/component/Mesh.h
	src/world/component/Material.h
	src/world/component/Sun.h
	src/world/component/Lamp.h
	src/world/component/Flash.h
	src/world/system/rendering/Phong.h
	src/world/system/rendering/PBR.h
	src/world/system/rendering/Colored.h
	src/world/system/rendering/Skybox.h
	src/world/system/rendering/Point.h
	src/world/system/rendering/Line.h
	src/world/system/rendering/Hiddenline.h
	src/world/system/rendering/Background.h
	src/world/system/rendering/Postprocessor.h
	src/world/system/updating/Camera.h
	src/world/system/updating/Mesh.h
	src/world/system/querying/Pick.h
	src/world/system/scripting/Script.h
	src/infra/mem/chunck.h
	src/infra/managers/Selector.h
	src/defs/Defs.h
	src/imgui/imgui.h
	src/imgui/imconfig.h
	src/imgui/imgui_impl_opengl3.h
	src/imgui/imgui_impl_win32.h
	src/imgui/imgui_internal.h
	src/imgui/imstb_rectpack.h
	src/imgui/imstb_textedit.h
	src/imgui/imstb_truetype.h
	src/utils/Defer.h

	${UI_HEADERS}
	)

#src
set(SOURCES
	src/app/Painter.cpp
	src/engine/Engine.cpp
	src/engine/renderer/Phong.cpp
	src/engine/renderer/Phong_Shadow.cpp
	src/engine/renderer/PBR.cpp
	src/engine/renderer/Colored.cpp
	src/engine/renderer/Point.cpp
	src/engine/renderer/Line.cpp
	src/engine/renderer/Edge.cpp
	src/engine/renderer/CQuad.cpp
	src/engine/renderer/TQuad.cpp
	src/engine/renderer/Skybox.cpp
	src/engine/renderer/Hiddenline.cpp
	src/engine/renderer/Postprocessor.cpp
	src/engine/renderer/Outlines3D.cpp
	src/world/Universe.cpp
	src/world/system/rendering/Phong.cpp
	src/world/system/rendering/PBR.cpp
	src/world/system/rendering/Colored.cpp
	src/world/system/rendering/Skybox.cpp
	src/world/system/rendering/Point.cpp
	src/world/system/rendering/Line.cpp
	src/world/system/rendering/Hiddenline.cpp
	src/world/system/rendering/Background.cpp
	src/world/system/rendering/Postprocessor.cpp
	src/world/system/updating/Camera.cpp
	src/world/system/updating/Mesh.cpp
	src/world/system/querying/Pick.cpp
	src/world/system/scripting/Script.cpp
	src/gl/glgpu.cpp
	src/gl/gl_context.cpp
	src/gl/glew.c
	src/io/Input.cpp
	src/io/Image.cpp
	src/io/STL_Reader.cpp
	src/window/window.cpp
	src/imgui/imgui.cpp
	src/imgui/imgui_demo.cpp
	src/imgui/imgui_draw.cpp
	src/imgui/imgui_impl_opengl3.cpp
	src/imgui/imgui_impl_win32.cpp
	src/imgui/imgui_widgets.cpp
	src/main/main.cpp

	${UI_SOURCES}
	)

#make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RESOURCES})

#include the source directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
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
