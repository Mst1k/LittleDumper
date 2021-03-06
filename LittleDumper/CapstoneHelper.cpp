#include "CapstoneHelper.h"
#include <capstone/capstone.h>

uint16_t ArmCapstoneHelper::GetLValueRegType(cs_insn* pInst)
{
    return pInst->detail->arm.operands[0].reg;
}

uint16_t ArmCapstoneHelper::GetRValueRegType(cs_insn* pInst)
{
    return pInst->detail->arm.operands[1].reg;
}

uint16_t ArmCapstoneHelper::GetRegTypeById(cs_insn* pInst, uint16_t opId)
{
    return pInst->detail->arm.operands[opId].reg;
}

bool ArmCapstoneHelper::RegisterPresent(cs_insn* pInst, uint16_t reg)
{
    for (int i = 0; i != pInst->detail->arm.op_count; i++)
    {
        if (pInst->detail->arm.operands[i].reg == reg)
            return true;
    }

    return false;
}

bool ArmCapstoneHelper::HeuristicReturn(cs_insn* pInst)
{
    if (pInst->id == ARM_INS_BX)
    {
        if (RegisterPresent(pInst, ARM_REG_LR))
            return true;
    }

    if (pInst->id == ARM_INS_POP)
    {
        if (RegisterPresent(pInst, ARM_REG_PC))
            return true;
    }


    return false;
}

uintptr_t ArmCapstoneHelper::ResolvePCRelative(unsigned char* pInst, uintptr_t pcOffset)
{
    return *(uintptr_t*)(pInst + 8 + pcOffset);
}

uint16_t Arm64CapstoneHelper::GetLValueRegType(cs_insn* pInst)
{
    return pInst->detail->arm64.operands[0].reg;
}

uint16_t Arm64CapstoneHelper::GetRValueRegType(cs_insn* pInst)
{
    return pInst->detail->arm64.operands[1].reg;
}

bool Arm64CapstoneHelper::HeuristicReturn(cs_insn* pInst)
{
    if (pInst->id == ARM64_INS_RET)
        return true;

    return false;
}
