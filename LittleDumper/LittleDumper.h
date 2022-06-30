#pragma once

#include <fstream>

struct ConfigManager;
struct HeaderFileManager;
class ABIManager;

class LittleDumper
{
private:
	ConfigManager* pConfigManager;
	ABIManager* pAbiManager;
	HeaderFileManager* pHeaderFileRender;
	std::ofstream* pOutputDump;
public:
	LittleDumper();
	~LittleDumper();

	bool Init();
	void Run();

	ConfigManager* getConfigManager();
	HeaderFileManager* getHeaderFileRender();
};

