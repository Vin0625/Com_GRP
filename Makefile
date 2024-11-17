CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/include #GLFW헤더 위치
LDFLAGS = -framework OpenGL -L/opt/homebrew/lib -lglfw #glfw라이브러리 위치

TARGET = E01
SOURCES = E01.cpp glSetup.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
