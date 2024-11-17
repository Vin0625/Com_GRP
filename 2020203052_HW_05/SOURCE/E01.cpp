#include "glSetup.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window,int key, int code, int action, int mods);

bool readMesh(const char* filename);
void deleteMesh();

vec3 eye(3,3,3);
vec3 center(0,0,0);
vec3 up(0,0,1);

vec4 light(5.0,5.0,0.0,1);

float AXIS_LENGTH=1.75;
float AXIS_LINE_WIDTH=2;

GLfloat bgColor[4]= {1,1,1,1};

int selection =1;

GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk =NULL;

bool pause =true;

float timeStep=1.0f/120;
float period = 8.0;
float theta =0;

bool depthTest=true;
bool polygonFill=true;

vector<list<int>> edgesList(2162);

int main(int argc, char* argv[]){
    vsync = 0;

    GLFWwindow* window=initializeOpenGL(argc,argv,bgColor);
    if(window==NULL){
        return -1;
    }

    glfwSetKeyCallback(window,keyboard);

    glEnable(GL_NORMALIZE);

    glDisable(GL_CULL_FACE);

    reshape(window,windowW,windowH);

    init();

    float previous = (float)glfwGetTime();
    float elapsed=0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float now=(float)glfwGetTime();
        float delta = now - previous;
        previous =now;

        elapsed +=delta;

        if(elapsed>timeStep){
            if(!pause){
                theta+=float(2.0*M_PI)/period*elapsed;
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
    readMesh("m01_bunny.off");

    cout<<endl;
    cout<<"Keyboard Input: d for depth test on/off"<<endl;
    cout<<"Keyboard Input: f for polygon fill on/off"<<endl;
    cout<<endl;
}

void quit(){
    deleteMesh();
}

void setupLight(){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient[4]= {0.1f,0.1f,0.1f,1.0f};
    GLfloat diffuse[4]={1.0f,1.0f,1.0f,1.0f};
    GLfloat specular[4]={1.0f,1.0f,1.0f,1.0f};

    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0,GL_POSITION,value_ptr(light));

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
}

void setupMaterial(){
    GLfloat mat_ambient[4]={0.1f,0.1f,0.1f,1.0f};
    GLfloat mat_specular[4]={0.5f,0.5f,0.5f,1.0f};
    GLfloat mat_shinineww = 128;

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat_shinineww);
}

void setDiffuseColor(const vec3& color){
    GLfloat mat_diffuse[4]={color[0],color[1],color[2],1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
}

int nVertices=0, nFaces=0, nEdges=0;
vec3* vertex=NULL;
int* face[3]={NULL,NULL,NULL};

bool readMesh(const char* filename) {
    std::ifstream is(filename);
    if (is.fail()) {
        return false; 
    }

    char magicNumber[256];
    is >> magicNumber;

    is >> nVertices >> nFaces >> nEdges;
    std::cout << "# vertices = " << nVertices << std::endl; 
    std::cout << "# faces = " << nFaces << std::endl; 

    vertex = new vec3[nVertices]; 
    for (int i = 0; i < nVertices; i++) {
        is >> vertex[i].x >> vertex[i].y >> vertex[i].z; 
    }

    face[0] = new int[nFaces]; 
    face[1] = new int[nFaces];
    face[2] = new int[nFaces];

    edgesList.resize(nVertices);

    int n;
    for (int i = 0; i < nFaces; i++) {
        is >> n >> face[0][i] >> face[1][i] >> face[2][i]; 
        if (n != 3) { 
            std::cout << "# vertices of the " << i << "-th faces = " << n << std::endl;
        } else {
            int v1 = face[0][i];
            int v2 = face[1][i];
            int v3 = face[2][i];

            int edge1_min = std::min(v1, v2);
            int edge1_max = std::max(v1, v2);
            if (std::find(edgesList[edge1_min].begin(), edgesList[edge1_min].end(), edge1_max) == edgesList[edge1_min].end()) {
                edgesList[edge1_min].push_back(edge1_max); 
            }

            int edge2_min = std::min(v2, v3);
            int edge2_max = std::max(v2, v3);
            if (std::find(edgesList[edge2_min].begin(), edgesList[edge2_min].end(), edge2_max) == edgesList[edge2_min].end()) {
                edgesList[edge2_min].push_back(edge2_max);
            }

            int edge3_min = std::min(v3, v1);
            int edge3_max = std::max(v3, v1);
            if (std::find(edgesList[edge3_min].begin(), edgesList[edge3_min].end(), edge3_max) == edgesList[edge3_min].end()) {
                edgesList[edge3_min].push_back(edge3_max); 
            }
        }
    }

    int edgeCount = 0;
    for (const auto& edgeList : edgesList) {
        edgeCount += edgeList.size();
    }
    std::cout << "# edges = " << edgeCount << std::endl;

    return true;
}


void deleteMesh(){
    if(vertex){
        delete[] vertex;
        vertex = NULL;
    }
    if(face[0]){
        delete[] face[0];
        face[0]=NULL;
    }
    if(face[1]){
        delete[] face[1];
        face[1]=NULL;
    }
    if(face[2]){
        delete[] face[2];
        face[2]=NULL;
    }
}

void drawEdges() {
    glBegin(GL_LINES);
    for (int i = 0; i < nVertices; ++i) {
        for (int adjacent : edgesList[i]) {
            glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
            glVertex3f(vertex[adjacent].x, vertex[adjacent].y, vertex[adjacent].z);
        }
    }
    glEnd();
}

void drawMesh(){
    setDiffuseColor(vec3(0,0,0));

    glRotatef(90,0,0,1);
    glRotatef(90,1,0,0);

    glPointSize(3*dpiScaling);
    glBegin(GL_POINTS);
    for(int i=0;i<nVertices;i++){
        glVertex3f(vertex[i].x,vertex[i].y,vertex[i].z);
    }
    glEnd();

    drawEdges();
}

void render(GLFWwindow* window){
    if(depthTest){
        glEnable(GL_DEPTH_TEST);
    }else{
        glDisable(GL_DEPTH_TEST);
    }

    if(polygonFill){
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }

    glClearColor(bgColor[0],bgColor[1],bgColor[2],bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],up[0],up[1],up[2]);

    glScalef(1.5f, 1.5f, 1.5f);

    glDisable(GL_LIGHTING);
    drawAxes(AXIS_LENGTH,AXIS_LINE_WIDTH*dpiScaling);

    setupLight();

    setupMaterial();

    glRotatef(degrees(theta),0,1,0);

    drawMesh();
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action==GLFW_PRESS||action==GLFW_REPEAT){
        switch ((key))
        {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window,GL_TRUE);
            break;
        case GLFW_KEY_SPACE:
            pause = !pause;
            break;

    }
}
}