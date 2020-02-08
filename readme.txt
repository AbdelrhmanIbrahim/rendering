
WINDOWS:
1) add an enviroment path called "rendering" pointing to this directory
2) add to "Path" variable "%rendering%\3rdparty\glew_glut\bin"


TODO:
1) remove GLUT - ?
2) cmake
3) support renderdoc by creating context using wglCreateContextAttribsARB then init glew
4) add imGui
5) make a shadow renderer only that outputs a texture with shadows then merge it later
6) add ambient occlusion
7) main loop indpendent frame rate
