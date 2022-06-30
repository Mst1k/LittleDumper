#include "Field.h"
#include "Pattern.h"
#include "JsonHelper.h"
#include "Dataset.h"
#include "Target.h"
#include <iostream>
#include "HPPManager.h"
#include "VectorUtils.h"
#include "ICapstoneTool.h"
#include "AbiTarget.h"
#include "File.h"

Field::Field(Dataset* pDataset, const Json::Value& fieldJsn)
	: pOwner(pDataset)
	, pPattern(nullptr)
{
	if (JSON_ASSERT(fieldJsn, "name")) throw "Cant Find Field Name, Omitting Field";
	name = fieldJsn["name"].asString();
	if (JSON_ASSERT(fieldJsn, "pattern")) throw "Cant Find Pattern, Omitting Field";
	pPattern = new Pattern(fieldJsn["pattern"].asString());

	offset = 0;
	bInterpret = true;

	if (JSON_IS_MEMBER(fieldJsn, "options"))
	{
		const auto& options = fieldJsn["options"];

		if (JSON_IS_MEMBER(options, "offset")) offset = options["offset"].asUInt64();
		if (JSON_IS_MEMBER(options, "interpret")) bInterpret = options["interpret"].asBool();
	}
}

Field::~Field()
{
	if (pPattern) delete pPattern;
}

bool Field::HandleInterpret(uintptr_t& outInst)
{
	ICapstoneTool* pCapstoneTool = pOwner->getParent()->getParent()->getCapstoneTool();
	const unsigned char* pEntryFIleInMem = pOwner->getParent()->pTargetFile->pBuff->mBuff;

	return pCapstoneTool && pCapstoneTool->TryInterpret(pEntryFIleInMem, pEntryFIleInMem + outInst, outInst);
}

HeaderFileManager* Field::getHeaderFileRender()
{
	return pOwner->getHeaderFileRender();
}

void Field::HandleFixups()
{
	for (auto& result : patternResults)
	{
		result += uintptr_t(offset);
		if (bInterpret) if (!HandleInterpret(result)) std::cout << "Unable to Handle "<< name <<" 0x" << std::hex << result << " Interpretation\n";
	}

	RemoveDuplicates(patternResults);
}

void Field::Render()
{
	HeaderFileManager* pHeaderRender = getHeaderFileRender();
	size_t resultCnt = patternResults.size();

	if (resultCnt == 1) pHeaderRender->AppendConstUintVar(name, patternResults[0]);
	else if (resultCnt > 1) std::cout << "Warning: " << name << " with " << resultCnt << std::endl;
	else if(resultCnt < 1) std::cout << "Warning: " << name << " Not Found" << std::endl;
}
