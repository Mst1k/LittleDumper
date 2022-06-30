#pragma once

#include <vector>
#include <unordered_map>
#include <json/json.h>

struct Target;
struct Field;
struct HeaderFileManager;

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
	void Render();
	HeaderFileManager* getHeaderFileRender();
	bool NeedInterpreter();
	Field* getFieldByName(const std::string& name);
};

