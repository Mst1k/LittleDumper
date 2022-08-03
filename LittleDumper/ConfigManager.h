#pragma once

#include <string>
#include <json/json.h>

struct ConfigManager
{
	std::string dumpFileName;
	std::string StructName;
	std::string targetsAbisPath;
	Json::Value configJson;

	ConfigManager(std::string configPath = "config.json");
};

