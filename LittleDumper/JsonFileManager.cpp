#include "JsonFileManager.h"
#include <fstream>
#include "JsonHelper.h"

JsonFileManager::JsonFileManager(const std::string& outPath)
    : bFinalized(false)
    , m_OutPath(outPath)
{
}

JsonFileManager::~JsonFileManager()
{
    Finalize();
}

Json::Value& JsonFileManager::GetRoot()
{
    return m_Root;
}

bool JsonFileManager::Finalize()
{
    if (!bFinalized)
    {
        
        bFinalized = JsonHelper::Json2File(m_Root, m_OutPath);
        return bFinalized;
    }

    return false;
}

