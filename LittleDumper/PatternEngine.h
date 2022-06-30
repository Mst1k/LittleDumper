#pragma once

#include <vector>

struct File;
struct Pattern;

class PatternEngine
{
public:
	static bool CompareMem(const unsigned char* memChunk, const unsigned char* mask, const size_t len);
	static void FindPattern(const File* pFile, const Pattern& pPatternMask, std::vector<uintptr_t>& outResults);
	static void FindPattern(const unsigned char* pStartAddr, const unsigned char* pEndAddr, const unsigned char* pMask, size_t maskLen, std::vector<uintptr_t>& outResults);
	static void FindPatterns(const File* pFile, const std::vector<std::vector<unsigned char>>& vecOfPatterns, const std::vector<std::vector<uintptr_t>*>& vecOfOutputs);
	static void FindPatterns(const unsigned char* pStartAddr, const unsigned char* pEndAddr, const std::vector<std::vector<unsigned char>>& vecOfPatterns, const std::vector<std::vector<uintptr_t>*>& vecOfOutputs);
};

