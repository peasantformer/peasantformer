BINARY_NAME = test_01

SRC_DIRS = src

#FLAGS = -g -lSDL -mfpmath=sse -mtune=cpu-type=k8-sse3 -O3
FLAGS = `sdl-config --libs --cflags` -pedantic -Wall
CXXFLAGS = -g -pedantic -Wall

SRC_OBJS = $(shell find $(SRC_DIRS) -type f -name '*.cpp' | sed 's/cpp$$/o/g')
RM_OBJS = $(shell find $(SRC_DIRS) -type f -name '*.o')
HDRS = $(shell find $(SRC_DIRS) -type f -name '*.h')

all: $(BINARY_NAME)

$(BINARY_NAME): $(SRC_OBJS) $(HDRS)
	$(CXX) -o $(BINARY_NAME) $(FLAGS) $(SRC_OBJS)

clean:
	rm -f $(RM_OBJS)
	rm -f $(BINARY_NAME)
