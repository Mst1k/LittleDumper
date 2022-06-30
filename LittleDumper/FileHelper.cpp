#include "FileHelper.h"

#include "Buffer.h"

bool FileHelper::FileExist(const std::string& filePathStr)
{
    bool bExist = false;
    FILE* pFile = nullptr;

    if (!fopen_s(&pFile, filePathStr.c_str(), "r"))
    {
        fclose(pFile);
        bExist = true;
    }

    return bExist;
}

off_t FileHelper::getFileSize(FILE* pFile)
{
    off_t currOffset = ftell(pFile);

    // Going to the bottom
    fseek(pFile, 0, SEEK_END);
    off_t bottomOffset = ftell(pFile);

    // Restoring Stream Pointer
    fseek(pFile, currOffset, SEEK_SET);

    return bottomOffset;
}

bool FileHelper::ReadFile(const std::string& filePath, Buffer& pOutBuffer)
{
    FILE* pFile = nullptr;
    bool bFileReaded = false;

    if (!fopen_s(&pFile, filePath.c_str(), "rb"))
    {
        size_t fileSize = getFileSize(pFile);

        if (fileSize > 0)
        {
            pOutBuffer.mSize = fileSize;
            pOutBuffer.mBuff = new unsigned char[fileSize];

            if (pOutBuffer.mBuff) bFileReaded = fread(pOutBuffer.mBuff, fileSize, 1, pFile) > 0;
        }

        fclose(pFile);
    }

    return bFileReaded;
}
