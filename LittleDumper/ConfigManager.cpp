#include "ConfigManager.h"
#include "JsonHelper.h"
#include "StringHelper.h"

ConfigManager::ConfigManager(std::string configPath)
{
	if (!JsonHelper::File2Json(configPath, configJson)) throw "Unable to load Configs Json";

	if (!configJson.isMember("targetsabispath")) throw "Unable to get Abis targets path";
	targetsAbisPath = configJson["targetsabispath"].asString();

	bool bNamespacesNameExist = configJson.isMember("namespacename");
	bool bDumpNameExist = configJson.isMember("dumpname");

	if (!bNamespacesNameExist && !bDumpNameExist) throw "Unable to get main Namespace Name and/or Dump Name";
	
	if (bNamespacesNameExist && !bDumpNameExist)
	{
		StructName = configJson["structname"].asString();
		dumpFileName = StructName + ".hpp";
	}
	else if (!bNamespacesNameExist && bDumpNameExist)
	{
		dumpFileName = configJson["dumpname"].asString();

		if (dumpFileName.find(".hpp") != std::string::npos)
			StructName = StringHelper::Capitalize(StringHelper::Tokenize(dumpFileName, '.')[0]);
		else 
			StructName = StringHelper::Capitalize(dumpFileName);
	}
	else {
		dumpFileName = configJson["dumpname"].asString();
		StructName = configJson["structname"].asString();
	}
	
}
