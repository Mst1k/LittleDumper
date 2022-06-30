#pragma once

#include <vector>
#include <json/json.h>

struct Target;
struct Field;
struct HeaderFileManager;

class Dataset
{
private:
	std::vector<Field*> fields;
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
};

