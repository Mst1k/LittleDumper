#pragma once

#include <json/json.h>

class JsonHelper
{
public:
	static bool String2Json(const std::string& fullJsonStr, Json::Value& outJson);
	static bool File2Json(const std::string& filePath, Json::Value& outJson);
};

#define JSON_IS_MEMBER(json, x) (json.isMember(x))
#define JSON_ASSERT(json, x) (!JSON_IS_MEMBER(json, x))
