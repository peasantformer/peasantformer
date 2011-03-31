BINARY_NAME = test_01

SRC_DIRS = src

OPT = 

#FLAGS = -g -lSDL -lSDL_ttf -mfpmath=sse -mtune=cpu-type=k8-sse3
FLAGS = -g `sdl-config --libs --cflags` -lSDL_ttf -pedantic -Wall -Wsign-conversion $(OPT)
CXXFLAGS = -pedantic -Wall -g -Wsign-conversion $(OPT)

SRC_OBJS = $(shell find $(SRC_DIRS) -type f -name '*.cpp' | sed 's/cpp$$/o/g')
RM_OBJS = $(shell find $(SRC_DIRS) -type f -name '*.o')
HDRS = $(shell find $(SRC_DIRS) -type f -name '*.h')

all: $(BINARY_NAME)

$(BINARY_NAME): $(SRC_OBJS) $(HDRS)
	$(CXX) -o $(BINARY_NAME) $(FLAGS) $(SRC_OBJS)

clean:
	rm -f $(RM_OBJS)
	rm -f $(BINARY_NAME)
