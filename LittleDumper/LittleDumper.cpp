#include "LittleDumper.h"
#include "ConfigManager.h"
#include "ABIManager.h"
#include <iostream>
#include "HPPManager.h"
#ifdef _MSC_VER
#include <Windows.h>
#define GetTID() GetCurrentThreadId()
#define __SLEEP(sec) Sleep(sec*1000)
#elif __linux__ || ANDROID || __arm__
#include <pthread.h>
#include <unistd.h>
#define GetTID() pthread_self()
#define __SLEEP(sec) sleep(sec)
#endif

LittleDumper::LittleDumper()
	: pConfigManager(nullptr)
	, pHeaderFileRender(nullptr)
	, pOutputDump(nullptr)
{}

LittleDumper::~LittleDumper()
{
	if (pAbiManager) delete pAbiManager;
	if (pHeaderFileRender) delete pHeaderFileRender;
	if (pOutputDump) delete pOutputDump;
	if (pConfigManager) delete pConfigManager;
}

bool LittleDumper::Init()
{
	bool bOk = true;

	srand((unsigned)time(NULL) * GetTID());

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
	pAbiManager->RenderHPP(true);
}

ConfigManager* LittleDumper::getConfigManager()
{
	return pConfigManager;
}

HeaderFileManager* LittleDumper::getHeaderFileRender()
{
	return pHeaderFileRender;
}
