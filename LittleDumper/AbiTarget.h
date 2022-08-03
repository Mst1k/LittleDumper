#pragma once

#include <json/json.h>

struct Target;
class FilesEngine;
class ABIManager;
struct HeaderFileManager;
class ICapstoneTool;
struct JsonFileManager;

struct AbiTarget
{
	ABIManager* pOwner;
	FilesEngine* pFilesEngine;
	JsonFileManager* pJsonFileManager;
	ICapstoneTool* pCapstone;
	std::vector<Target*> targets;
	std::string macro;
	std::string arch;
	std::string archmode;

	AbiTarget(ABIManager* _pOwner, const Json::Value& abiTargetJsn);
	~AbiTarget();

	FilesEngine* getFilesEngine();

	void Analyze();
	void PostAnalysis();
	void RenderStatic();
	void RenderDynamic();
	void RenderDynamicAssigns(const std::string& jsonProviderParamName, bool bObfuscate = false);
	HeaderFileManager* getHeaderFileRender();
	bool NeedInterpreter();
	ICapstoneTool* getCapstoneTool();
};

