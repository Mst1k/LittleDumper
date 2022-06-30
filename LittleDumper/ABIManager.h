#pragma once

#include <vector>
#include <json/json.h>
#include <unordered_map>

class LittleDumper;
struct AbiTarget;
struct HeaderFileManager;

class ABIManager
{
private:
	LittleDumper* pOwner;
	std::vector<AbiTarget*> abiTargets;
	Json::Value abiTargetsJson;
public:
	ABIManager(LittleDumper* pDumper);
	~ABIManager();

	const std::vector<AbiTarget*>& getAbiTargets();

	void Analyze();
	void Render();
	HeaderFileManager* getHeaderFileRender();
};

