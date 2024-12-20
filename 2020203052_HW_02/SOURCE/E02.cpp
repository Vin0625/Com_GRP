#include "glSetup.h"
#include <iostream>

void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

GLfloat bgColor[4]={1,1,1,1};

bool stipple =false;
bool fill=true;

int main(int argc, char* argv[]){
    perspectiveView =false;

    GLFWwindow* window = initializeOpenGL(argc,argv,bgColor);
    if(window == NULL){
        return -1;
    }

    glfwSetKeyCallback(window,keyboard);

    reshape(window,windowW,windowH);

    std::cout << std::endl;
    std::cout<< "Keyboard Input: q for quit"<<std::endl;

    while(!glfwWindowShouldClose(window)){
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void render(GLFWwindow* window){
    glClearColor(bgColor[0],bgColor[1],bgColor[2],bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(stipple){
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(int(3*dpiScaling),0xcccc);
    }else{
        glDisable(GL_LINE_STIPPLE);
    }

    if(fill){
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(5*dpiScaling);

    glBegin(GL_QUAD_STRIP);
    {
        glColor3f(1,0,0); glVertex2f(-1.5,-0.5);
        glColor3f(0,1,0); glVertex2f(-1.5,0.5);
        glColor3f(0,0,1); glVertex2f(-0.5,-0.5);
        glColor3f(0,0,0); glVertex2f(-0.5,0.5);

        glColor3f(0,0,1); glVertex2f(0.5,-0.5);
        glColor3f(0,0,0); glVertex2f(0.5,0.5);

        glColor3f(1,0,0); glVertex2f(1.5,-0.5);
        glColor3f(0,1,0); glVertex2f(1.5,0.5);
        
    }

    glEnd();
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action==GLFW_PRESS||action==GLFW_REPEAT){
        switch (key)
        {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window,GL_TRUE);
            break;
        case GLFW_KEY_S: 
            stipple = !stipple;
            break;
        case GLFW_KEY_F:
            fill=!fill;
            break;
        }
    }
}