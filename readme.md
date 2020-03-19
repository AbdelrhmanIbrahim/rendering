## Simple C++ Rendering Engine from scratch for learning purposes. 

#### 
	- Clone, submodule init then update.
	- Add QT5_DIR as an enviroment variable points to the directory that has Qt5Config.cmake, for example : "C:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5"
	- Add QT5 bin folder path to "Path" enviroment variable

### TODO :
- remove GLUT ✔
- remove prebuilt glew binaries ✔
- cmake ✔
- renderdoc ✔
- fix pbr new bug after refactor, diff roughness ✔
- hide renderers impl inside and clean the interface ✔
- get samplers in ✔
- create internal handle for gl objects ✔
- propagate samplers in rendereres ✔
- fix rotation + movement events bug ✔
- add imGui and interact ✔
- add fmt ✔
- get QT in ✔
- blend QT windowing with our created naive window
- small profiling shows swapbuffer waiting take much time, read more about this and vsync
- add ECS
- lazy main loop update for input events and comparing ecs worlds in case of changing data internally
- make a shadow renderer only that outputs a texture with shadows then merge it later
- propagate uniform blocks to phong_shadow
- if we don't need exceptions, disable them
- fix imgui input handling bug
- deferred shading
- realtime GI (global illumination) 
- ambient occlusion
- main loop indpendent frame rate
- compute shaders
- space partitioning 
- view frustm culling
- load textured models, check assimp impl
- resources file format
- selection and picking
- terrian deformation
- fluid/water/particle system simulation (*read Fluid Engine Development book*)
- cloud rendering
- rigid body dynamics
- mesh destruction
- animation 
