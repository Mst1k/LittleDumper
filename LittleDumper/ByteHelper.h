#pragma once

#include <string>
#include <vector>
#include "PatternByte.h"

class ByteHelper {
public:
    static std::string Byte2String(unsigned char byte);
	static void Pattern2Bytes(const std::string& pattern, std::vector<unsigned char>& result); // 
	static void Pattern2Bytes(const std::string& pattern, std::vector<PatternByte>& result);
};