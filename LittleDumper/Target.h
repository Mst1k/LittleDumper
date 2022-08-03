#pragma once

#include <string>
#include <json/json.h>
#include <unordered_map>
#include <capstone/capstone.h>

class Dataset;
struct File;
struct AbiTarget;
struct HeaderFileManager;
struct JsonFileManager;

struct Target
{
	AbiTarget* pParent;
	std::string name;
	std::string path;
	std::string datasetPath;
	
	File* pTargetFile;
	Dataset* pDataSet;

	Target(AbiTarget* pOwner, const Json::Value& targetRoot);
	~Target();

	void FindPatterns();
	Dataset* getDataset();
	void RenderStatic();
	void RenderDynamic();
	void RenderDynamicAssigns(const std::string& jsonProviderParamName, bool bObfuscate = false);
	HeaderFileManager* getHeaderFileRender();
	AbiTarget* getParent();
	JsonFileManager* getJsonFileRender();
};

