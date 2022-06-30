#pragma once

#include <stdio.h>
#include <string>

struct Buffer;

class FileHelper {
public:
    static bool FileExist(const std::string& filePathStr);
    static off_t getFileSize(FILE* pFile);
    static bool ReadFile(const std::string& filePath, Buffer& pOutBuffer);
};

