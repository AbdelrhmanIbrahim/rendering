
WINDOWS:
1) add an enviroment path called "rendering" pointing to this directory
2) add to "Path" variable "%rendering%\3rdparty\glew_glut\bin"


TODO:
1) add GLFW instead of GLUT
2) support renderdoc by creating context using wglCreateContextAttribsARB then init glew
3) add imGui
4) make a shadow renderer only that outputs a texture with shadows then merge it later
5) add ambient occlusion
6) main loop indpendent frame rate
