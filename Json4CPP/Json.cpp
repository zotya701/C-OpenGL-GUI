#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace Json4CPP::Detail;

namespace Json4CPP
{
  Json operator""_json(const wchar_t* value, std::size_t size)
  {
    return Value::ParseJson(wstring(value));
  }

  void Json::Dump(wstringstream& os, int indentation, int level) const
  {
    switch (Value::GetType(_value))
    {
    case JsonType::Object: get<JsonObject>(_value).Dump(os, indentation, level); break;
    case JsonType::Array : get<JsonArray >(_value).Dump(os, indentation, level); break;
    default: Value::Write(os, _value); break;
    }
  }

  Json::Json()
  {
    _value = nullptr;
  }

  Json::Json(JsonBuilder value)
  {
    switch (value.Type())
    {
    case JsonBuilderType::Object: _value = JsonObject(value); break;
    case JsonBuilderType::Array : _value = JsonArray (value); break;
    default:
      visit(Overload{
        [&](auto arg) { _value = arg; },
        [&](vector<JsonBuilder> const& arg)
        {
          switch (value.Type())
          {
          case JsonBuilderType::Empty: case JsonBuilderType::Object: _value = JsonObject(arg); break;
          case JsonBuilderType::Array: case JsonBuilderType::Pair  : _value = JsonArray (arg); break;
          default: throw exception("Should not be possible!");
          }
        }
      }, value._value);
      break;
    }
  }

  Json::Json(initializer_list<JsonBuilder> values) : Json(JsonBuilder(values)) {}

  Json::Json(Json const& json)
  {
    _value = json._value;
  }

  Json::Json(nullptr_t      value) { _value = value;          }
  Json::Json(const wchar_t* value) { _value = wstring(value); }
  Json::Json(wstring        value) { _value = value;          }
  Json::Json(bool           value) { _value = value;          }
  Json::Json(char           value) { _value = double(value);  }
  Json::Json(int8_t         value) { _value = double(value);  }
  Json::Json(uint8_t        value) { _value = double(value);  }
  Json::Json(int16_t        value) { _value = double(value);  }
  Json::Json(uint16_t       value) { _value = double(value);  }
  Json::Json(int32_t        value) { _value = double(value);  }
  Json::Json(uint32_t       value) { _value = double(value);  }
  Json::Json(int64_t        value) { _value = double(value);  }
  Json::Json(uint64_t       value) { _value = double(value);  }
  Json::Json(float          value) { _value = double(value);  }
  Json::Json(double         value) { _value = value;          }
  Json::Json(JsonObject     value) { _value = value;          }
  Json::Json(JsonArray      value) { _value = value;          }

  JsonType Json::Type() const
  {
    return Value::GetType(_value);
  }

  bool Json::Is(JsonType type) const
  {
    return Type() == type;
  }

  wstring Json::Dump(int indentation) const
  {
    wstringstream buffer;
    Dump(buffer, indentation, 0);
    return buffer.str();
  }

  Json Json::Read(path filePath)
  {
    return Value::ParseJson(wfstream(filePath, wfstream::in));
  }

  void Json::Write(path filePath) const
  {
    wfstream(filePath, wfstream::out) << *this;
  }

  void Json::AddPair(std::pair<KEY, Json> pair)
  {
    switch (Type())
    {
    case JsonType::Null:
      _value = JsonObject();
    case JsonType::Object:
      return get<JsonObject>(_value).AddPair(pair);
    default:
      throw exception("Add(pair<KEY, Json>) is only defined for JsonObject!");
    }
  }

  void Json::AddValue(Json value)
  {
    switch (Type())
    {
    case JsonType::Null:
      _value = JsonArray();
    case JsonType::Array:
      return get<JsonArray>(_value).AddValue(value);
    default:
      throw exception("Add(Json) is only defined for JsonArray!");
    }
  }

#pragma region Implicit conversion operators
#pragma warning(push)
#pragma warning(disable : 4244)
  Json::operator std::nullptr_t()
  {
    if (Is(JsonType::Null)) return nullptr;
    throw exception("Invalid conversion!");
  }

  Json::operator std::wstring()
  {
    if (Is(JsonType::String)) return Get<std::wstring>();
    throw exception("Invalid conversion!");
  }

  Json::operator bool()
  {
    switch (Type())
    {
    case JsonType::Null: return false;
    case JsonType::Boolean: return Get<bool>();
    default: return true;
    }
  }

  Json::operator char()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int8_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint8_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int16_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint16_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int32_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint32_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int64_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint64_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator float()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator double()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator JsonObject()
  {
    if (Is(JsonType::Object)) return Get<JsonObject>();
    throw exception("Invalid conversion!");
  }

  Json::operator JsonArray()
  {
    if (Is(JsonType::Array)) return Get<JsonArray>();
    throw exception("Invalid conversion!");
  }

  Json::operator std::nullptr_t() const
  {
    if (Is(JsonType::Null)) return nullptr;
    throw exception("Invalid conversion!");
  }

  Json::operator std::wstring() const
  {
    if (Is(JsonType::String)) return Get<std::wstring>();
    throw exception("Invalid conversion!");
  }

  Json::operator bool() const
  {
    switch (Type())
    {
    case JsonType::Null: return false;
    case JsonType::Boolean: return Get<bool>();
    default: return true;
    }
  }

  Json::operator char() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int8_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint8_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int16_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint16_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int32_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint32_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int64_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint64_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator float() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator double() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator JsonObject() const
  {
    if (Is(JsonType::Object)) return Get<JsonObject>();
    throw exception("Invalid conversion!");
  }

  Json::operator JsonArray() const
  {
    if (Is(JsonType::Array)) return Get<JsonArray>();
    throw exception("Invalid conversion!");
  }

  Json::operator std::nullptr_t&()
  {
    return Get<std::nullptr_t>();
  }

  Json::operator std::wstring&()
  {
    return Get<std::wstring>();
  }

  Json::operator bool&()
  {
    return Get<bool>();
  }

  Json::operator double&()
  {
    return Get<double>();
  }

  Json::operator JsonObject&()
  {
    return Get<JsonObject>();
  }

  Json::operator JsonArray&()
  {
    return Get<JsonArray>();
  }
#pragma warning(pop)
#pragma endregion

  Json& Json::operator[](KEY const& key)
  {
    switch (Type())
    {
    case JsonType::Null:
      _value = JsonObject();
    case JsonType::Object:
      return get<JsonObject>(_value)[key];
    default:
      throw exception("Operator[KEY] is only defined for JsonObject!");
    }
  }

  Json& Json::operator[](const wchar_t* key)
  {
    return (*this)[wstring(key)];
  }

  Json& Json::operator[](int const& index)
  {
    switch (Type())
    {
    case JsonType::Array: return get<JsonArray>(_value)[index];
    default: throw exception("Operator[int] is only defined for JsonArray!");
    }
  }

  Json& Json::operator=(std::nullptr_t                      value) { _value = value;               return *this; }
  Json& Json::operator=(const wchar_t*                      value) { _value = std::wstring(value); return *this; }
  Json& Json::operator=(std::wstring                        value) { _value = value;               return *this; }
  Json& Json::operator=(bool                                value) { _value = value;               return *this; }
  Json& Json::operator=(char                                value) { _value = double(value);       return *this; }
  Json& Json::operator=(int8_t                              value) { _value = double(value);       return *this; }
  Json& Json::operator=(uint8_t                             value) { _value = double(value);       return *this; }
  Json& Json::operator=(int16_t                             value) { _value = double(value);       return *this; }
  Json& Json::operator=(uint16_t                            value) { _value = double(value);       return *this; }
  Json& Json::operator=(int32_t                             value) { _value = double(value);       return *this; }
  Json& Json::operator=(uint32_t                            value) { _value = double(value);       return *this; }
  Json& Json::operator=(int64_t                             value) { _value = double(value);       return *this; }
  Json& Json::operator=(uint64_t                            value) { _value = double(value);       return *this; }
  Json& Json::operator=(float                               value) { _value = double(value);       return *this; }
  Json& Json::operator=(double                              value) { _value = value;               return *this; }
  Json& Json::operator=(Json                                value) { _value = value._value;        return *this; }
  Json& Json::operator=(JsonObject                          value) { _value = value;               return *this; }
  Json& Json::operator=(JsonArray                           value) { _value = value;               return *this; }
  Json& Json::operator=(JsonBuilder                         value) { _value = Json(value)._value;  return *this; }
  Json& Json::operator=(std::initializer_list<JsonBuilder> values) { _value = Json(values)._value; return *this; }

  wostream& operator<<(wostream& os, Json const& json)
  {
    return os << json.Dump(JsonDefault::Indentation);
  }

  wistream& operator>>(wistream&is, Json& json)
  {
    return Value::Read(is, json._value);
  }

  bool  operator==(Json const& left, Json const& right) { return Value::Equal(left._value, right._value);                }
  bool  operator!=(Json const& left, Json const& right) { return Value::NotEqual(left._value, right._value);             }
  bool  operator< (Json const& left, Json const& right) { return Value::LessThan(left._value, right._value);             }
  bool  operator<=(Json const& left, Json const& right) { return Value::LessThanOrEqual(left._value, right._value);      }
  bool  operator> (Json const& left, Json const& right) { return Value::GreaterThan(left._value, right._value);          }
  bool  operator>=(Json const& left, Json const& right) { return Value::GreaterThanOrEqual(left._value, right._value);   }
  Json  operator+ (Json const& left, Json const& right) { return Json(Value::Add(left._value, right._value));            }
  Json& operator+=(Json&       left, Json const& right) { Value::AddAssign(left._value, right._value); return left;      }
  Json  operator- (Json const& left, Json const& right) { return Json(Value::Subtract(left._value, right._value));       }
  Json& operator-=(Json&       left, Json const& right) { Value::SubtractAssign(left._value, right._value); return left; }
  Json  operator* (Json const& left, Json const& right) { return Json(Value::Multiply(left._value, right._value));       }
  Json& operator*=(Json&       left, Json const& right) { Value::MultiplyAssign(left._value, right._value); return left; }
  Json  operator/ (Json const& left, Json const& right) { return Json(Value::Divide(left._value, right._value));         }
  Json& operator/=(Json&       left, Json const& right) { Value::DivideAssign(left._value, right._value); return left;   }
  Json  operator% (Json const& left, Json const& right) { return Json(Value::Modulo(left._value, right._value));         }
  Json& operator%=(Json&       left, Json const& right) { Value::ModuloAssign(left._value, right._value); return left;   }
  Json  operator- (Json const& value)                   { return Json(Value::Negate(value._value));                      }
  bool  operator! (Json const& value)                   { return Value::Not(value._value);                               }
  Json& operator++(Json&       value)                   { Value::PreIncrement(value._value); return value;               }
  Json  operator++(Json&       value, int)              { return Json(Value::PostIncrement(value._value));               }
  Json& operator--(Json&       value)                   { Value::PreDecrement(value._value); return value;               }
  Json  operator--(Json&       value, int)              { return Json(Value::PostDecrement(value._value));               }
  bool  operator&&(Json const& left, Json const& right) { return Value::LogicalAnd(left._value, right._value);           }
  bool  operator||(Json const& left, Json const& right) { return Value::LogicalOr(left._value, right._value);            }
}