#include "ABIManager.h"
#include "JsonHelper.h"
#include "LittleDumper.h"
#include "ConfigManager.h"
#include "Dataset.h"
#include "HPPManager.h"
#include <iostream>
#include "AbiTarget.h"
#include "StringHelper.h"

ABIManager::ABIManager(LittleDumper* pDumper)
	: pOwner(pDumper)
{
	if (!JsonHelper::File2Json(pDumper->getConfigManager()->targetsAbisPath, abiTargetsJson)) throw "Unable to load Abis Targets";
	if (!abiTargetsJson.isArray()) throw "Invalid Abis Targets JSON Format";

	for (size_t i = 0; i < abiTargetsJson.size(); i++)
		abiTargets.push_back(new AbiTarget(this, abiTargetsJson[i]));
}

ABIManager::~ABIManager()
{
	for (auto* pAbiTarget : abiTargets)
	{
		if (pAbiTarget) delete pAbiTarget;
	}
}

const std::vector<AbiTarget*>& ABIManager::getAbiTargets()
{
	return abiTargets;
}

void ABIManager::Analyze()
{
	for (auto* pAbiTarget : abiTargets)
	{
		pAbiTarget->Analyze();
		pAbiTarget->PostAnalysis();
	}
}

void ABIManager::RenderHPP(bool bRenderDynamic)
{
	auto* pHeaderManager = pOwner->getHeaderFileRender();

	pHeaderManager->AppendPragmaOnce();
	pHeaderManager->AppendGlobalInclude("cstdint");

	pHeaderManager->BeginStruct(pOwner->getConfigManager()->StructName);

	if (bRenderDynamic) pHeaderManager->AppendMacroIfDefined("STATIC_OFFS");

	for (AbiTarget* pAbiTarget : abiTargets) pAbiTarget->RenderStatic();


	if (bRenderDynamic) {
		pHeaderManager->AppendMacroElse();

		for (AbiTarget* pAbiTarget : abiTargets) pAbiTarget->RenderDynamic();

		std::string JsonProviderParamName = "offs";

		pHeaderManager->BeginFunction("void", "Set", {
			"const Json::Value& " + JsonProviderParamName
			});

		for (AbiTarget* pAbiTarget : abiTargets) pAbiTarget->RenderDynamicAssigns(JsonProviderParamName, true);

		pHeaderManager->EndFunction();

		pHeaderManager->AppendMacroEndIf();
	}


	pHeaderManager->EndStruct(pOwner->getConfigManager()->StructName, {
		StructDeclarationInfo("g"+StringHelper::Capitalize(pOwner->getConfigManager()->StructName), true, true)
		});
}

HeaderFileManager* ABIManager::getHeaderFileRender()
{
	return pOwner->getHeaderFileRender();
}
