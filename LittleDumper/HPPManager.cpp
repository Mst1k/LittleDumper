#include "HPPManager.h"

HeaderFileManager::HeaderFileManager(std::ofstream* traits) :
	m_Traits(traits), m_TabLevel(0x0)
{
}

void HeaderFileManager::AppendPragmaOnce()
{
	std::ofstream& traits = *m_Traits;

	traits << "#pragma once" << std::endl << std::endl;
}

void HeaderFileManager::AppendMacroIf()
{
	std::ofstream& traits = *m_Traits;

	traits << "#if";
}

void HeaderFileManager::AppendMacroEndIf()
{
	std::ofstream& traits = *m_Traits;

	traits << "#endif" << std::endl << std::endl;
}

void HeaderFileManager::AppendMacroDefined(const std::string& macroName)
{
	std::ofstream& traits = *m_Traits;

	traits << "defined(" << macroName << ")";
}

void HeaderFileManager::AppendString(const std::string& str)
{
	std::ofstream& traits = *m_Traits;

	traits << str;
}

void HeaderFileManager::AppendMacroIfDefined(const std::string& macroName, bool bTerminateLine)
{
	AppendMacroIf(); AppendString(" "); AppendMacroDefined(macroName); if (bTerminateLine) AppendNextLine();
}

void HeaderFileManager::AppendTab()
{
	std::ofstream& traits = *m_Traits;

	traits << "\t";
}

void HeaderFileManager::AppendTab(uintptr_t count)
{
	for (size_t i = 0; i < count; i++)
		AppendTab();
}

void HeaderFileManager::AppendConstUintVar(const std::string& name, uintptr_t value)
{
	std::ofstream& traits = *m_Traits;

	AppendTab(m_TabLevel); traits << "constexpr uintptr_t " << name << " = 0x" << std::hex << value << ";" << std::endl;
}

void HeaderFileManager::AppendNextLine()
{
	std::ofstream& traits = *m_Traits;

	traits << std::endl;
}

void HeaderFileManager::AppendGlobalInclude(const std::string& fileName)
{
	std::ofstream& traits = *m_Traits;

	traits << "#include <" << fileName << ">\n";
}

void HeaderFileManager::BeginNameSpace(const std::string& name)
{
	std::ofstream& traits = *m_Traits;

	AppendTab(m_TabLevel); traits << "namespace " << name << " {" << std::endl; m_TabLevel++;
	//AppendNextLine();

}

void HeaderFileManager::EndNameSpace(const std::string& name)
{
	std::ofstream& traits = *m_Traits;

	//AppendNextLine();
	m_TabLevel--; AppendTab(m_TabLevel);  traits << "} ";

	if (!name.empty())
	{
		traits << "// " << name << " Namespace Ending" << std::endl;
	}
	else {
		traits << std::endl;
	}
}
