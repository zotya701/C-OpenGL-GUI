#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonType.h"
#include "Value.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonLinter.h"
#include "JsonTokenType.h"

#include <variant>
#include <string>
#include <vector>
#include <utility>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <iterator>

namespace Json4CPP
{
  namespace Test
  {
    class JsonTest;
  }
  namespace Detail
  {
    class JSON_API JsonBuilder;
  }
  class JSON_API JsonObject;
  class JSON_API JsonArray;

  class JSON_API Json
  {
  private:
    friend class ::Json4CPP::Test::JsonTest;
    friend class JsonObject;
    friend class JsonArray;
    friend class Detail::JsonBuilder;
#pragma warning(suppress: 4251)
    Detail::VALUE _value;

    static Json                       Read (                  std::deque<Detail::TOKEN>& tokens);
    static std::deque<Detail::TOKEN>& Write(Json const& json, std::deque<Detail::TOKEN>& tokens);
  public:
    Json();
    Json(Detail::JsonBuilder value);
    Json(std::initializer_list<Detail::JsonBuilder> values);
    Json(Json const& json);

    Json(std::nullptr_t value);
    Json(const wchar_t* value);
    Json(std::wstring   value);
    Json(bool           value);
    Json(char           value);
    Json(int8_t         value);
    Json(uint8_t        value);
    Json(int16_t        value);
    Json(uint16_t       value);
    Json(int32_t        value);
    Json(uint32_t       value);
    Json(int64_t        value);
    Json(uint64_t       value);
    Json(float          value);
    Json(double         value);
    Json(JsonObject     value);
    Json(JsonArray      value);

    JsonType Type() const;
    bool Is(JsonType type) const;

    std::wstring Dump(uint8_t indentSize = 0, wchar_t indentChar = L' ') const;

    static Json Parse(std::string const& string);
    static Json Parse(std::wstring const& wstring);
    static Json Parse(std::u32string const& u32string);

    template<typename It,
      std::enable_if_t<std::is_integral_v<typename std::iterator_traits<It>::value_type> &&
                       ((sizeof(typename std::iterator_traits<It>::value_type)) == 1 ||
                        (sizeof(typename std::iterator_traits<It>::value_type)) == 2 ||
                        (sizeof(typename std::iterator_traits<It>::value_type)) == 4)>* = nullptr>
    static Json Parse(It begin, It end)
    {
      if constexpr (sizeof(typename std::iterator_traits<It>::value_type) == 1)
      {
        return Json::Parse(std::string(begin, end));
      }
      else if constexpr (sizeof(typename std::iterator_traits<It>::value_type) == 2)
      {
        return Json::Parse(std::wstring(begin, end));
      }
      else if constexpr (sizeof(typename std::iterator_traits<It>::value_type) == 4)
      {
        return Json::Parse(std::u32string(begin, end));
      }
    }

    template<typename Iterable,
      std::enable_if_t<std::is_integral_v<typename std::iterator_traits<decltype(std::declval<Iterable>().begin())>::value_type> &&         // Has begin() and it is integral type
                       std::is_integral_v<typename std::iterator_traits<decltype(std::declval<Iterable>().end  ())>::value_type> &&         // Has end() and it is integral type
                       ((sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().begin())>::value_type)) == 1 &&             // The integral type's size is 1 byte
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().end  ())>::value_type)) == 1 ||             // Or
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().begin())>::value_type)) == 2 &&             // The integral type's size is 2 byte
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().end  ())>::value_type)) == 2 ||             // Or
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().begin())>::value_type)) == 4 &&             // The integral type's size is 4 byte
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().end  ())>::value_type)) == 4)>* = nullptr>
    static Json Parse(Iterable values)
    {
      return Parse(values.begin(), values.end());
    }

    static Json Read(std::filesystem::path filePath);
    void Write(std::filesystem::path filePath) const;
    void Write(std::filesystem::path filePath, uint8_t indentSize) const;
    void Write(std::filesystem::path filePath, wchar_t indentChar) const;
    void Write(std::filesystem::path filePath, uint8_t indentSize, wchar_t indentChar) const;

    template<typename T>
    auto const& Get() const
    {
      return std::get<T>(_value);
    }

    template<typename T>
    auto GetIf()
    {
      return std::get_if<T>(&_value);
    }

    template<typename T>
    static std::wstring Stringify(T const& value)
    {
      if constexpr (std::is_constructible_v<T, Json&>)
      {
        return Json(value).Dump();
      }
      else
      {
        std::wostringstream os;
        os << value;
        return os.str();
      }
    }

    int64_t Size() const;
    void Resize(int64_t size);
    void Clear();
    void PushBack(Json value);
    bool Insert(std::pair<KEY, Json> pair);
    void Insert(int64_t index, Json value);
    void Erase(KEY key);
    void Erase(int64_t index);
    std::vector<KEY> Keys() const;
    Json      & operator[](KEY const& key);
    Json const& operator[](KEY const& key) const;
    Json      & operator[](int64_t const& index);
    Json const& operator[](int64_t const& index) const;
    Json      & At(KEY     const& key);
    Json const& At(KEY     const& key) const;
    Json      & At(int64_t const& index);
    Json const& At(int64_t const& index) const;

    explicit operator std::nullptr_t () const;
    explicit operator std::wstring   () const;
    explicit operator bool           () const;
    explicit operator char           () const;
    explicit operator int8_t         () const;
    explicit operator uint8_t        () const;
    explicit operator int16_t        () const;
    explicit operator uint16_t       () const;
    explicit operator int32_t        () const;
    explicit operator uint32_t       () const;
    explicit operator int64_t        () const;
    explicit operator uint64_t       () const;
    explicit operator float          () const;
    explicit operator double         () const;
    explicit operator JsonObject     () const;
    explicit operator JsonArray      () const;

    Json& operator= (std::nullptr_t      value);
    Json& operator= (const wchar_t*      value);
    Json& operator= (std::wstring        value);
    Json& operator= (bool                value);
    Json& operator= (char                value);
    Json& operator= (int8_t              value);
    Json& operator= (uint8_t             value);
    Json& operator= (int16_t             value);
    Json& operator= (uint16_t            value);
    Json& operator= (int32_t             value);
    Json& operator= (uint32_t            value);
    Json& operator= (int64_t             value);
    Json& operator= (uint64_t            value);
    Json& operator= (float               value);
    Json& operator= (double              value);
    Json& operator= (Json                value);
    Json& operator= (JsonObject          value);
    Json& operator= (JsonArray           value);
    Json& operator= (Detail::JsonBuilder value);
    Json& operator= (std::initializer_list<Detail::JsonBuilder> values);

    template<typename T>
    Json& operator=(T const& value)
    {
      Detail::TypeDebug<T>(); // Type T is not supported. See output error C2027 for the exact type.
    }

    JSON_API friend Json operator""_Json(const wchar_t* value, std::size_t size);

    JSON_API friend std::wostream& operator<<(std::wostream& os, Json const& json);
    JSON_API friend std::wistream& operator>>(std::wistream& is, Json      & json);

    JSON_API friend bool   operator==(Json const& left, Json const& right);
    JSON_API friend bool   operator!=(Json const& left, Json const& right);
    JSON_API friend bool   operator< (Json const& left, Json const& right);
    JSON_API friend bool   operator<=(Json const& left, Json const& right);
    JSON_API friend bool   operator> (Json const& left, Json const& right);
    JSON_API friend bool   operator>=(Json const& left, Json const& right);
    JSON_API friend Json   operator+ (Json const& left, Json const& right);
    JSON_API friend Json&  operator+=(Json      & left, Json const& right);
    JSON_API friend Json   operator- (Json const& left, Json const& right);
    JSON_API friend Json&  operator-=(Json      & left, Json const& right);
    JSON_API friend Json   operator* (Json const& left, Json const& right);
    JSON_API friend Json&  operator*=(Json      & left, Json const& right);
    JSON_API friend Json   operator/ (Json const& left, Json const& right);
    JSON_API friend Json&  operator/=(Json      & left, Json const& right);
    JSON_API friend Json   operator% (Json const& left, Json const& right);
    JSON_API friend Json&  operator%=(Json      & left, Json const& right);
    JSON_API friend Json   operator- (Json const& value                  );
    JSON_API friend bool   operator! (Json const& value                  );
    JSON_API friend Json&  operator++(Json      & value                  );
    JSON_API friend Json   operator++(Json      & value, int             );
    JSON_API friend Json&  operator--(Json      & value                  );
    JSON_API friend Json   operator--(Json      & value, int             );
    JSON_API friend bool   operator&&(Json const& left, Json const& right);
    JSON_API friend bool   operator||(Json const& left, Json const& right);
  };
}