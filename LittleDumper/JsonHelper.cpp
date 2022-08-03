#include "JsonHelper.h"
#include "FilesEngine.h"
#include <fstream>

bool JsonHelper::String2Json(const std::string& fullJsonStr, Json::Value& outJson)
{
    Json::CharReaderBuilder charReaderBuilder;
    Json::IStringStream stringStream = Json::IStringStream(fullJsonStr);
    
    return Json::parseFromStream(charReaderBuilder, stringStream, &outJson, nullptr);
}

bool JsonHelper::Json2File(const Json::Value& jsonRoot, const std::string& outPath)
{
    Json::FastWriter fw;

    std::string jsonStr = fw.write(jsonRoot);

    std::ofstream fTrait(outPath);

    if (fTrait.is_open())
    {
        fTrait.clear();
        fTrait << jsonStr;
        fTrait.close();

        return true;
    }

    return false;
}

bool JsonHelper::File2Json(const std::string& filePath, Json::Value& outJson)
{
    FilesEngine fEngine;
    File* pFile = nullptr;

    if (fEngine.LoadFile(filePath, &pFile)) return String2Json((const char*)pFile->pBuff->mBuff, outJson);

    return false;
}
