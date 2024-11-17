CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/include #GLFW헤더 위치
LDFLAGS = -framework OpenGL -L/opt/homebrew/lib -lglfw #glfw라이브러리 위치

TARGETS = E01 P01

E01_SOURCES = E01.cpp glSetup.cpp
P01_SOURCES = P01.cpp glSetup.cpp

E01_OBJECTS = $(E01_SOURCES:.cpp=.o)
P01_OBJECTS = $(P01_SOURCES:.cpp=.o)

all: $(TARGETS)

E01: $(E01_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

P01: $(P01_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(E01_OBJECTS) $(P01_OBJECTS)
