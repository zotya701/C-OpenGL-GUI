#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonTest)
  {
  public:
    TEST_METHOD(TestConstructorDefault)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::AreEqual<Json>(nullptr, json);
    }

    TEST_METHOD(TestConstructorJsonBuilder)
    {
      Json json;

      json = JsonBuilder();
      Assert::AreEqual<VALUE>(nullptr, json._value);

      // JsonBuilder from VALUE
      json = JsonBuilder(VALUE(nullptr_t()));
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(VALUE(L"TestString"s));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(VALUE(true));
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder(VALUE(13.37));
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(VALUE(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(VALUE(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);

      // JsonBuilder constructors
      json = JsonBuilder(nullptr_t());
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(L"TestString");
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(L"TestString"s);
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(true);
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder((char)1);
      Assert::AreEqual<VALUE>(1.0, json._value);
      json = JsonBuilder((int8_t)2);
      Assert::AreEqual<VALUE>(2.0, json._value);
      json = JsonBuilder((uint8_t)3);
      Assert::AreEqual<VALUE>(3.0, json._value);
      json = JsonBuilder((int16_t)4);
      Assert::AreEqual<VALUE>(4.0, json._value);
      json = JsonBuilder((uint16_t)5);
      Assert::AreEqual<VALUE>(5.0, json._value);
      json = JsonBuilder((int32_t)6);
      Assert::AreEqual<VALUE>(6.0, json._value);
      json = JsonBuilder((uint32_t)7);
      Assert::AreEqual<VALUE>(7.0, json._value);
      json = JsonBuilder((int64_t)8);
      Assert::AreEqual<VALUE>(8.0, json._value);
      json = JsonBuilder((uint64_t)9);
      Assert::AreEqual<VALUE>(9.0, json._value);
      json = JsonBuilder(13.37f);
      Assert::AreEqual<VALUE>((double)13.37f, json._value);
      json = JsonBuilder(13.37);
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(JsonArray({ 1337, L"1337", true }));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      json = JsonBuilder(Json());
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(Json(nullptr_t()));
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(Json(L"TestString"));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(Json(L"TestString"s));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(Json(true));
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder(Json((char)1));
      Assert::AreEqual<VALUE>(1.0, json._value);
      json = JsonBuilder(Json((int8_t)2));
      Assert::AreEqual<VALUE>(2.0, json._value);
      json = JsonBuilder(Json((uint8_t)3));
      Assert::AreEqual<VALUE>(3.0, json._value);
      json = JsonBuilder(Json((int16_t)4));
      Assert::AreEqual<VALUE>(4.0, json._value);
      json = JsonBuilder(Json((uint16_t)5));
      Assert::AreEqual<VALUE>(5.0, json._value);
      json = JsonBuilder(Json((int32_t)6));
      Assert::AreEqual<VALUE>(6.0, json._value);
      json = JsonBuilder(Json((uint32_t)7));
      Assert::AreEqual<VALUE>(7.0, json._value);
      json = JsonBuilder(Json((int64_t)8));
      Assert::AreEqual<VALUE>(8.0, json._value);
      json = JsonBuilder(Json((uint64_t)9));
      Assert::AreEqual<VALUE>(9.0, json._value);
      json = JsonBuilder(Json(13.37f));
      Assert::AreEqual<VALUE>((double)13.37f, json._value);
      json = JsonBuilder(Json(13.37));
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(Json(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      json = JsonBuilder(Json(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
    }

    TEST_METHOD(TestConstructorInitializerList)
    {
      Json json = {
        nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
        { { L"Key1", nullptr }, { L"Key2", L"Test1" }, { L"Key3", L"Test2"s }, { L"Key4", false }, { L"Key5", true }, { L"Key6", (char)1 }, { L"Key7", 2i8 }, { L"Key8", 3ui8 },
          { L"Key9", 4i16 }, { L"Key10", 5ui16 }, { L"Key11", 6i32 }, { L"Key12", 7ui32 }, { L"Key13", 8i64 }, { L"Key14", 9ui64 }, { L"Key15", 13.37f }, { L"Key16", 313.37 },
          { L"Key17", { { L"InnerKey1", L"Value1" }, { L"InnerKey2", L"Value2" } } },
          { L"Key18", { 1, 3, 3, 7 } } },
        { nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
          { { L"InnerKey1", L"Value1" }, { L"InnerKey2", L"Value2" } },
          { 1, 3, 3, 7 } }
      };

      Assert::AreEqual<Json>(18, json.Size());
      Assert::AreEqual<Json>(18, json[16].Size());
      Assert::AreEqual<Json>(18, json[17].Size());

      auto values = vector<Json>{ nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37 };
      for (int i = 0; i < values.size(); ++i)
      {
        Assert::AreEqual<Json>(values[i], json[i]);
        Assert::AreEqual<Json>(values[i], json[16][L"Key"s + to_wstring(i + 1)]);
        Assert::AreEqual<Json>(values[i], json[17][i]);
      }

      Assert::AreEqual<Json>(2, json[16][L"Key17"].Size());
      Assert::AreEqual<Json>(L"Value1"s, json[16][L"Key17"][L"InnerKey1"]);
      Assert::AreEqual<Json>(L"Value2"s, json[16][L"Key17"][L"InnerKey2"]);
      Assert::AreEqual<Json>(4, json[16][L"Key18"].Size());
      Assert::AreEqual<Json>(1, json[16][L"Key18"][0]);
      Assert::AreEqual<Json>(3, json[16][L"Key18"][1]);
      Assert::AreEqual<Json>(3, json[16][L"Key18"][2]);
      Assert::AreEqual<Json>(7, json[16][L"Key18"][3]);

      Assert::AreEqual<Json>(2, json[17][16].Size());
      Assert::AreEqual<Json>(L"Value1"s, json[17][16][L"InnerKey1"]);
      Assert::AreEqual<Json>(L"Value2"s, json[17][16][L"InnerKey2"]);
      Assert::AreEqual<Json>(4, json[17][17].Size());
      Assert::AreEqual<Json>(1, json[17][17][0]);
      Assert::AreEqual<Json>(3, json[17][17][1]);
      Assert::AreEqual<Json>(3, json[17][17][2]);
      Assert::AreEqual<Json>(7, json[17][17][3]);
    }

    TEST_METHOD(TestConstructorJson)
    {
      auto json = Json(nullptr);
      Assert::AreEqual<Json>(nullptr, Json(json));
      json = Json(L"Test");
      Assert::AreEqual<Json>(L"Test", Json(json));
      json = Json(L"Test"s);
      Assert::AreEqual<Json>(L"Test"s, Json(json));
      json = Json(false);
      Assert::AreEqual<Json>(false, Json(json));
      json = Json(true);
      Assert::AreEqual<Json>(true, Json(json));
      json = Json((char)1);
      Assert::AreEqual<Json>((char)1, Json(json));
      json = Json(2i8);
      Assert::AreEqual<Json>(2i8, Json(json));
      json = Json(3ui8);
      Assert::AreEqual<Json>(3ui8, Json(json));
      json = Json(4i16);
      Assert::AreEqual<Json>(4i16, Json(json));
      json = Json(5ui16);
      Assert::AreEqual<Json>(5ui16, Json(json));
      json = Json(6i32);
      Assert::AreEqual<Json>(6i32, Json(json));
      json = Json(7ui32);
      Assert::AreEqual<Json>(7ui32, Json(json));
      json = Json(8i64);
      Assert::AreEqual<Json>(8i64, Json(json));
      json = Json(9ui64);
      Assert::AreEqual<Json>(9ui64, Json(json));
      json = Json(13.37f);
      Assert::AreEqual<Json>(13.37f, Json(json));
      json = Json(13.37);
      Assert::AreEqual<Json>(13.37, Json(json));
      json = Json({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } });
      Assert::AreEqual<Json>({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } }, Json(json));
      json = Json({ 1, 3, 3, 7 });
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, Json(json));
    }

    TEST_METHOD(TestConstructorNullptr)
    {
      Assert::IsTrue(Json(nullptr_t()).Is(JsonType::Null));
      Assert::IsTrue(Json(nullptr_t()).Type() == JsonType::Null);
    }

    TEST_METHOD(TestConstructorConstWcharPointer)
    {
      Assert::IsTrue(Json(L"").Is(JsonType::String));
      Assert::IsTrue(Json(L"").Type() == JsonType::String);
    }

    TEST_METHOD(TestConstructorWstring)
    {
      Assert::IsTrue(Json(wstring()).Is(JsonType::String));
      Assert::IsTrue(Json(wstring()).Type() == JsonType::String);
    }

    TEST_METHOD(TestConstructorBool)
    {
      Assert::IsTrue(Json(bool()).Is(JsonType::Boolean));
      Assert::IsTrue(Json(bool()).Type() == JsonType::Boolean);
    }

    TEST_METHOD(TestConstructorChar)
    {
      Assert::IsTrue(Json(char()).Is(JsonType::Number));
      Assert::IsTrue(Json(char()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorInt8_t)
    {
      Assert::IsTrue(Json(int8_t()).Is(JsonType::Number));
      Assert::IsTrue(Json(int8_t()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorUInt8_t)
    {
      Assert::IsTrue(Json(uint8_t()).Is(JsonType::Number));
      Assert::IsTrue(Json(uint8_t()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorInt16_t)
    {
      Assert::IsTrue(Json(int16_t()).Is(JsonType::Number));
      Assert::IsTrue(Json(int16_t()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorUInt16_t)
    {
      Assert::IsTrue(Json(uint16_t()).Is(JsonType::Number));
      Assert::IsTrue(Json(uint16_t()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorInt32_t)
    {
      Assert::IsTrue(Json(int32_t()).Is(JsonType::Number));
      Assert::IsTrue(Json(int32_t()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorUInt32_t)
    {
      Assert::IsTrue(Json(uint32_t()).Is(JsonType::Number));
      Assert::IsTrue(Json(uint32_t()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorInt64_t)
    {
      Assert::IsTrue(Json(int64_t()).Is(JsonType::Number));
      Assert::IsTrue(Json(int64_t()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorUInt64_t)
    {
      Assert::IsTrue(Json(uint64_t()).Is(JsonType::Number));
      Assert::IsTrue(Json(uint64_t()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorFloat)
    {
      Assert::IsTrue(Json(float()).Is(JsonType::Number));
      Assert::IsTrue(Json(float()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorDouble)
    {
      Assert::IsTrue(Json(double()).Is(JsonType::Number));
      Assert::IsTrue(Json(double()).Type() == JsonType::Number);
    }

    TEST_METHOD(TestConstructorJsonObject)
    {
      Assert::IsTrue(Json(JsonObject()).Is(JsonType::Object));
      Assert::IsTrue(Json(JsonObject()).Type() == JsonType::Object);
    }

    TEST_METHOD(TestConstructorJsonArray)
    {
      Assert::IsTrue(Json(JsonArray()).Is(JsonType::Array));
      Assert::IsTrue(Json(JsonArray()).Type() == JsonType::Array);
    }

    TEST_METHOD(TestLiteral)
    {
      Assert::AreEqual<Json>({ nullptr }, L"[null]"_json);
      Assert::AreEqual<Json>({ L"TestString" }, L"[\"TestString\"]"_json);
      Assert::AreEqual<Json>({ true }, L"[true]"_json);
      Assert::AreEqual<Json>({ 13.37 }, L"[13.37]"_json);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, L"{ \"Key1\": 1, \"Key2\": 2 }"_json);
      Assert::AreEqual<Json>({ 1, 2, 3 }, L"[ 1, 2, 3 ]"_json);
    }

    TEST_METHOD(TestType)
    {
      auto tuples = vector<tuple<Json, JsonType, JsonType>>
      {
        { nullptr_t() , JsonType::Null   , JsonType::Simple  },
        { wstring()   , JsonType::String , JsonType::Simple  },
        { bool()      , JsonType::Boolean, JsonType::Simple  },
        { double()    , JsonType::Number , JsonType::Simple  },
        { JsonObject(), JsonType::Object , JsonType::Complex },
        { JsonArray() , JsonType::Array  , JsonType::Complex },
      };
      for (auto& [input, expected1, expected2] : tuples)
      {
        Assert::AreEqual(expected1, input.Type());
        Assert::AreEqual(expected2, input.Type());
      }
    }

    TEST_METHOD(TestIs)
    {
      auto tuples = vector<tuple<Json, JsonType, JsonType>>
      {
        { nullptr_t() , JsonType::Null   , JsonType::Simple  },
        { wstring()   , JsonType::String , JsonType::Simple  },
        { bool()      , JsonType::Boolean, JsonType::Simple  },
        { double()    , JsonType::Number , JsonType::Simple  },
        { JsonObject(), JsonType::Object , JsonType::Complex },
        { JsonArray() , JsonType::Array  , JsonType::Complex },
      };
      for (auto& [input, expected1, expected2] : tuples)
      {
        Assert::IsTrue(input.Is(expected1));
        Assert::IsTrue(input.Is(expected2));
      }
    }

    TEST_METHOD(TestDump)
    {
      auto pairs = vector<pair<Json, wstring>>
      {
        { nullptr  , L"null"s      },
        { L"Test1" , L"\"Test1\""s },
        { L"Test2"s, L"\"Test2\""s },
        { false    , L"false"s     },
        { true     , L"true"s      },
        { (char)1  , L"1"s         },
        { 2i8      , L"2"s         },
        { 3ui8     , L"3"s         },
        { 4i16     , L"4"s         },
        { 5ui16    , L"5"s         },
        { 6i32     , L"6"s         },
        { 7ui32    , L"7"s         },
        { 8i64     , L"8"s         },
        { 9ui64    , L"9"s         },
        { 13.37f   , L"13.37"s     },
        { 313.37   , L"313.37"s    },
        { 337184288, L"337184288"s },
        { 1.3f     , L"1.3"        },
        { 1.3      , L"1.3"        }
      };
      for (auto& [input, expected] : pairs)
      {
        for (int i = 0; i < 4; ++i)
        {
          Assert::AreEqual(expected, input.Dump(i));
        }
      }

      Json object = {
        { L"Key1", nullptr },
        { L"Key2", L"Test1" },
        { L"Key3", L"Test2"s },
        { L"Key4", false },
        { L"Key5", true },
        { L"Key6", (char)1 },
        { L"Key7", 2i8 },
        { L"Key8", 3ui8 },
        { L"Key9", 4i16 },
        { L"Key10", 5ui16 },
        { L"Key11", 6i32 },
        { L"Key12", 7ui32 },
        { L"Key13", 8i64 },
        { L"Key14", 9ui64 },
        { L"Key15", 13.37f },
        { L"Key16", 313.37 },
        { L"Key17", {
          { L"InnerKey1", L"Value1" },
          { L"InnerKey2", L"Value2" } }
        },
        { L"Key18", { 1, 3, 3, 7 } }
      };

      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump());
      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump(0));

      Assert::AreEqual(
        L"{"                              "\r\n"
         " \"Key1\": null,"               "\r\n"
         " \"Key2\": \"Test1\","          "\r\n"
         " \"Key3\": \"Test2\","          "\r\n"
         " \"Key4\": false,"              "\r\n"
         " \"Key5\": true,"               "\r\n"
         " \"Key6\": 1,"                  "\r\n"
         " \"Key7\": 2,"                  "\r\n"
         " \"Key8\": 3,"                  "\r\n"
         " \"Key9\": 4,"                  "\r\n"
         " \"Key10\": 5,"                 "\r\n"
         " \"Key11\": 6,"                 "\r\n"
         " \"Key12\": 7,"                 "\r\n"
         " \"Key13\": 8,"                 "\r\n"
         " \"Key14\": 9,"                 "\r\n"
         " \"Key15\": 13.37,"             "\r\n"
         " \"Key16\": 313.37,"            "\r\n"
         " \"Key17\": {"                  "\r\n"
         "  \"InnerKey1\": \"Value1\","   "\r\n"
         "  \"InnerKey2\": \"Value2\""    "\r\n"
         " },"                            "\r\n"
         " \"Key18\": ["                  "\r\n"
         "  1,"                           "\r\n"
         "  3,"                           "\r\n"
         "  3,"                           "\r\n"
         "  7"                            "\r\n"
         " ]"                             "\r\n"
         "}"s, object.Dump(1));

      Assert::AreEqual(
        L"{"                              "\r\n"
         "  \"Key1\": null,"              "\r\n"
         "  \"Key2\": \"Test1\","         "\r\n"
         "  \"Key3\": \"Test2\","         "\r\n"
         "  \"Key4\": false,"             "\r\n"
         "  \"Key5\": true,"              "\r\n"
         "  \"Key6\": 1,"                 "\r\n"
         "  \"Key7\": 2,"                 "\r\n"
         "  \"Key8\": 3,"                 "\r\n"
         "  \"Key9\": 4,"                 "\r\n"
         "  \"Key10\": 5,"                "\r\n"
         "  \"Key11\": 6,"                "\r\n"
         "  \"Key12\": 7,"                "\r\n"
         "  \"Key13\": 8,"                "\r\n"
         "  \"Key14\": 9,"                "\r\n"
         "  \"Key15\": 13.37,"            "\r\n"
         "  \"Key16\": 313.37,"           "\r\n"
         "  \"Key17\": {"                 "\r\n"
         "    \"InnerKey1\": \"Value1\"," "\r\n"
         "    \"InnerKey2\": \"Value2\""  "\r\n"
         "  },"                           "\r\n"
         "  \"Key18\": ["                 "\r\n"
         "    1,"                         "\r\n"
         "    3,"                         "\r\n"
         "    3,"                         "\r\n"
         "    7"                          "\r\n"
         "  ]"                            "\r\n"
         "}"s, object.Dump(2));

      Json array = {
        nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
        {
          { L"InnerKey1", L"Value1" },
          { L"InnerKey2", L"Value2" }
        },
        { 1, 3, 3, 7 }
      };

      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump());
      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump(0));

      Assert::AreEqual(
        L"["                              "\r\n"
         " null,"                         "\r\n"
         " \"Test1\","                    "\r\n"
         " \"Test2\","                    "\r\n"
         " false,"                        "\r\n"
         " true,"                         "\r\n"
         " 1,"                            "\r\n"
         " 2,"                            "\r\n"
         " 3,"                            "\r\n"
         " 4,"                            "\r\n"
         " 5,"                            "\r\n"
         " 6,"                            "\r\n"
         " 7,"                            "\r\n"
         " 8,"                            "\r\n"
         " 9,"                            "\r\n"
         " 13.37,"                        "\r\n"
         " 313.37,"                       "\r\n"
         " {"                             "\r\n"
         "  \"InnerKey1\": \"Value1\","   "\r\n"
         "  \"InnerKey2\": \"Value2\""    "\r\n"
         " },"                            "\r\n"
         " ["                             "\r\n"
         "  1,"                           "\r\n"
         "  3,"                           "\r\n"
         "  3,"                           "\r\n"
         "  7"                            "\r\n"
         " ]"                             "\r\n"
         "]"s, array.Dump(1));

      Assert::AreEqual(
        L"["                              "\r\n"
         "  null,"                         "\r\n"
         "  \"Test1\","                    "\r\n"
         "  \"Test2\","                    "\r\n"
         "  false,"                        "\r\n"
         "  true,"                         "\r\n"
         "  1,"                            "\r\n"
         "  2,"                            "\r\n"
         "  3,"                            "\r\n"
         "  4,"                            "\r\n"
         "  5,"                            "\r\n"
         "  6,"                            "\r\n"
         "  7,"                            "\r\n"
         "  8,"                            "\r\n"
         "  9,"                            "\r\n"
         "  13.37,"                        "\r\n"
         "  313.37,"                       "\r\n"
         "  {"                             "\r\n"
         "    \"InnerKey1\": \"Value1\","   "\r\n"
         "    \"InnerKey2\": \"Value2\""    "\r\n"
         "  },"                            "\r\n"
         "  ["                             "\r\n"
         "    1,"                           "\r\n"
         "    3,"                           "\r\n"
         "    3,"                           "\r\n"
         "    7"                            "\r\n"
         "  ]"                             "\r\n"
         "]"s, array.Dump(2));
    }
  };
}