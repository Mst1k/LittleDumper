#pragma once

#include <cstdint>

struct cs_insn;

class ArmCapstoneHelper{
public:
	static uint16_t GetLValueRegType(cs_insn* pInst);
	static uint16_t GetRValueRegType(cs_insn* pInst);
	static uint16_t GetRegTypeById(cs_insn* pInst, uint16_t opId);
	static bool RegisterPresent(cs_insn* pInst, uint16_t reg);
	static bool HeuristicReturn(cs_insn* pInst);
	static uintptr_t ResolvePCRelative(unsigned char* pInst, uintptr_t pcOffset);
};

class Arm64CapstoneHelper {
public:
	static uint16_t GetLValueRegType(cs_insn* pInst);
	static uint16_t GetRValueRegType(cs_insn* pInst);
	static bool RegisterPresent(cs_insn* pInst, uint16_t reg);
	static bool HeuristicReturn(cs_insn* pInst);
};


