#include "LittleDumper.h"
#include <iostream>
#include "HPPManager.h"

int main()
{
	LittleDumper* littleDumper = new LittleDumper();

	if (littleDumper->Init())
	{
		littleDumper->Run();
		delete littleDumper;
	}
	else return 1;

	std::cout << "Dump Done!\n";

	return 0;
}

/*int main()
{
	std::ofstream pStream("hola.hpp");
	HeaderFileManager pFileManager(&pStream);

	pFileManager.BeginStruct("S");

	pFileManager.AppendUintVar("U1");
	pFileManager.AppendUintVar("U2", true, true, 0xDEAD);

	pFileManager.EndStruct("S", {
		StructDeclarationInfo("gS1"),
		StructDeclarationInfo("gS2", true),
		StructDeclarationInfo("gS3", true, true),
		StructDeclarationInfo("gS4", false, true),
		StructDeclarationInfo("gS5", true, false)
	});



	pFileManager.BeginFunction("void", "Set", {
		"const Json::Value& offs"
		});

	pFileManager.EndFunction();




}*/