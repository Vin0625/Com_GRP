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
bool polygonFill=false;

int selection =1;

GLUquadricObj* sphere =NULL;
GLUquadricObj* cylinder =NULL;
GLUquadricObj* disk = NULL;

bool pause =false;

float timeStep=1.0f/120;
float period = 4.0f;

float delta=0;

int frame = 0;

vec3 p[36][18];

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
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    GLfloat ambient[4] = {0.1f,0.1f,0.1f,1.0f};
    GLfloat diffuse[4] = {1.0f,1.0f,1.0f,1.0f};
    GLfloat specular[4] = {0.1f,0.1f,0.1f,1.0f};

    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0,GL_POSITION,value_ptr(light));
}

void setupMaterial(){
    GLfloat mat_specular[4]={0.5f,0.5f,0.5f,1.0f};
    GLfloat mat_shininess =128;


    GLfloat ambientR[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat ambientL[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientL);
    glMaterialfv(GL_BACK,GL_AMBIENT,ambientR);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
}

void setDiffuseColor(const vec3& color){
    GLfloat mat_diffuse[4]={color[0],color[1],color[2],1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
}

const int numPoints = 18; 
const float radius = 0.3f;

void task1(){
    float theta = glm::radians(10.0f); // 회전 각도
    glm::vec3 axis(0.0f, 1.0f, 0.0f); // Y축
    
    for (int i = 0; i < numPoints; ++i) {
            float angle = 2.0f * M_PI * i / numPoints; 
            float x = radius * cos(angle) + 0.5f; 
            float z = 0; 
            float y = radius * sin(angle) + 0.5f;

            p[0][i]=vec3(x,y,z);

        }
    if(delta>=0){
        for(int j=1;j<36;j++){
        for (int i = 0; i < numPoints; ++i) {
            mat4 M = glm::rotate(glm::mat4(1.0f), theta, axis);
            vec4 point(p[j-1][i].x, p[j-1][i].y, p[j-1][i].z, 1.0f);
            p[j][i]=vec3(M*point);
        }
    }
    }else{
        for(int j=1;j<36;j++){
        for (int i = 0; i < numPoints; ++i) {
            mat4 M = glm::rotate(glm::mat4(1.0f), -theta, axis);
            vec4 point(p[j-1][i].x, p[j-1][i].y, p[j-1][i].z, 1.0f);
            p[j][i]=vec3(M*point);
        }
    }
    }


    if(delta!=0){

        if(polygonFill){
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glPolygonOffset(1.0f, 1.0f);

            glBegin(GL_QUADS);
        for (int i = 0; i < abs(delta); ++i) {
        for (int j = 0; j < numPoints; ++j) {
            int nextIndex = (j + 1) % numPoints;
            glm::vec3 normal;
            normal = glm::normalize(glm::cross(p[i][nextIndex] - p[i][j], p[i + 1][j] - p[i][j]));

               
                if(dot(normal, eye) > 0.0f) {
                    if(delta>0){
                        glColor3f(1.0f, 0.0f, 0.0f);
                    }else{
                        glColor3f(0.0f, 0.0f, 1.0f);
                    }
                } else {
                    if(delta>0){
                        glColor3f(0.0f, 0.0f, 1.0f);
                    }else{
                        glColor3f(1.0f, 0.0f, 0.0f);
                    }
                }
            glVertex3f(p[i][j].x, p[i][j].y, p[i][j].z);
            glVertex3f(p[i][nextIndex].x, p[i][nextIndex].y, p[i][nextIndex].z);
            glVertex3f(p[i + 1][nextIndex].x, p[i + 1][nextIndex].y, p[i + 1][nextIndex].z);
            glVertex3f(p[i + 1][j].x, p[i + 1][j].y, p[i + 1][j].z);
            }
        }
        glEnd();
        }
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glBegin(GL_QUADS);
        for (int i = 0; i < abs(delta); ++i) {
        for (int j = 0; j < numPoints; ++j) {
            int nextIndex = (j + 1) % numPoints;
            glm::vec3 normal;
            normal = glm::normalize(glm::cross(p[i][nextIndex] - p[i][j], p[i + 1][j] - p[i][j]));

            
            glColor3f(0,0,0);
                
            glVertex3f(p[i][j].x, p[i][j].y, p[i][j].z);
            glVertex3f(p[i][nextIndex].x, p[i][nextIndex].y, p[i][nextIndex].z);
            glVertex3f(p[i + 1][nextIndex].x, p[i + 1][nextIndex].y, p[i + 1][nextIndex].z);
            glVertex3f(p[i + 1][j].x, p[i + 1][j].y, p[i + 1][j].z);
            }
        }
        glEnd();

    }else if (delta==0)
    {

        glBegin(GL_LINE_LOOP);
        glColor3f(0.0f, 0.0f, 0.0f);
        for(int i=0;i<numPoints;i++){
            glVertex3f(p[0][i].x,p[0][i].y,p[0][i].z);
        }
        
        glEnd();
    }
    
    

}

void render(GLFWwindow* window){
    glClearColor(bgColor[0],bgColor[1],bgColor[2],bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],up[0],up[1],up[2]);

    glDisable(GL_LIGHTING);
    drawAxes(AXIS_LENTH,AXIS_LINE_WIDTH*dpiScaling);

    setupLight();
    setupMaterial();

    glm::vec3 axis(0,1,0);

    switch (selection)
    {
    case 1:
        task1();
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
        case GLFW_KEY_LEFT:
            if(delta>-35){
                delta-=1;
            }
            break;
        case GLFW_KEY_RIGHT:
            if(delta<35){
                delta+=1;
            }
            break;

        case GLFW_KEY_F:
            polygonFill= !polygonFill;
            break;
        }
    }
}