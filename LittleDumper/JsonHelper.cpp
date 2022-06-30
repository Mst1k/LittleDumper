#include "JsonHelper.h"
#include "FilesEngine.h"

bool JsonHelper::String2Json(const std::string& fullJsonStr, Json::Value& outJson)
{
    Json::CharReaderBuilder charReaderBuilder;
    Json::IStringStream stringStream = Json::IStringStream(fullJsonStr);
    
    return Json::parseFromStream(charReaderBuilder, stringStream, &outJson, nullptr);
}

bool JsonHelper::File2Json(const std::string& filePath, Json::Value& outJson)
{
    FilesEngine fEngine;
    File* pFile = nullptr;

    if (fEngine.LoadFile(filePath, &pFile)) return String2Json((const char*)pFile->pBuff->mBuff, outJson);

    return false;
}
