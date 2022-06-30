#pragma once

#include <json/json.h>

struct Target;
class FilesEngine;
class ABIManager;
struct HeaderFileManager;
class ICapstoneTool;

struct AbiTarget
{
	ABIManager* pOwner;
	FilesEngine* pFilesEngine;
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
	void Render();
	HeaderFileManager* getHeaderFileRender();
	bool NeedInterpreter();
	ICapstoneTool* getCapstoneTool();
};

