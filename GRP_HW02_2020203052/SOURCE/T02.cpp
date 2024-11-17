#include "glSetup.h"
#include <iostream>
#include <cmath>

using namespace std;

#ifdef _WIN32
    #define _USE_MATH_DEFINES
#endif


#include <math.h>

void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void mouseButton(GLFWwindow* window, int button, int action,int mods);
void mouseMove(GLFWwindow* window, double x, double y);
bool checkpoint(double x, double y);
void delpoint();
bool checksegment(double x, double y);

float AXIS_LENGTH = 0.5;
float AXIS_LINE_WIDTH = 2.0;

GLfloat bgColor[4]={1,1,1,1};

float point[25][2]={{0,0},{0,0},{0.0}};
int check=0;

enum class InputMode{
    NONE = 0,
    SELECT = 1,
    COMPLETE=2,
};

InputMode inputMode =InputMode::NONE;

bool pause = true;

float timeStep=1.0f/120;
float period = 8.0;

int pointnum=0;

void rotate(float p[2],float delta_theta){
    float x= p[0];
    float y=p[1];

    float cos_delta_theta =cos(delta_theta);
    float sin_delta_theta=sin(delta_theta);

    p[0]=cos_delta_theta* x - sin_delta_theta* y;
    p[1]=sin_delta_theta * x + cos_delta_theta * y;

}

int main(int argc, char* argv[]){
    perspectiveView =false;

    vsync = 0;

    GLFWwindow* window = initializeOpenGL(argc,argv,bgColor);
    if(window==NULL){
        return -1;
    }

    glfwSetKeyCallback(window,keyboard);
    glfwSetMouseButtonCallback(window,mouseButton);
    glfwSetCursorPosCallback(window,mouseMove);

    reshape(window,windowW,windowH);

    float previous =(float)glfwGetTime();
    float elapsed =0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float now=(float)glfwGetTime();
        float delta_t=now-previous;
        previous=now;

        elapsed +=delta_t;

        render(window);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
    
}

void render(GLFWwindow* window){
    glClearColor(bgColor[0],bgColor[1],bgColor[2],bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPointSize(10);
    glLoadIdentity();

    glDisable(GL_LINE_STIPPLE);

    glLineWidth(3*dpiScaling);

    if(pointnum>1){
        for(int i=0;i<pointnum-1;i++){
             if(inputMode==InputMode::SELECT){
                if(i==check-1||i==check){
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(int(3*dpiScaling),0xcccc);
                }else{
                    glDisable(GL_LINE_STIPPLE);
                }
            }else{
                glDisable(GL_LINE_STIPPLE);
            }
            glBegin(GL_LINES);
            glColor3f(0,0,0);
            glVertex2f(point[i][0],point[i][1]);
            glVertex2f(point[i+1][0],point[i+1][1]);
            glEnd();


            glBegin(GL_POINTS);
            glColor3f(1,1,0);
            glPointSize(10);
            glVertex2f(point[i][0],point[i][1]);
            glEnd();
        }
    }else{
        if(pointnum>0){
            glBegin(GL_POINTS);
            glPointSize(10);
            glColor3f(1,1,0);
            glVertex2f(point[0][0],point[0][1]);
        glEnd();
        }
    }

    }
    


void keyboard(GLFWwindow* window, int key, int scancode, int action,int mods){
    if(action==GLFW_PRESS||action==GLFW_REPEAT){
        switch (key)
        {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window,GL_TRUE);
            break;

        case GLFW_KEY_SPACE:
            pause=!pause;
            break;

        case GLFW_KEY_UP:
            period+=0.1f;
            break;

        case GLFW_KEY_DOWN:
            period = (float)max(period-0.1,0.1);
            break;
        case GLFW_KEY_BACKSPACE:
            if(inputMode==InputMode::SELECT){
                delpoint();
            }
            break;
        case GLFW_KEY_R:
            pointnum=0;
            break;
        }
    }
}

void screen2world(float xs, float ys, float& xw, float& yw){
    float aspect = (float)screenW/screenH;
    xw=2.0f*(xs/(screenW-1)-0.5f)*aspect;
    yw=-2.0f * (ys/(screenH-1)-0.5f);
}

bool checkpoint(double x, double y){
    float distance;

    for(int i=0;i<pointnum;i++){
        distance=sqrt((x-point[i][0])*(x-point[i][0])+(y-point[i][1])*(y-point[i][1]));
        if(distance<0.1){
            check=i;
            return true;
        }
    }

    return false;
}

void mouseButton(GLFWwindow* window, int button, int action, int mods){
    double xs,ys;
    glfwGetCursorPos(window,&xs,&ys);

    float xw,yw;
    screen2world((float)xs,(float)ys,xw,yw);

    if(action==GLFW_PRESS&&button==GLFW_MOUSE_BUTTON_LEFT){//마우스 왼쪽 눌리때
        if(inputMode==InputMode::SELECT){
            inputMode=InputMode::NONE;
            point[check][0]=xw;
            point[check][1]=yw;

        }else if (checkpoint(xw,yw))
        {   inputMode=InputMode::SELECT;
            point[check][0]=xw;
            point[check][1]=yw;
            
        }else if(checksegment(xw,yw)){
            for(int i=pointnum;i>check+1;i--){
                point[i][0]=point[i-1][0];
                point[i][1]=point[i-1][1];
            }
            point[check+1][0]=xw;
            point[check+1][1]=yw;
            pointnum++;
        }else{
            point[pointnum][0]=xw;
            point[pointnum][1]=yw;
            pointnum++;
        }
    }

}

void mouseMove(GLFWwindow* window, double x, double y){
    if(inputMode==InputMode::SELECT){
        screen2world((float)x,(float)y,point[check][0],point[check][1]);
    }
}

void delpoint(){
    if(check!=pointnum-1){
        for(int i=check;i<pointnum-1;i++){
        point[i][0]=point[i+1][0];
        point[i][1]=point[i+1][1];
        }
    }
    pointnum--;
    inputMode=InputMode::NONE;

}

bool checksegment(double x, double y){
    float distance;
    float a;
    float b;
    float c;
    for(int i=0;i<pointnum-1;i++){
    
        if (point[i + 1][0] == point[i][0]) { 
            distance = fabs(y - point[i][1]);
        } else if (point[i + 1][1] == point[i][1]) { 
            distance = fabs(x - point[i][0]); 
        } else {
            a = (point[i + 1][1] - point[i][1]) / (point[i + 1][0] - point[i][0]);
            b = -1;
            c = point[i][1] - a * point[i][0];
            distance = fabs(a * x + b * y + c) / sqrt(a * a + b * b);
        }

        if(x >= std::min(point[i][0], point[i+1][0]) && x <= std::max(point[i][0], point[i+1][0]) &&
                y >= std::min(point[i][1], point[i+1][1]) && y <= std::max(point[i][1], point[i+1][1])&&distance<0.01){
            check=i;
            return true;
        }
    }

    return false;
}