#include "Dataset.h"
#include "Target.h"
#include "FilesEngine.h"
#include "Field.h"
#include "JsonHelper.h"
#include "Buffer.h"
#include <iostream>

Dataset::Dataset(Target* _pOwner)
	: pOwner(_pOwner)
{
	if(!(JsonHelper::File2Json(pOwner->datasetPath, outJDataset))) throw "Unable to Load Dataset Path";
	if (!outJDataset.isArray()) throw "Invalid Dataset Format";

	for (size_t i = 0; i < outJDataset.size(); i++)
	{
		try {
			Field* pField = new Field(this, outJDataset[i]);
			fieldsMap[pField->name] = pField;
			fields.push_back(pField);
		}
		catch(const char* what) {
			std::cout << "Warning: " << what << std::endl;
		}
	}
}

Dataset::~Dataset()
{
	for (const auto* pField : fields)
		delete pField;
}

bool Dataset::NeedArch()
{
	for (const auto& field : fields)
	{
		if (field->bInterpret == true)
			return true;
	}

	return false;
}

std::vector<Field*>& Dataset::getAllFields()
{
	return fields;
}

void Dataset::HandleAllFixups()
{
	for (auto* pField : fields)
		pField->HandleFixups();

	for (auto* pField : fields)
		pField->PostFixups();
}

Target* Dataset::getParent()
{
	return pOwner;
}

void Dataset::RenderStatic()
{
	for (auto* pField : fields) pField->RenderStatic();
}

void Dataset::RenderDynamic()
{
	for (auto* pField : fields) pField->RenderDynamic();
}

void Dataset::RenderDynamicAssigns(const std::string& jsonProviderParamName, const std::string& targetMemberName, bool bObfuscate)
{
	for (auto* pField : fields) pField->RenderDynamicAssign(jsonProviderParamName, targetMemberName, bObfuscate);
}

HeaderFileManager* Dataset::getHeaderFileRender()
{
	return pOwner->getHeaderFileRender();
}

bool Dataset::NeedInterpreter()
{
	for (Field* pField : fields)
	{
		if (pField->bInterpret) return true;
	}

	return false;
}

Field* Dataset::getFieldByName(const std::string& name)
{
	if (fieldsMap.find(name) != fieldsMap.end())
		return fieldsMap[name];

	return nullptr;
}

JsonFileManager* Dataset::getJsonFileRender()
{
	return pOwner->getJsonFileRender();
}
