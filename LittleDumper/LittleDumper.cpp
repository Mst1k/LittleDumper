#include "LittleDumper.h"
#include "ConfigManager.h"
#include "ABIManager.h"
#include <iostream>
#include "HPPManager.h"

LittleDumper::LittleDumper()
	: pConfigManager(nullptr)
	, pHeaderFileRender(nullptr)
	, pOutputDump(nullptr)
{}

LittleDumper::~LittleDumper()
{
	if (pConfigManager) delete pConfigManager;
}

bool LittleDumper::Init()
{
	bool bOk = true;

	try {
		pConfigManager = new ConfigManager();
		pAbiManager = new ABIManager(this);
		pOutputDump = new std::ofstream(pConfigManager->dumpFileName);
		pHeaderFileRender = new HeaderFileManager(pOutputDump);
	}
	catch (const char* what)
	{
		bOk = false;
		std::cout << "Error: " << what << std::endl;
	}

	return bOk;
}

void LittleDumper::Run()
{
	pAbiManager->Analyze();
	pAbiManager->Render();
}

ConfigManager* LittleDumper::getConfigManager()
{
	return pConfigManager;
}

HeaderFileManager* LittleDumper::getHeaderFileRender()
{
	return pHeaderFileRender;
}
