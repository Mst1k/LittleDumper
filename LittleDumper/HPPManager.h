#pragma once

#include <fstream>

struct HeaderFileManager {
	std::ofstream* m_Traits;
	uintptr_t		m_TabLevel;

	HeaderFileManager(std::ofstream* traits);

	void AppendPragmaOnce();
	void AppendMacroIf();
	void AppendMacroEndIf();
	void AppendMacroDefined(const std::string& macroName);
	void AppendString(const std::string& str);
	void AppendMacroIfDefined(const std::string& macroName, bool bTerminateLine = true);
	void AppendTab();
	void AppendTab(uintptr_t count);
	void AppendConstUintVar(const std::string& name, uintptr_t value, bool bJumpNewLine = true);
	void AppendComment(const std::string& comment, bool bJumpNewLine = true);
	void AppendNextLine();
	void AppendGlobalInclude(const std::string& fileName);
	void BeginNameSpace(const std::string& name);
	void EndNameSpace(const std::string& name = "");
};