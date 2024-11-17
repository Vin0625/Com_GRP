#ifndef __GL_SETUP_H_
#define __GL_SETUP_H_
#include <OpenGL/glu.h>

#include <GLFW/glfw3.h>

extern float screenScale;
extern int screenW, screenH;
extern int windowW, windowH;
extern float aspect;
extern float dpiScaling;

extern int vsync;

extern bool perspectiveView;
extern float fovy;
extern float nearDist;
extern float farDist;

GLFWwindow* initializeOpenGL(int argc, char* argv[],GLfloat bg[4]);
void    reshape(GLFWwindow* window,int w, int h);
void    drawAxes(float l,float w);

#endif