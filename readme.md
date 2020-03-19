## Simple C++ Rendering Engine from scratch for learning purposes. 

#### 
	Clone, submodule init then update.
	Add an enviroment variable "QT5_INSTALL_DIR" points to QT5 install directory (where libs and include of QT5 are, for example - C:\Qt\5.14.1\msvc2017_64).
	Add %QT5_INSTALL_DIR%/bin to Path variable for now

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
- link to QT ✔
- get QT in
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
