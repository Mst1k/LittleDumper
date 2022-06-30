#include "PatternEngine.h"
#include "File.h"
#include "Pattern.h"
#include "Buffer.h"

bool PatternEngine::CompareMem(const unsigned char* memChunk, const unsigned char* mask, const size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		if (mask[i] != memChunk[i] && mask[i] != '?')
			return false;
	}

	return true;
}

void PatternEngine::FindPattern(const File* pFile, const Pattern& patternMask, std::vector<uintptr_t>& outResults)
{
	Buffer* pBuff = pFile->pBuff;
	const auto patternMaskBuff = patternMask.getBytes();

	if (pBuff) FindPattern(pBuff->mBuff, pBuff->mBuff + pBuff->mSize, patternMaskBuff.data(), patternMaskBuff.size(), outResults);
}

void PatternEngine::FindPattern(const unsigned char* pStartAddr, const unsigned char* pEndAddr, const unsigned char* pMask, size_t maskLen, std::vector<uintptr_t>& outResults)
{
	for (const unsigned char* pCurr = pStartAddr; (pCurr + maskLen) < pEndAddr; pCurr++)
	{
		if (CompareMem(pCurr, pMask, maskLen))
			outResults.push_back((uintptr_t)(pCurr - pStartAddr));
	}
}

void PatternEngine::FindPatterns(const File* pFile, const std::vector<std::vector<unsigned char>>& vecOfPatterns, const std::vector<std::vector<uintptr_t>*>& vecOfOutputs)
{
	Buffer* pBuff = pFile->pBuff;

	if (pBuff) FindPatterns(pBuff->mBuff, pBuff->mBuff + pBuff->mSize, vecOfPatterns, vecOfOutputs);
}

void PatternEngine::FindPatterns(const unsigned char* pStartAddr, const unsigned char* pEndAddr, const std::vector<std::vector<unsigned char>>& vecOfPatterns, const std::vector<std::vector<uintptr_t>*>& vecOfOutputs)
{
	uintptr_t longestPatternSize = 1;
	for (const auto& currPatern : vecOfPatterns)
	{
		if (currPatern.size() > longestPatternSize)
			longestPatternSize = currPatern.size();
	}

	for (const unsigned char* pCurr = pStartAddr; (pCurr + longestPatternSize) <= pEndAddr; pCurr++)
	{
		for (size_t i = 0; i < vecOfPatterns.size(); i++)
		{
			const auto& currPattern = vecOfPatterns[i];
			auto* currResultVec = vecOfOutputs[i];

			if (CompareMem(pCurr, currPattern.data(), currPattern.size()))
				currResultVec->push_back((uintptr_t)(pCurr - pStartAddr));
		}
	}
}
