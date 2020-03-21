#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <sstream>
#include <string>

namespace Json4CPP::Detail
{
  enum class JSON_API JsonToken
  {
    Undefined,
    Null,
    String,
    Boolean,
    Number,
    PropertyName,
    StartObject,
    EndObject,
    StartArray,
    EndArray,
  };

  JSON_API std::wostream& operator<<(std::wostream& os, JsonToken const& token);
  JSON_API std::wstring Dump(JsonToken const& token);
}