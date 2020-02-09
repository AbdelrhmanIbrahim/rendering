Windows: add an enviroment path called "rendering" pointing to this directory

TODO:
1) remove GLUT - ✔
2) remove prebuilt glew binaries, add glew.c to the project - ✔
3) cmake
4) support renderdoc by creating context using wglCreateContextAttribsARB then init glew
5) fix rotation+movement events bug
6) add imGui
7) make a shadow renderer only that outputs a texture with shadows then merge it later
8) deferred shading + ambient occlusion
9) main loop indpendent frame rate
10) load textured models, check assimp impl
11) resources file format
12) selection and picking
13) terrian formation
