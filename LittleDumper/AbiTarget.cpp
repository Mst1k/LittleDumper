#include "AbiTarget.h"
#include "Target.h"
#include "JsonHelper.h"
#include "FilesEngine.h"
#include <iostream>
#include "Dataset.h"
#include "ABIManager.h"
#include "HPPManager.h"
#include "Target.h"
#include "ICapstoneTool.h"
#include "JsonFileManager.h"

std::unordered_map<std::string, cs_arch> capstoneArchMap = {
	{"arm32", CS_ARCH_ARM},
	{"arm64", CS_ARCH_ARM64}
};

std::unordered_map<std::string, cs_mode> capstoneArchModeMap = {
	{"arm", CS_MODE_ARM},
};


AbiTarget::AbiTarget(ABIManager* _pOwner, const Json::Value& abiTargetJsn)
	: pFilesEngine(new FilesEngine())
	, pOwner(_pOwner)
	, pCapstone(nullptr)
{
	macro = abiTargetJsn["macro"].asString();

	if (abiTargetJsn.isMember("targets"))
	{
		auto& targetsJsn = abiTargetJsn["targets"];

		if (targetsJsn.isArray())
		{
			for (size_t i = 0; i < targetsJsn.size(); i++)
			{
				try
				{
					targets.push_back(new Target(this, targetsJsn[i]));
				}
				catch (const char* what)
				{
					std::cout << "Warning: " << what << ", Omiting Target\n";
				}
			}
		}
		else throw "Targets is not Array";


		if (NeedInterpreter()) // pCapstone
		{
			arch = abiTargetJsn["arch"].asString();
			//archmode = abiTargetJsn["archmode"].asString();

			if (!arch.empty())
			{
				if (capstoneArchMap.find(arch) == capstoneArchMap.end()) throw "Arch Not Supported";

				//if (!archmode.empty())
				//{
				//	//if (capstoneArchModeMap.find(archmode) == capstoneArchModeMap.end()) throw "Arch Mode Not Supported";

				//}

				cs_arch archId = capstoneArchMap[arch];
				//cs_mode archModeId = capstoneArchModeMap[archmode];

				switch (archId)
				{
				case CS_ARCH_ARM:
					pCapstone = new CapstoneARMTool();
					break;

				case CS_ARCH_ARM64:
					pCapstone = new CapstoneARM64Tool();
					break;
				}

				if (!pCapstone) throw "Unknown Error, Cant Initialize Capstone Tool";
			}
			else throw "Arch Not Exist or is Empty";
		}

		pJsonFileManager = new JsonFileManager("offsets_" + macro + ".json");
	}
	else throw "Targets Member not Found";
}

AbiTarget::~AbiTarget()
{
	for (auto* pTarget : targets)
	{
		if (pTarget) delete pTarget;
	}

	if (pFilesEngine) delete pFilesEngine;
	if (pJsonFileManager) delete pJsonFileManager;
}

FilesEngine* AbiTarget::getFilesEngine()
{
	return pFilesEngine;
}

void AbiTarget::Analyze()
{
	for (auto* pTarget : targets)
		pTarget->FindPatterns();
}

void AbiTarget::PostAnalysis()
{
	for (auto* pTarget : targets)
		pTarget->getDataset()->HandleAllFixups();
}

void AbiTarget::RenderStatic()
{
	auto* pHeaderRender = getHeaderFileRender();

	bool bValidMacro = !macro.empty();
	bool bParentMultipleMacros = pOwner->getAbiTargets().size() > 1;

	if (!bValidMacro && bParentMultipleMacros)
	{
		std::cout << "Warning: ABITarget with macro empty\n";
		return;
	}

	if (bValidMacro) pHeaderRender->AppendMacroIfDefined(macro, true);
	pHeaderRender->AppendNextLine();

	for (auto* pTarget : targets) pTarget->RenderStatic();

	pHeaderRender->AppendNextLine();
	if (bValidMacro) pHeaderRender->AppendMacroEndIf();

}

void AbiTarget::RenderDynamic()
{
	auto* pHeaderRender = getHeaderFileRender();

	bool bValidMacro = !macro.empty();
	bool bParentMultipleMacros = pOwner->getAbiTargets().size() > 1;

	if (!bValidMacro && bParentMultipleMacros)
	{
		std::cout << "Warning: ABITarget with macro empty\n";
		return;
	}

	if (bValidMacro) pHeaderRender->AppendMacroIfDefined(macro, true);
	pHeaderRender->AppendNextLine();

	for (auto* pTarget : targets) pTarget->RenderDynamic();

	pHeaderRender->AppendNextLine();
	if (bValidMacro) pHeaderRender->AppendMacroEndIf();
}

void AbiTarget::RenderDynamicAssigns(const std::string& jsonProviderParamName, bool bObfuscate)
{
	auto* pHeaderRender = getHeaderFileRender();

	bool bValidMacro = !macro.empty();
	bool bParentMultipleMacros = pOwner->getAbiTargets().size() > 1;

	if (!bValidMacro && bParentMultipleMacros)
	{
		std::cout << "Warning: ABITarget with macro empty\n";
		return;
	}

	if (bValidMacro) pHeaderRender->AppendMacroIfDefined(macro, true);
	pHeaderRender->AppendNextLine();

	for (auto* pTarget : targets) pTarget->RenderDynamicAssigns(jsonProviderParamName, bObfuscate);

	pHeaderRender->AppendNextLine();
	if (bValidMacro) pHeaderRender->AppendMacroEndIf();
}

HeaderFileManager* AbiTarget::getHeaderFileRender()
{
	return pOwner->getHeaderFileRender();
}

bool AbiTarget::NeedInterpreter()
{
	for (auto* pTarget : targets)
	{
		if (pTarget->getDataset()->NeedInterpreter()) return true;
	}

	return false;
}

ICapstoneTool* AbiTarget::getCapstoneTool()
{
	return pCapstone;
}
