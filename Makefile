CXXFLAGS = -g -W -std=c++11 -I/usr/local/include
LDLIBS = -L/usr/local/lib -lglfw -lGLEW -framework OpenGL -framework CoreVideo -framework IOKit -framework Cocoa
OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
TARGET = sample

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	-$(RM) $(TARGET) $(OBJECTS) *~ .*~ core
