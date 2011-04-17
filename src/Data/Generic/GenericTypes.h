#ifndef PEASANTFORMER_Data_GenericTypes
#define PEASANTFORMER_Data_GenericTypes

#include <stdint.h>

typedef int32_t PeasantPos;
typedef int32_t PeasantID;
typedef int32_t PeasantSize;

enum Direction {
	INVALID = 0x0,
	MOVE_UP = 0x1,
	MOVE_DOWN = 0x2,
	MOVE_LEFT = 0x4,
	MOVE_RIGHT = 0x8
};

#endif
