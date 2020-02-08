Windows: add an enviroment path called "rendering" pointing to this directory

TODO:
1) remove GLUT - ✔
2) remove prebuilt glew binaries, add glew.c to the project - ✔
3) cmake
4) support renderdoc by creating context using wglCreateContextAttribsARB then init glew
5) add imGui
6) make a shadow renderer only that outputs a texture with shadows then merge it later
7) add ambient occlusion
8) main loop indpendent frame rate
