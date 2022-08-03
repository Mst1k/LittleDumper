#pragma once

#include <json/json.h>
#include <string>

struct JsonFileManager
{
private:
	Json::Value m_Root;
	std::string m_OutPath;
	bool bFinalized;

public:
	JsonFileManager(const std::string& outPath);
	~JsonFileManager();

	Json::Value& GetRoot();
	bool Finalize();
};

