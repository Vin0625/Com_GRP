CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/include -O2
LDFLAGS = -framework OpenGL -L/opt/homebrew/lib -lglfw

TARGETS = T02

T02_SOURCES = T02.cpp glSetup.cpp

T02_OBJECTS = $(T02_SOURCES:.cpp=.o)

all: $(TARGETS)

T02: $(T02_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(T02_OBJECTS)
