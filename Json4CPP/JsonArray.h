#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <vector>
#include <sstream>
#include <iostream>
#include <initializer_list>
#include <string>

namespace Json4CPP
{
  namespace Detail
  {
    class JSON_API JsonBuilder;
  }
  class JSON_API JsonObject;
  class JSON_API Json;

  class JSON_API JsonArray
  {
  private:
    friend class JsonObject;
    friend class Json;
    friend class Detail::JsonBuilder;
#pragma warning(suppress: 4251)
    std::vector<Json> _values;

    void _Dump(std::wstringstream& os, uint8_t indentation, uint64_t level) const;
  public:
    JsonArray() = default;
    JsonArray(Detail::JsonBuilder builder);
    JsonArray(std::initializer_list<Detail::JsonBuilder> builders);
    JsonArray(JsonArray const& array);

    std::wstring Dump(uint8_t indentation = 0) const;

    int64_t Size() const;
    void Resize(int64_t size);
    void Clear();
    void PushBack(Json value);
    void Insert(Json value, int64_t index);
    void Erase(int64_t index);
    Json& operator[](int const& index);

    std::vector<Json>::iterator begin();
    std::vector<Json>::iterator end();
    std::vector<Json>::const_iterator begin() const;
    std::vector<Json>::const_iterator end() const;

    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonArray const& array);
    JSON_API friend std::wistream& operator>>(std::wistream& is, JsonArray      & array);

    JSON_API friend bool operator==(JsonArray const& left, JsonArray const& right);
    JSON_API friend bool operator!=(JsonArray const& left, JsonArray const& right);
  };
}