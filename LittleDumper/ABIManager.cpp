#include "ABIManager.h"
#include "JsonHelper.h"
#include "LittleDumper.h"
#include "ConfigManager.h"
#include "Dataset.h"
#include "HPPManager.h"
#include <iostream>
#include "AbiTarget.h"

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

void ABIManager::Render()
{
	pOwner->getHeaderFileRender()->AppendPragmaOnce();
	pOwner->getHeaderFileRender()->AppendGlobalInclude("cstdint");

	pOwner->getHeaderFileRender()->BeginNameSpace(pOwner->getConfigManager()->namespaceName);

	for (AbiTarget* pAbiTarget : abiTargets) pAbiTarget->Render();

	pOwner->getHeaderFileRender()->EndNameSpace(pOwner->getConfigManager()->namespaceName);
}

HeaderFileManager* ABIManager::getHeaderFileRender()
{
	return pOwner->getHeaderFileRender();
}
