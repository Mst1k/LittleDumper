#include "ByteHelper.h"
#include <sstream>

std::string ByteHelper::Byte2String(unsigned char byte)
{
    char bytePlaceHolder[3]{};

    sprintf_s(bytePlaceHolder, 3, "%02X", byte);

    return std::string(bytePlaceHolder);
}

void ByteHelper::Pattern2Bytes(const std::string& pattern, std::vector<unsigned char>& result)
{
	std::stringstream ss(pattern);
	std::string str; ss >> str;

	do
	{
		if (str.find("?") != std::string::npos)
			result.push_back('?');
		else
			result.push_back((unsigned char)strtoull(str.c_str(), nullptr, 16));

		if (ss.eof())
			break;
	} while (ss >> str);
}

void ByteHelper::Pattern2Bytes(const std::string& pattern, std::vector<PatternByte>& result)
{
	std::stringstream ss(pattern);
	std::string str; ss >> str;

	do
	{
		PatternByte pb{};

		if (str.find("?") != std::string::npos)
		{
			pb.byte = 0x0;
			pb.byteType = ByteType::WILDCARD;
		}
		else
		{
			pb.byte = (unsigned char)strtoull(str.c_str(), nullptr, 16);
			pb.byteType = ByteType::BYTE;
		}

		result.push_back(pb);

		if (ss.eof())
			break;
	} while (ss >> str);
}
