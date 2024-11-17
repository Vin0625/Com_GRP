CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/include -O2
LDFLAGS = -framework OpenGL -L/opt/homebrew/lib -lglfw

TARGETS = T01

T01_SOURCES = T01.cpp glSetup.cpp

T01_OBJECTS = $(T01_SOURCES:.cpp=.o)

all: $(TARGETS)

T01: $(T01_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(T01_OBJECTS)
