#pragma once

#include <string>
#include <json/json.h>

struct Pattern;
class Dataset;
struct HeaderFileManager;

struct Field
{
	Dataset* pOwner;
	std::string name;
	Pattern* pPattern;
	uint64_t offset;
	bool bInterpret;
	std::vector<uintptr_t> patternResults;

	Field(Dataset* pDataset, const Json::Value& fieldJsn);
	~Field();

	void HandleFixups();
	bool HandleInterpret(uintptr_t& outInterptr);
	void Render();
	HeaderFileManager* getHeaderFileRender();
};

