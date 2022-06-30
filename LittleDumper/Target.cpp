#include "Target.h"
#include "FilesEngine.h"
#include "Dataset.h"
#include <unordered_map>
#include "JsonHelper.h"
#include "Pattern.h"
#include "Field.h"
#include "PatternEngine.h"
#include "AbiTarget.h"
#include "HPPManager.h"
#include <iostream>

Target::Target(AbiTarget* _pOwner, const Json::Value& targetRoot)
	: pParent(_pOwner)
{
	if (JSON_ASSERT(targetRoot, "path")) throw "Unable to Find Target Path";
	path = targetRoot["path"].asString();

	if (!pParent->getFilesEngine()->LoadFile(path, &pTargetFile)) throw "Unable to Load Target File";

	if (JSON_ASSERT(targetRoot, "datasetpath")) throw "Unable to Find Dataset Path";
	datasetPath = std::string(targetRoot["datasetpath"].asString());

	if (JSON_ASSERT(targetRoot, "name")) throw "Unable to get name Path";
	name = std::string(targetRoot["name"].asString());

	pDataSet = new Dataset(this);
}

Target::~Target()
{
	if (pDataSet) delete pDataSet;
}

void Target::FindPatterns()
{
	std::cout << "Finding Patterns for " << name << std::endl;

	std::vector<std::vector<unsigned char>> inputsPatterns;
	std::vector<std::vector<uintptr_t>*> outputVecs;

	for (auto* pField : pDataSet->getAllFields())
	{
		if (pField->NeedFindPattern())
		{
			inputsPatterns.push_back(pField->pPattern->getBytes());
			outputVecs.push_back((std::vector<uintptr_t>*) & (pField->patternResults));
		}
	}

	PatternEngine::FindPatterns(pTargetFile, inputsPatterns, outputVecs);
}

Dataset* Target::getDataset()
{
	return pDataSet;
}

void Target::Render()
{
	auto* pHeaderRender = getHeaderFileRender();

	pHeaderRender->BeginNameSpace(name);

	pDataSet->Render();

	pHeaderRender->EndNameSpace(name);
}

HeaderFileManager* Target::getHeaderFileRender()
{
	return pParent->getHeaderFileRender();
}

AbiTarget* Target::getParent()
{
	return pParent;
}
