#include "Pattern.h"
#include "ByteHelper.h"

Pattern::Pattern()
	: cachedStr("")
{
}

Pattern::Pattern(const std::string& pattern)
{
	ByteHelper::Pattern2Bytes(pattern, aob);
}

void Pattern::CacheStr()
{
	const std::string qMark = std::string("?");

	for (size_t i = 0; i < aob.size(); i++)
	{
		const auto& pb = aob[i];

		cachedStr += i == 0 ? "" : " ";

		if (pb.byteType == ByteType::BYTE) cachedStr += ByteHelper::Byte2String(pb.byte);
		else cachedStr += qMark;
	}
}

void Pattern::InvalidateCache()
{
	cachedStr = "";
}

std::string Pattern::toString()
{
	if (cachedStr.empty()) CacheStr();

	return cachedStr;
}

void Pattern::AddByte(unsigned char byte, bool bIsWildCard)
{
	PatternByte pB{};

	pB.byte = byte;
	pB.byteType = bIsWildCard ? ByteType::WILDCARD : ByteType::BYTE;

	aob.push_back(pB);

	InvalidateCache();
}

void Pattern::AddWildCard()
{
	AddByte(0x0, true);
}

std::vector<unsigned char> Pattern::getBytes() const
{
	std::vector<unsigned char> bytes;

	for (const PatternByte& uc : aob)
		bytes.push_back(uc.byteType == ByteType::BYTE ? uc.byte : '?');

	return bytes;
}
