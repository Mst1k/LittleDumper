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
	fieldStatus = ANALYSIS_PENDING;

	if (JSON_ASSERT(fieldJsn, "name")) throw "Cant Find Field Name, Omitting Field";
	name = fieldJsn["name"].asString();

	comment = fieldJsn["comment"].asString();

	offset = 0;
	bInterpret = true;

	const auto& options = fieldJsn["options"];

	if (JSON_IS_MEMBER(fieldJsn, "value"))
	{
		patternResults.push_back(uintptr_t(fieldJsn["value"].asUInt64()));
		fieldStatus = FIELD_ENDED_SUCESSFULL;
		bInterpret = false;
		return;
	}

	if (JSON_IS_MEMBER(fieldJsn, "options"))
	{
		
		if (JSON_IS_MEMBER(options, "offset")) offset = options["offset"].asUInt64();
		if (JSON_IS_MEMBER(options, "interpret")) bInterpret = options["interpret"].asBool();
		if (JSON_IS_MEMBER(options, "combine"))
		{
			const auto& combineArr = options["combine"];

			if (combineArr.isArray())
			{
				for (size_t i = 0; i < combineArr.size(); i++) toCombineFieldsNames.push_back(combineArr[i].asString());
			}
		}
	}

	if (JSON_IS_MEMBER(fieldJsn, "pattern"))
	{
		pPattern = new Pattern(fieldJsn["pattern"].asString());
	} 
	else if (toCombineFieldsNames.size() != 0)
	{
		bInterpret = false;
		patternResults.push_back(0x0);
	}
	else throw "Cant Find Pattern, Omitting Field";
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

bool Field::NeedFindPattern()
{
	return pPattern != nullptr && fieldStatus != FIELD_ENDED_SUCESSFULL;
}

void Field::HandleFixups()
{
	for (auto& result : patternResults)
	{
		result += uintptr_t(offset);
		if (bInterpret) if (!HandleInterpret(result)) std::cout << "Unable to Handle "<< name <<" 0x" << std::hex << result << " Interpretation\n";
	}

	RemoveDuplicates(patternResults);

	fieldStatus = POST_FIXUP_PENDING;
}

void Field::PostFixups()
{

	if (patternResults.size() == 1)
	{
		for (const std::string& combiningName : toCombineFieldsNames)
		{
			Field* pCurrCombining = pOwner->getFieldByName(combiningName);

			if (pCurrCombining)
			{
				if (pCurrCombining->fieldStatus > POST_FIXUP_PENDING)
				{
					uintptr_t toCombineSize = pCurrCombining->patternResults.size();

					if (toCombineSize == 1)
					{
						patternResults[0] += pCurrCombining->patternResults[0];
					}
					else std::cout << "Warning: " << combiningName << " is not unique Result" << std::endl;
				}
				else std::cout << "Warning: " << combiningName << " is not Ready to be combined" << std::endl;
			}
			else std::cout << "Warning: Unable to Find " << combiningName << std::endl;
		}
	}

	fieldStatus = FIELD_ENDED_SUCESSFULL;
}

void Field::Render()
{
	HeaderFileManager* pHeaderRender = getHeaderFileRender();
	size_t resultCnt = patternResults.size();

	if (resultCnt == 1)
	{
		bool bAppendingComment = !comment.empty();

		pHeaderRender->AppendConstUintVar(name, patternResults[0], !bAppendingComment);
		if (bAppendingComment) pHeaderRender->AppendComment(comment);
	}
	else if (resultCnt > 1) std::cout << "Warning: " << name << " with " << resultCnt << std::endl;
	else if(resultCnt < 1) std::cout << "Warning: " << name << " Not Found" << std::endl;

	fieldStatus = FIELD_ENDED_SUCESSFULL;
}
