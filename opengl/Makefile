BINARY_NAME = test_02

SRC_DIRS = src

OPT = -g -O3 -pedantic -Wall -Wsign-conversion

LIBNAMES = sdl gl ftgl freetype2

#FLAGS = -g -lSDL -lSDL_ttf -mfpmath=sse -mtune=cpu-type=k8-sse3
FLAGS = $(shell pkg-config --libs $(LIBNAMES)) $(OPT)
CXXFLAGS = $(shell pkg-config --cflags $(LIBNAMES))  $(OPT)

SRC_OBJS = $(shell find $(SRC_DIRS) -type f -name '*.cpp' | sed 's/cpp$$/o/g')
RM_OBJS = $(shell find $(SRC_DIRS) -type f -name '*.o')
HDRS = $(shell find $(SRC_DIRS) -type f -name '*.h')

all: $(BINARY_NAME)

$(BINARY_NAME): $(SRC_OBJS) $(HDRS)
	$(CXX) -o $(BINARY_NAME) $(FLAGS) $(SRC_OBJS)

clean:
	rm -f $(RM_OBJS)
	rm -f $(BINARY_NAME)
