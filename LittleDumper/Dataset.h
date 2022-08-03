#pragma once

#include <vector>
#include <unordered_map>
#include <json/json.h>

struct Target;
struct Field;
struct HeaderFileManager;
struct JsonFileManager;

class Dataset
{
private:
	std::vector<Field*> fields;
	std::unordered_map<std::string, Field*> fieldsMap;
	Target* pOwner;
	Json::Value outJDataset;
public:
	Dataset(Target* pOwner);
	~Dataset();

	bool NeedArch();
	std::vector<Field*>& getAllFields();
	void HandleAllFixups();
	Target* getParent();
	void RenderStatic();
	void RenderDynamic();
	void RenderDynamicAssigns(const std::string& jsonProviderParamName, const std::string& targetMemberName, bool bObfuscate = false);
	HeaderFileManager* getHeaderFileRender();
	bool NeedInterpreter();
	Field* getFieldByName(const std::string& name);
	JsonFileManager* getJsonFileRender();
};

