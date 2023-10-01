#pragma once
#include <stdint.h>

typedef uint32_t dword;
typedef uint16_t word;
typedef uint8_t byte;

const int memory_size = 1048576;	// 2 ^ 20 byte

// bits of flags register
static enum class Flag {
	O = 11, D = 10, I = 9,
	T = 8, S = 7, Z = 6,
	A = 4, P = 2, C = 0
};

static enum class RegId {
	AL, AH, AX,
	BL, BH, BX,
	CL, CH, CX,
	DL, DH, DX,
	SP, BP, SI, DI,
	CS, DS, SS, ES,
	IP
};

static enum OpType {
	Byte, Word
};