## Simple C++ Rendering Engine from scratch for learning purposes. (windows/opengl based only for now) 

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
- blend QT windowing with painter ✔
- add ECS ✔
- fix crash at closing ✔
- get the rest of renderers back to use ECS ✔
- point light ✔
- spot light ✔
- attach spot light to camera ✔
- add systems ✔
- move all renderers to renderering systems ✔
- remove unnecessary data transformations, no need to link renderers to ecs ✔
- find a good way for communications and relations between components (unity scripts) ✔
- add camera sys ✔
- propagate different lights to pbr ✔
- add light entities ✔
- point renderer ✔
- fix point renderer bug (the points show in rdoc only which is weird) ✔
- render light source as shinny point with radius ✔
- fix zooming bug ✔
- line renderer ✔
- virtual tiles ground ✔
- hiddenline mode ✔
- emit line strip only when face is ccw ✔
- mesh picking ✔
- moving picked meshes ✔
- selection manager ✔
- orbiting camera ✔
- Textured quad ✔
- fix postprocessing bug (input texture is bound as the result outprocessing texture, you dumb) 
- 3d objects outlines
- Fix PBR distortion bug (happened after camera orbiral change commit : https://github.com/AbdelrhmanIbrahim/rendering/commit/1138169c966fd829c145cfd172d17c5601d78ae1, revert camera_view_matrix() and bug fixed but figure out why)
- application state (e.g. Picking state)
- load obj textured models, check assimp impl
- normal mapping
- read more about material systems (shading models impl chp 5 in rtr + unreal blueprint) 
- deferred shading
- improve PBR + maps usage
- shadows 
- SSAO
- parallex mapping or relief mapping (raymarching) 
- OI and OD transparency like weighted average or depth peeling (nvidia bookmark and https://m.habr.com/en/post/457292/) 
- continue in real time rendering book
- MSAA enable when needed
- we do alot of fetching then uploading data to gpu even if there's no change to the data (do some kind of lazy updating)
- slerp (https://www.geometrictools.com/Documentation/FastAndAccurateSlerp.pdf)
- make the tiles ground infinite
- visualize mesh normals and point clouds with geometry shader
- imporve hiddenline visualization, check its geometry shader comments
- small profiling shows swapbuffer waiting take much time, read more about this and vsync
- lazy main loop update for input events and comparing ecs worlds in case of changing data internally
- make a shadow renderer only that outputs a texture with shadows then merge it later
- propagate uniform blocks to phong_shadow
- if we don't need exceptions, disable them
- fix imgui input handling bug
- realtime GI (global illumination) 
- main loop indpendent frame rate
- compute shaders
- space partitioning 
- view frustm culling
- resources file format
- terrian deformation
- fluid/water/particle system simulation (*read Fluid Engine Development book*)
- cloud rendering
- rigid body dynamics
- mesh destruction
- animation 
