#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonTypeTest)
  {
  public:
    TEST_METHOD(TestOperatorInsertion)
    {
      auto pairs = vector<pair<JsonType, wstring>>
      {
        { JsonType::Null   , L"Null"    },
        { JsonType::String , L"String"  },
        { JsonType::Boolean, L"Boolean" },
        { JsonType::Number , L"Number"  },
        { JsonType::Object , L"Object"  },
        { JsonType::Array  , L"Array"   },
        { JsonType::Simple , L"Simple"  },
        { JsonType::Complex, L"Complex" },
        { JsonType::Invalid, L"Invalid" },
      };
      for (auto& [input, expected] : pairs)
      {
        wostringstream os;
        os << input;
        Assert::AreEqual(expected, os.str());
      }
    }

    TEST_METHOD(TestOperatorEqual)
    {
      auto pairs = vector<tuple<JsonType, JsonType, bool>>
      {
        { JsonType::Null   , JsonType::Null   , true  },
        { JsonType::Null   , JsonType::String , false },
        { JsonType::Null   , JsonType::Boolean, false },
        { JsonType::Null   , JsonType::Number , false },
        { JsonType::Null   , JsonType::Object , false },
        { JsonType::Null   , JsonType::Array  , false },
        { JsonType::Null   , JsonType::Simple , true  },
        { JsonType::Null   , JsonType::Complex, false },
        { JsonType::Null   , JsonType::Invalid, false },
        { JsonType::String , JsonType::Null   , false },
        { JsonType::String , JsonType::String , true  },
        { JsonType::String , JsonType::Boolean, false },
        { JsonType::String , JsonType::Number , false },
        { JsonType::String , JsonType::Object , false },
        { JsonType::String , JsonType::Array  , false },
        { JsonType::String , JsonType::Simple , true  },
        { JsonType::String , JsonType::Complex, false },
        { JsonType::String , JsonType::Invalid, false },
        { JsonType::Boolean, JsonType::Null   , false },
        { JsonType::Boolean, JsonType::String , false },
        { JsonType::Boolean, JsonType::Boolean, true  },
        { JsonType::Boolean, JsonType::Number , false },
        { JsonType::Boolean, JsonType::Object , false },
        { JsonType::Boolean, JsonType::Array  , false },
        { JsonType::Boolean, JsonType::Simple , true  },
        { JsonType::Boolean, JsonType::Complex, false },
        { JsonType::Boolean, JsonType::Invalid, false },
        { JsonType::Number , JsonType::Null   , false },
        { JsonType::Number , JsonType::String , false },
        { JsonType::Number , JsonType::Boolean, false },
        { JsonType::Number , JsonType::Number , true  },
        { JsonType::Number , JsonType::Object , false },
        { JsonType::Number , JsonType::Array  , false },
        { JsonType::Number , JsonType::Simple , true  },
        { JsonType::Number , JsonType::Complex, false },
        { JsonType::Number , JsonType::Invalid, false },
        { JsonType::Object , JsonType::Null   , false },
        { JsonType::Object , JsonType::String , false },
        { JsonType::Object , JsonType::Boolean, false },
        { JsonType::Object , JsonType::Number , false },
        { JsonType::Object , JsonType::Object , true  },
        { JsonType::Object , JsonType::Array  , false },
        { JsonType::Object , JsonType::Simple , false },
        { JsonType::Object , JsonType::Complex, true  },
        { JsonType::Object , JsonType::Invalid, false },
        { JsonType::Array  , JsonType::Null   , false },
        { JsonType::Array  , JsonType::String , false },
        { JsonType::Array  , JsonType::Boolean, false },
        { JsonType::Array  , JsonType::Number , false },
        { JsonType::Array  , JsonType::Object , false },
        { JsonType::Array  , JsonType::Array  , true  },
        { JsonType::Array  , JsonType::Simple , false },
        { JsonType::Array  , JsonType::Complex, true  },
        { JsonType::Array  , JsonType::Invalid, false },
        { JsonType::Simple , JsonType::Null   , true  },
        { JsonType::Simple , JsonType::String , true  },
        { JsonType::Simple , JsonType::Boolean, true  },
        { JsonType::Simple , JsonType::Number , true  },
        { JsonType::Simple , JsonType::Object , false },
        { JsonType::Simple , JsonType::Array  , false },
        { JsonType::Simple , JsonType::Simple , true  },
        { JsonType::Simple , JsonType::Complex, false },
        { JsonType::Simple , JsonType::Invalid, false },
        { JsonType::Complex, JsonType::Null   , false },
        { JsonType::Complex, JsonType::String , false },
        { JsonType::Complex, JsonType::Boolean, false },
        { JsonType::Complex, JsonType::Number , false },
        { JsonType::Complex, JsonType::Object , true  },
        { JsonType::Complex, JsonType::Array  , true  },
        { JsonType::Complex, JsonType::Simple , false },
        { JsonType::Complex, JsonType::Complex, true  },
        { JsonType::Complex, JsonType::Invalid, false },
        { JsonType::Invalid, JsonType::Null   , false },
        { JsonType::Invalid, JsonType::String , false },
        { JsonType::Invalid, JsonType::Boolean, false },
        { JsonType::Invalid, JsonType::Number , false },
        { JsonType::Invalid, JsonType::Object , false },
        { JsonType::Invalid, JsonType::Array  , false },
        { JsonType::Invalid, JsonType::Simple , false },
        { JsonType::Invalid, JsonType::Complex, false },
        { JsonType::Invalid, JsonType::Invalid, true  },
      };
      for (auto& [left, right, expected] : pairs)
      {
        Assert::AreEqual(expected, left == right);
      }
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      auto pairs = vector<tuple<JsonType, JsonType, bool>>
      {
        { JsonType::Null   , JsonType::Null   , false },
        { JsonType::Null   , JsonType::String , true  },
        { JsonType::Null   , JsonType::Boolean, true  },
        { JsonType::Null   , JsonType::Number , true  },
        { JsonType::Null   , JsonType::Object , true  },
        { JsonType::Null   , JsonType::Array  , true  },
        { JsonType::Null   , JsonType::Simple , false },
        { JsonType::Null   , JsonType::Complex, true  },
        { JsonType::Null   , JsonType::Invalid, true  },
        { JsonType::String , JsonType::Null   , true  },
        { JsonType::String , JsonType::String , false },
        { JsonType::String , JsonType::Boolean, true  },
        { JsonType::String , JsonType::Number , true  },
        { JsonType::String , JsonType::Object , true  },
        { JsonType::String , JsonType::Array  , true  },
        { JsonType::String , JsonType::Simple , false },
        { JsonType::String , JsonType::Complex, true  },
        { JsonType::String , JsonType::Invalid, true  },
        { JsonType::Boolean, JsonType::Null   , true  },
        { JsonType::Boolean, JsonType::String , true  },
        { JsonType::Boolean, JsonType::Boolean, false },
        { JsonType::Boolean, JsonType::Number , true  },
        { JsonType::Boolean, JsonType::Object , true  },
        { JsonType::Boolean, JsonType::Array  , true  },
        { JsonType::Boolean, JsonType::Simple , false },
        { JsonType::Boolean, JsonType::Complex, true  },
        { JsonType::Boolean, JsonType::Invalid, true  },
        { JsonType::Number , JsonType::Null   , true  },
        { JsonType::Number , JsonType::String , true  },
        { JsonType::Number , JsonType::Boolean, true  },
        { JsonType::Number , JsonType::Number , false },
        { JsonType::Number , JsonType::Object , true  },
        { JsonType::Number , JsonType::Array  , true  },
        { JsonType::Number , JsonType::Simple , false },
        { JsonType::Number , JsonType::Complex, true  },
        { JsonType::Number , JsonType::Invalid, true  },
        { JsonType::Object , JsonType::Null   , true  },
        { JsonType::Object , JsonType::String , true  },
        { JsonType::Object , JsonType::Boolean, true  },
        { JsonType::Object , JsonType::Number , true  },
        { JsonType::Object , JsonType::Object , false },
        { JsonType::Object , JsonType::Array  , true  },
        { JsonType::Object , JsonType::Simple , true  },
        { JsonType::Object , JsonType::Complex, false },
        { JsonType::Object , JsonType::Invalid, true  },
        { JsonType::Array  , JsonType::Null   , true  },
        { JsonType::Array  , JsonType::String , true  },
        { JsonType::Array  , JsonType::Boolean, true  },
        { JsonType::Array  , JsonType::Number , true  },
        { JsonType::Array  , JsonType::Object , true  },
        { JsonType::Array  , JsonType::Array  , false },
        { JsonType::Array  , JsonType::Simple , true  },
        { JsonType::Array  , JsonType::Complex, false },
        { JsonType::Array  , JsonType::Invalid, true  },
        { JsonType::Simple , JsonType::Null   , false },
        { JsonType::Simple , JsonType::String , false },
        { JsonType::Simple , JsonType::Boolean, false },
        { JsonType::Simple , JsonType::Number , false },
        { JsonType::Simple , JsonType::Object , true  },
        { JsonType::Simple , JsonType::Array  , true  },
        { JsonType::Simple , JsonType::Simple , false },
        { JsonType::Simple , JsonType::Complex, true  },
        { JsonType::Simple , JsonType::Invalid, true  },
        { JsonType::Complex, JsonType::Null   , true  },
        { JsonType::Complex, JsonType::String , true  },
        { JsonType::Complex, JsonType::Boolean, true  },
        { JsonType::Complex, JsonType::Number , true  },
        { JsonType::Complex, JsonType::Object , false },
        { JsonType::Complex, JsonType::Array  , false },
        { JsonType::Complex, JsonType::Simple , true  },
        { JsonType::Complex, JsonType::Complex, false },
        { JsonType::Complex, JsonType::Invalid, true  },
        { JsonType::Invalid, JsonType::Null   , true  },
        { JsonType::Invalid, JsonType::String , true  },
        { JsonType::Invalid, JsonType::Boolean, true  },
        { JsonType::Invalid, JsonType::Number , true  },
        { JsonType::Invalid, JsonType::Object , true  },
        { JsonType::Invalid, JsonType::Array  , true  },
        { JsonType::Invalid, JsonType::Simple , true  },
        { JsonType::Invalid, JsonType::Complex, true  },
        { JsonType::Invalid, JsonType::Invalid, false },
      };
      for (auto& [left, right, expected] : pairs)
      {
        Assert::AreEqual(expected, left != right);
      }
    }
  };
}