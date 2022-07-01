#include "ICapstoneTool.h"
#include <iostream>
#include <conio.h>
#include "CapstoneHelper.h"

bool ICapstoneTool::TryInterpret(const unsigned char* pFileInMemEntry, const unsigned char* pInst, uintptr_t& outDisp)
{
    cs_insn* pDisasmdInst = nullptr;
    uintptr_t count = 0;
    bool result = false;

    if ((count = cs_disasm(GetCapstoneHandle(), pInst, 0x4, (uint64_t)(pInst), 0, &pDisasmdInst)) != 0 && pDisasmdInst)
    {
        result = InterpretInst(pFileInMemEntry, pDisasmdInst, outDisp);
        cs_free(pDisasmdInst, count);
    }

    return result;
}

bool ICapstoneTool::TryInterpretPCRelative(const unsigned char* pFileInMemEntry, cs_insn* pInst, uintptr_t& outDisp)
{
    cs_insn* pDisasmdInst = nullptr;
    uintptr_t count = 0;
    bool result = false;

    if ((count = cs_disasm(GetCapstoneHandle(), (uint8_t*)pInst->address, 0x50, PCRelInstAddrRebaseRoot() ? (pInst->address - uintptr_t(pFileInMemEntry)) : pInst->address, 0, &pDisasmdInst)) != 0 && pDisasmdInst)
    {
        result = InterpretPCRelativeInst(pFileInMemEntry, pDisasmdInst, pDisasmdInst + count, outDisp);
        cs_free(pDisasmdInst, count);
    }

    return result;
}

bool CapstoneARMTool::InterpretInst(const unsigned char* pFileInMemEntry, cs_insn* pInst, uintptr_t& outDisp)
{
    switch (pInst->id)
    {

    case ARM_INS_LDR:
    case ARM_INS_LDRH:
    case ARM_INS_LDRD:
    case ARM_INS_LDRB:
    case ARM_INS_LDRBT:
    case ARM_INS_LDREXB:
    {
        if (ArmCapstoneHelper::GetRValueRegType(pInst) == ARM_REG_PC) return TryInterpretPCRelative(pFileInMemEntry, pInst, outDisp);
        else outDisp = pInst->detail->arm.operands[pInst->detail->arm.op_count].mem.disp;
    } break;

    case ARM_INS_STR:
    case ARM_INS_STRH:
    case ARM_INS_STRB:
    case ARM_INS_STRD:
    case ARM_INS_STRBT:
    case ARM_INS_STREXB:
    {
        outDisp = pInst->detail->arm.operands[pInst->detail->arm.op_count].mem.disp;
    } break;

    case ARM_INS_VLDR:
    case ARM_INS_VSTR:
    {
        outDisp = pInst->detail->arm.operands[pInst->detail->arm.op_count - 1].mem.disp;
    } break;

    case ARM_INS_ADD:
    {
        outDisp = pInst->detail->arm.operands[pInst->detail->arm.op_count - 1].imm;
    }break;

    default:
        return false;
    }

    return true;
}

bool CapstoneARMTool::InterpretPCRelativeInst(const unsigned char* pFileInMemEntry, cs_insn* pInstBegin, cs_insn* pInstEnd, uintptr_t& outDisp)
{

    uint16_t regPcRelOffHolderType = ArmCapstoneHelper::GetLValueRegType(pInstBegin);
    uintptr_t targetPcRelOff = ArmCapstoneHelper::ResolvePCRelative((unsigned char*)pInstBegin->address, pInstBegin->detail->arm.operands[pInstBegin->detail->arm.op_count].mem.disp);

    for (auto* pCurrInst = pInstBegin + 1; pCurrInst < pInstEnd; pCurrInst++)
    {

        switch(pCurrInst->id) {

        case ARM_INS_LDR:
        case ARM_INS_STR:
        {
            if (pCurrInst->detail->arm.operands[1].mem.base == ARM_REG_PC &&
                pCurrInst->detail->arm.operands[1].mem.index == regPcRelOffHolderType)
            {
                outDisp = (uintptr_t(pCurrInst->address) + 0x8 + targetPcRelOff) - uintptr_t(pFileInMemEntry);
                return true;
            }
        }break;

        case ARM_INS_ADD:
        {
            if ((pCurrInst->detail->arm.operands[1].reg == ARM_REG_PC &&
                pCurrInst->detail->arm.operands[2].reg == regPcRelOffHolderType) || 
                (pCurrInst->detail->arm.operands[2].reg == ARM_REG_PC &&
                    pCurrInst->detail->arm.operands[1].reg == regPcRelOffHolderType))
            {
                outDisp = (uintptr_t(pCurrInst->address) + 0x8 + targetPcRelOff) - uintptr_t(pFileInMemEntry);
                return true;
            }
        }break;

        }

        
    }

    return false;
}

CapstoneARMTool::CapstoneARMTool()
    : ICapstoneTool(CS_ARCH_ARM, CS_MODE_ARM)
{}

CapstoneARMTool::~CapstoneARMTool()
{
}

ICapstoneTool::ICapstoneTool(cs_arch arch, cs_mode archMode)
{
    //opening the handle of the capstone disasm
    if (cs_open(arch, archMode, &m_CapstoneDisasm) != CS_ERR_OK)
        throw "Failed Opening Handle of Disassembler";

    // setting detailed mode on
    if (cs_option(m_CapstoneDisasm, CS_OPT_DETAIL, true) != CS_ERR_OK)
    {
        cs_close(&m_CapstoneDisasm);

        throw "Failed enabling Detailed Options";
    }
}

ICapstoneTool::~ICapstoneTool()
{
    if (m_CapstoneDisasm)
        cs_close(&m_CapstoneDisasm);
}

csh ICapstoneTool::GetCapstoneHandle()
{
    return m_CapstoneDisasm;
}

bool CapstoneARM64Tool::InterpretInst(const unsigned char* pFileInMemEntry, cs_insn* pInst, uintptr_t& outDisp)
{
    switch(pInst->id) {

    case ARM64_INS_LDR:
    case ARM64_INS_LDRH:
    case ARM64_INS_LDRB:
    {
        if (ArmCapstoneHelper::GetRValueRegType(pInst) == ARM_REG_PC) return false;
        else outDisp = pInst->detail->arm64.operands[pInst->detail->arm64.op_count].mem.disp;
    } break;

    case ARM64_INS_STR:
    case ARM64_INS_STRH:
    case ARM64_INS_STRB:
    {
        outDisp = pInst->detail->arm64.operands[pInst->detail->arm64.op_count].mem.disp;
    } break;

    case ARM64_INS_ADD:
    {
        outDisp = uintptr_t(pInst->detail->arm64.operands[pInst->detail->arm64.op_count - 1].imm);
    }break;

    case ARM64_INS_LDP:
    {
        outDisp = uintptr_t(pInst->detail->arm64.operands[pInst->detail->arm64.op_count].mem.disp);
    }break;

    case ARM64_INS_ADRP:
    {
        return TryInterpretPCRelative(pFileInMemEntry, pInst, outDisp);
    }break;

    default:
        return false;
    }
    
    return true;
}

bool CapstoneARM64Tool::InterpretPCRelativeInst(const unsigned char* pFileInMemEntry, cs_insn* pInstBegin, cs_insn* pInstEnd, uintptr_t& outDisp)
{
    uint16_t adrpRegTargetType = Arm64CapstoneHelper::GetLValueRegType(pInstBegin);
    uintptr_t pageEntry = uintptr_t(pInstBegin->detail->arm64.operands[pInstBegin->detail->arm64.op_count - 1].imm);

    for (auto* pCurrInst = pInstBegin + 1; pCurrInst < pInstEnd; pCurrInst++)
    {
        switch (pCurrInst->id) {

        case ARM64_INS_LDR:
        case ARM64_INS_STR:
        {
            if (pCurrInst->detail->arm64.operands[1].mem.base == adrpRegTargetType)
            {
                intptr_t pageOffset = pCurrInst->detail->arm64.operands[pCurrInst->detail->arm64.op_count].mem.disp;
                outDisp = uintptr_t(pageEntry + pageOffset);
                return true;
            }
        }break;

        case ARM64_INS_ADD:
        {
            if (pCurrInst->detail->arm64.operands[1].reg == adrpRegTargetType)
            {
                outDisp = (pageEntry + intptr_t(pCurrInst->detail->arm64.operands[2].imm));
                return true;
            }
        }break;

        }
    }

    return false;
}

CapstoneARM64Tool::CapstoneARM64Tool()
    : ICapstoneTool(CS_ARCH_ARM64, CS_MODE_ARM)
{}
