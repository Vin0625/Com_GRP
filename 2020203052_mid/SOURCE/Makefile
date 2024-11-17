CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/include -O2
LDFLAGS = -framework OpenGL -L/opt/homebrew/lib -lglfw

TARGETS = P01

P01_SOURCES = P01.cpp glSetup.cpp

P01_OBJECTS = $(P01_SOURCES:.cpp=.o)

all: $(TARGETS)

P01: $(P01_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(P02_OBJECTS) $(P01_OBJECTS)
