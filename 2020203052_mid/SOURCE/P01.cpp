#include "glSetup.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

vec3 eye(3.5,3,3.5);
vec3 center(0,0,0);
vec3 up(0,1,0);

vec4 light(5.0,5.0,5.0,1);

float AXIS_LENTH=30;
float AXIS_LINE_WIDTH=2;

GLfloat bgColor[4]={1,1,1,1};

int selection =1;

GLUquadricObj* sphere =NULL;
GLUquadricObj* cylinder =NULL;
GLUquadricObj* disk = NULL;

bool pause =false;

float timeStep=1.0f/120;
float period = 4.0f;

int frame = 0;

int main(int argc, char* argv[]){
    vsync =0;

    GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
    if(window==NULL){
        return -1;
    }

    glfwSetKeyCallback(window,keyboard);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);

    reshape(window, windowW,windowH);

    init();

    float previous = (float)glfwGetTime();
    float elapsed =0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float now=(float)glfwGetTime();
        float delta = now - previous;
        previous=now;

        elapsed += delta;

        if(elapsed>timeStep){
            if(!pause){
                frame += 1;
            }
            elapsed=0;
        }

        render(window);
        glfwSwapBuffers(window);
    }
    
    quit();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void init(){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere,GLU_FILL);
    gluQuadricNormals(sphere,GLU_SMOOTH);
    gluQuadricOrientation(sphere,GLU_OUTSIDE);
    gluQuadricTexture(sphere,GL_FALSE);

    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder,GLU_FILL);
    gluQuadricNormals(cylinder,GLU_SMOOTH);
    gluQuadricOrientation(cylinder,GLU_SMOOTH);
    gluQuadricTexture(cylinder,GL_FALSE);

    disk=gluNewQuadric();
    gluQuadricDrawStyle(disk,GLU_FILL);
    gluQuadricNormals(disk,GLU_SMOOTH);
    gluQuadricOrientation(disk,GLU_OUTSIDE);
    gluQuadricTexture(disk,GL_FALSE);
}

void quit(){
    gluDeleteQuadric(sphere);
    gluDeleteQuadric(cylinder);
    gluDeleteQuadric(disk);
}

void setupLight(){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient[4] = {0.1f,0.1f,0.1f,1.0f};
    GLfloat diffuse[4] = {1.0f,1.0f,1.0f,1.0f};
    GLfloat specular[4] = {0.1f,0.1f,0.1f,1.0f};

    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0,GL_POSITION,value_ptr(light));
}

void setupMaterial(){
    GLfloat mat_ambient[4]={0.1f,0.1f,0.1f,1.0f};
    GLfloat mat_specular[4]={0.5f,0.5f,0.5f,1.0f};
    GLfloat mat_shininess =128;

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
}

void setDiffuseColor(const vec3& color){
    GLfloat mat_diffuse[4]={color[0],color[1],color[2],1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
}

void drawSphere(float radius, const vec3& color){
    setDiffuseColor(color);

    gluSphere(sphere,radius,72,72);
}

void drawCylinder(float radius,float height,const vec3& color){
    setDiffuseColor(color);
    gluCylinder(cylinder,radius,radius,height,72,5);
}

void drawDisk(float inner_radius, float outer_radius,const vec3& color){
    setDiffuseColor(color);

    gluDisk(disk,inner_radius,outer_radius,72,5);
}

void drawCube(){
    glBegin(GL_QUADS);
    //Front
    glNormal3f(0,0,1);

    glVertex3f(0,0,1);
    glVertex3f(1,0,1);
    glVertex3f(1,1,1);
    glVertex3f(0,1,1);

    //Back
    glNormal3f(0,0,-1);

    glVertex3f(0,1,0);
    glVertex3f(1,1,0);
    glVertex3f(1,0,0);
    glVertex3f(0,0,0);

    //Right
    glNormal3f(1,0,0);

    glVertex3f(1,0,1);
    glVertex3f(1,0,0);
    glVertex3f(1,1,0);
    glVertex3f(1,1,1);

    //Left
    glNormal3f(-1,0,0);

    glVertex3f(0,1,1);
    glVertex3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);

    //Top
    glNormal3f(0,1,0);

    glVertex3f(0,1,1);
    glVertex3f(1,1,1);
    glVertex3f(1,1,0);
    glVertex3f(0,1,0);

    //Bottom
    glNormal3f(0,-1,0);

    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glVertex3f(1,0,1);
    glVertex3f(0,0,1);

    glEnd();
}

void test_1(){//Draw an axis-aligned unit cube.
    setDiffuseColor(glm::vec3(1,1,1));
    drawCube();
}

void test_2(){//Scale the unit cube along the direction(1,1,0) with a scale factor s=0.2
    float s = 0.2;
    glm::vec3 direction(1,1,0);
    direction= normalize(direction);

    glm::vec3 axis= cross(glm::vec3(1,0,0),direction);
    float sinTheta=length(axis);
    float cosTheta = dot(glm::vec3(1,0,0),direction);
    float theta = (float)atan2(sinTheta,cosTheta);
    axis /= sinTheta;

    glm::mat4 M(1.0);

    M = glm::rotate(M, theta, axis); 
    M = glm::scale(M, glm::vec3(s, 1, 1)); 
    M = glm::rotate(M, -theta, axis);

    glMultMatrixf(value_ptr(M));

    setDiffuseColor(glm::vec3(1,1,1));
    drawCube();
}

void test_3(){//Repeat (2) wrt the center of the cube
    float s = 0.2;
    glm::vec3 direction(1,1,0);
    direction= normalize(direction);

    glm::vec3 axis= cross(glm::vec3(1,0,0),direction);
    float sinTheta=length(axis);
    float cosTheta = dot(glm::vec3(1,0,0),direction);
    float theta = (float)atan2(sinTheta,cosTheta);
    axis /= sinTheta;

    glm::mat4 M(1.0);

    M = glm::rotate(M, theta, axis); 
    M = glm::scale(M, glm::vec3(s, 1, 1)); 
    M = glm::rotate(M, -theta, axis);


    cosTheta = dot(glm::vec3(1,1,1),direction);
    theta = (float)atan2(sinTheta,cosTheta);
    axis /= sinTheta;

    M = glm::rotate(M, theta, axis); 
    M = glm::scale(M, glm::vec3(s, 1, 1)); 
    M = glm::rotate(M, -theta, axis);

    

    glMultMatrixf(value_ptr(M));

    setDiffuseColor(glm::vec3(1,1,1));
    drawCube();
}

void test_4(){//Rotate the cube int (3) about the z-axis wrt the center of the cube
    float s = 0.2;
    glm::vec3 direction(1,1,0);
    direction= normalize(direction);

    glm::vec3 axis= cross(glm::vec3(1,0,0),direction);
    float sinTheta=length(axis);
    float cosTheta = dot(glm::vec3(1,0,0),direction);
    float theta = (float)atan2(sinTheta,cosTheta);
    axis /= sinTheta;
    float theta1 = frame *4/period;
    glm::vec3 pivot(0.5,0.5,0);

    glm::mat4 M(1.0);

    M = glm::rotate(M, theta, axis); 
    M = glm::scale(M, glm::vec3(s, 1, 1)); 
    M = glm::rotate(M, -theta, axis);

    cosTheta = dot(glm::vec3(1,1,1),direction);
    theta = (float)atan2(sinTheta,cosTheta);
    axis /= sinTheta;

    M = glm::rotate(M, theta, axis); 
    M = glm::scale(M, glm::vec3(s, 1, 1)); 
    M = glm::rotate(M, -theta, axis);

    axis= vec3(0,0,1);

    M=glm::translate(M,pivot);
    M=glm::rotate(M,glm::radians(theta1),axis);
    M=glm::translate(M,-pivot);

    glMultMatrixf(value_ptr(M));

    setDiffuseColor(glm::vec3(1,1,1));
    drawCube();
}

void render(GLFWwindow* window){
    glClearColor(bgColor[0],bgColor[1],bgColor[2],bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],up[0],up[1],up[2]);

    glDisable(GL_LIGHTING);
    drawAxes(AXIS_LENTH,AXIS_LINE_WIDTH*dpiScaling);

    setupLight();
    setupMaterial();

    switch (selection)
    {
    case 1:
        test_1();
        break;
    case 2:
        test_2();
        break;
    case 3:
        test_3();
        break;
    case 4:
        test_4();
        break;
    }
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS||action==GLFW_REPEAT){
        switch (key)
        {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window,GL_TRUE);
            break;

        case GLFW_KEY_SPACE:
            pause=!pause;
            break;
        
        case GLFW_KEY_1:
            selection=1;
            break;
        case GLFW_KEY_2:
            selection=2;
            break;
        case GLFW_KEY_3:
            selection=3;
            break;
        case GLFW_KEY_4:
            selection=4;
            break;
        }
    }
}