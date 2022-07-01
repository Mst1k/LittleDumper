#pragma once

#include <vector>
#include <string>
#include <capstone\capstone.h>

class ICapstoneTool
{
private:
	csh	m_CapstoneDisasm;
public:
	ICapstoneTool(cs_arch arch, cs_mode archMode);
	~ICapstoneTool();

	csh GetCapstoneHandle();
	bool TryInterpret(const unsigned char* pFileInMemEntry, const unsigned char* pInst, uintptr_t& outDisp);
	bool TryInterpretPCRelative(const unsigned char* pFileInMemEntry, cs_insn* pInst, uintptr_t& outDisp);
	virtual bool InterpretInst(const unsigned char* pFileInMemEntry, cs_insn* pInst, uintptr_t& outDisp) = 0;
	virtual bool InterpretPCRelativeInst(const unsigned char* pFileInMemEntry, cs_insn* pInst, cs_insn* pInstEnd, uintptr_t& outDisp) = 0;
	virtual bool PCRelInstAddrRebaseRoot() = 0;
};

class CapstoneARMTool : public ICapstoneTool {
public:
	bool InterpretInst(const unsigned char* pFileInMemEntry, cs_insn* pInst, uintptr_t& outDisp) override;
	bool InterpretPCRelativeInst(const unsigned char* pFileInMemEntry, cs_insn* pInst, cs_insn* pInstEnd, uintptr_t& outDisp) override;
	bool PCRelInstAddrRebaseRoot() override { return false; };
	CapstoneARMTool();
	~CapstoneARMTool();
};

class CapstoneARM64Tool : public ICapstoneTool {
public:
	bool InterpretInst(const unsigned char* pFileInMemEntry, cs_insn* pInst, uintptr_t& outDisp) override;
	bool InterpretPCRelativeInst(const unsigned char* pFileInMemEntry, cs_insn* pInst, cs_insn* pInstEnd, uintptr_t& outDisp) override;
	bool PCRelInstAddrRebaseRoot() override { return true; };
	CapstoneARM64Tool();
	~CapstoneARM64Tool();
};


