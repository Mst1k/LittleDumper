#pragma once

#include <string>
#include <json/json.h>
#include <string>

struct Pattern;
class Dataset;
struct HeaderFileManager;
struct JsonFileManager;

enum FieldStatus {
	ANALYSIS_PENDING,
	FIXUP_PENDING,
	POST_FIXUP_PENDING,
	RENDER_PENDING,
	FIELD_ENDED_SUCESSFULL
};

struct Field
{
	Dataset* pOwner;
	std::string name;
	Pattern* pPattern;
	uint64_t offset;
	bool bInterpret;
	std::vector<uintptr_t> patternResults;
	std::vector<std::string> toCombineFieldsNames;
	FieldStatus fieldStatus;
	std::string comment;

	Field(Dataset* pDataset, const Json::Value& fieldJsn);
	~Field();

	void HandleFixups();
	void PostFixups();
	bool HandleInterpret(uintptr_t& outInterptr);
	void RenderStatic();
	void RenderDynamic();
	void RenderDynamicAssign(const std::string& jsonProviderParamName, const std::string& targetMemberName, bool bObfuscate = false);
	HeaderFileManager* getHeaderFileRender();
	JsonFileManager* getJsonFileRender();
	bool NeedFindPattern();
};

