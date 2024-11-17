#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

void exer1(){
    cout<<"Compute the following vector operations"<<endl;
    
    vec3 a(1,2,3);
    vec3 b(3,2,1);

    cout<<"V1 + V2 = "<<to_string(a+b)<<endl;
    cout<<"V1 - V2 = "<<to_string(a-b)<<endl;
    cout<<"-V1 = "<<to_string(-a)<<endl;
    cout<<"V1 - 2*V2 = "<<to_string(a-2.0f*b)<<endl;
    cout<<"V1 · V2 = "<<to_string(dot(a,b))<<endl;
    cout<<"V1 X V2 = "<<to_string(cross(a,b))<<endl;
}

void exer2(){
    cout<<"Compute the following matrix operations"<<endl;

    mat3 A(1.0,2.0,1.0,2.0,3.0,1.0,3.0,2.0,2.0);
    mat3 B(2.0,2.0,1.0,1.0,2.0,1.0,2.0,1.0,1.0);
    vec3 a(1,2,3);
    vec3 b(3,2,1);

    cout<<"A1 + A2 = "<<to_string(A+B)<<endl;
    cout<<"A1 - A2 = "<<to_string(A-B)<<endl;
    cout<<"-A1 = "<<to_string(-A)<<endl;
    cout<<"A1 - 2*A2 = "<<to_string(A-2.0f*B)<<endl;
    cout<<"A1*A2 = "<<to_string(A*B)<<endl;
    cout<<"A2*A1 = "<<to_string(B*A)<<endl;
    cout<<"A1*V1 = "<<to_string(A*a)<<endl;
    cout<<"A2*V2 = "<<to_string(B*b)<<endl;
    
}

int main(int argc, char* argv[]){
    int e=(argc<2) ? 1 : atoi(argv[1]);

    switch (e)
    {
    case 1:
        exer1();
        break;
    case 2:
        exer2();
        break;
    }

    return 0;
}