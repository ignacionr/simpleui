CXXFLAGS := `sdl2-config --cflags` --std=c++17 -Wall
HDRS := sdlhelp.h
LDFLAGS := -lSDL2_image `sdl2-config --libs`
SRCS := simpleui.cpp
OBJS := $(SRCS:.c=.o)
EXEC := simpleui

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): $(@:.o=.cpp) $(HDRS) Makefile
	$(CXX) $@ -o $(@:.cpp=.o) -c $(CXXFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean
