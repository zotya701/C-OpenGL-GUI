﻿#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonLinterTest)
  {
  private:
    template<typename T, typename F>
    static void ExceptException(F func, string const& msg)
    {
      auto found = false;
      try
      {
        func();
      }
      catch (T e)
      {
        Assert::AreEqual(msg.c_str(), e.what());
        found = true;
      }
      Assert::AreEqual(found, true);
    }
  public:
    TEST_METHOD(TestParseNull)
    {
      auto pairs = vector<tuple<wstring, bool, string>>
      {
        { L""s      , true , "Expected digit at position Line: 1 Column: 1!"s    },
        { L" "s     , true , "Expected digit at position Line: 1 Column: 2!"s    },
        { L" n"s    , true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" nu"s   , true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" nul"s  , true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" null"s , false, ""s                                                 },
        { L" null "s, true , "Unexpected ' ' at position Line: 1 Column: 6!"s    },
        { L" nu11 "s, true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" nill "s, true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" mull "s, true , "Expected digit at position Line: 1 Column: 2!"s    },
      };
      for (auto& [input, expectException, exceptionMessage] : pairs)
      {
        if (!expectException)
        {
          auto tokens1 = JsonLinter::Read(              input );
          auto tokens2 = JsonLinter::Read(wstringstream(input));
          Assert::AreEqual<size_t>(1, tokens1.size());
          Assert::AreEqual<size_t>(1, tokens2.size());
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Null, tokens1[0].first);
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Null, tokens2[0].first);
          Assert::AreEqual<VALUE_TOKEN>(nullptr, tokens1[0].second);
          Assert::AreEqual<VALUE_TOKEN>(nullptr, tokens2[0].second);
        }
        else
        {
          ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
          ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
        }
      }
    }

    TEST_METHOD(TestParseString)
    {
      auto pairs = vector<pair<wstring, wstring>>
      {
        { L"\"\""s        , L""s        },  // Empty
        { L"\"a\""s       , L"a"s       },  // a
        { L"\"\\\"\""s    , L"\""s      },  // Quote
        { L"\"\\\\\""s    , L"\\"s      },  // Backslash
        { L"\"\\/\""s     , L"/"s       },  // Slash
        { L"\"\\b\""s     , L"\b"s      },  // Backspace
        { L"\"\\f\""s     , L"\f"s      },  // Form feed
        { L"\"\\n\""s     , L"\n"s      },  // New line
        { L"\"\\r\""s     , L"\r"s      },  // Carriage return
        { L"\"\\t\""s     , L"\t"s      },  // Tab
        { L"\"\\u03A9\""s , L"\u03A9"s  },  // Ω
        { L"\"ab\""s      , L"ab"s      },
        { L"\"\\\"b\""s   , L"\"b"s     },
        { L"\"\\\\b\""s   , L"\\b"s     },
        { L"\"\\/b\""s    , L"/b"s      },
        { L"\"\\bb\""s    , L"\bb"s     },
        { L"\"\\fb\""s    , L"\fb"s     },
        { L"\"\\nb\""s    , L"\nb"s     },
        { L"\"\\rb\""s    , L"\rb"s     },
        { L"\"\\tb\""s    , L"\tb"s     },
        { L"\"\\u03A9b\""s, L"\u03A9b"s },
      };

      for (auto& [input, expected] : pairs)
      {
        auto tokens1 = JsonLinter::Read(              input );
        auto tokens2 = JsonLinter::Read(wstringstream(input));
        Assert::AreEqual<size_t>(1, tokens1.size());
        Assert::AreEqual<size_t>(1, tokens2.size());
        Assert::AreEqual<JsonTokenType>(JsonTokenType::String, tokens1[0].first);
        Assert::AreEqual<JsonTokenType>(JsonTokenType::String, tokens2[0].first);
        Assert::AreEqual<VALUE_TOKEN>(expected, tokens1[0].second);
        Assert::AreEqual<VALUE_TOKEN>(expected, tokens2[0].second);
      }

      auto pairs2 = vector<pair<wstring, string>>
      {
        { L"\"asd"s      , "Expected '\"' at position Line: 1 Column: 5!"s },
        { L"\"asd\\u123"s, "Expected a hexadecimal digit at position Line: 1 Column: 10!"s },
        { L"\"asd\\a"s   , "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 7!"s },
        { L"\"asd\t"s    , "Invalid character found at position Line: 1 Column: 5!"s },
      };

      for (auto& [input, exceptionMessage] : pairs2)
      {
        ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
        ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestParseBoolean1)
    {
      auto pairs = vector<tuple<wstring, bool, string>>
      {
        { L"",       true , "Expected digit at position Line: 1 Column: 1!"s    },
        { L" ",      true , "Expected digit at position Line: 1 Column: 2!"s    },
        { L" t",     true , "Expected \"true\" at position Line: 1 Column: 2!"s },
        { L" tr",    true , "Expected \"true\" at position Line: 1 Column: 2!"s },
        { L" tru",   true , "Expected \"true\" at position Line: 1 Column: 2!"s },
        { L" true",  false, ""s                                                 },
        { L" true ", true , "Unexpected ' ' at position Line: 1 Column: 6!"s    },
        { L" trie ", true , "Expected \"true\" at position Line: 1 Column: 2!"s },
      };
      for (auto& [input, expectException, exceptionMessage] : pairs)
      {
        if (!expectException)
        {
          auto tokens1 = JsonLinter::Read(input);
          auto tokens2 = JsonLinter::Read(wstringstream(input));
          Assert::AreEqual<size_t>(1, tokens1.size());
          Assert::AreEqual<size_t>(1, tokens2.size());
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Boolean, tokens1[0].first);
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Boolean, tokens2[0].first);
          Assert::AreEqual<VALUE_TOKEN>(true, tokens1[0].second);
          Assert::AreEqual<VALUE_TOKEN>(true, tokens2[0].second);
        }
        else
        {
          ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
          ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
        }
      }
    }

    TEST_METHOD(TestParseBoolean2)
    {
      auto pairs = vector<tuple<wstring, bool, string>>
      {
        { L"",        true , "Expected digit at position Line: 1 Column: 1!"s     },
        { L" ",       true , "Expected digit at position Line: 1 Column: 2!"s     },
        { L" f",      true , "Expected \"false\" at position Line: 1 Column: 2!"s },
        { L" fa",     true , "Expected \"false\" at position Line: 1 Column: 2!"s },
        { L" fal",    true , "Expected \"false\" at position Line: 1 Column: 2!"s },
        { L" fals",   true , "Expected \"false\" at position Line: 1 Column: 2!"s },
        { L" false",  false, ""s                                                  },
        { L" false ", true , "Unexpected ' ' at position Line: 1 Column: 7!"s     },
        { L" fakse ", true , "Expected \"false\" at position Line: 1 Column: 2!"s },
      };
      for (auto& [input, expectException, exceptionMessage] : pairs)
      {
        if (!expectException)
        {
          auto tokens1 = JsonLinter::Read(              input );
          auto tokens2 = JsonLinter::Read(wstringstream(input));
          Assert::AreEqual<size_t>(1, tokens1.size());
          Assert::AreEqual<size_t>(1, tokens2.size());
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Boolean, tokens1[0].first);
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Boolean, tokens2[0].first);
          Assert::AreEqual<VALUE_TOKEN>(false, tokens1[0].second);
          Assert::AreEqual<VALUE_TOKEN>(false, tokens2[0].second);
        }
        else
        {
          ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
          ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
        }
      }
    }

    TEST_METHOD(TestParseNumber)
    {
      auto pairs = vector<pair<wstring, double>>
      {
        { L"0"s          , 0.0                },
        { L"0E+1"s       , 0.0                },
        { L"0E+12"s      , 0.0                },
        { L"0E1"s        , 0.0                },
        { L"0E12"s       , 0.0                },
        { L"0E-1"s       , 0.0                },
        { L"0E-12"s      , 0.0                },
        { L"0e+1"s       , 0.0                },
        { L"0e+12"s      , 0.0                },
        { L"0e1"s        , 0.0                },
        { L"0e12"s       , 0.0                },
        { L"0e-1"s       , 0.0                },
        { L"0e-12"s      , 0.0                },
        { L"0.1"s        , 0.1                },
        { L"0.1E+1"s     , 1.0                },
        { L"0.1E+12"s    , 100000000000.0     },
        { L"0.1E1"s      , 1.0                },
        { L"0.1E12"s     , 100000000000.0     },
        { L"0.1E-1"s     , 0.01               },
        { L"0.1E-12"s    , 0.0000000000001    },
        { L"0.1e+1"s     , 1.0                },
        { L"0.1e+12"s    , 100000000000.0     },
        { L"0.1e1"s      , 1.0                },
        { L"0.1e12"s     , 100000000000.0     },
        { L"0.1e-1"s     , 0.01               },
        { L"0.1e-12"s    , 0.0000000000001    },
        { L"0.12"s       , 0.12               },
        { L"0.12E+1"s    , 1.2                },
        { L"0.12E+12"s   , 120000000000.0     },
        { L"0.12E1"s     , 1.2                },
        { L"0.12E12"s    , 120000000000.0     },
        { L"0.12E-1"s    , 0.012              },
        { L"0.12E-12"s   , 0.00000000000012   },
        { L"0.12e+1"s    , 1.2                },
        { L"0.12e+12"s   , 120000000000.0     },
        { L"0.12e1"s     , 1.2                },
        { L"0.12e12"s    , 120000000000.0     },
        { L"0.12e-1"s    , 0.012              },
        { L"0.12e-12"s   , 0.00000000000012   },
        { L"1"s          , 1.0                },
        { L"1E+1"s       , 10.0               },
        { L"1E+12"s      , 1000000000000.0    },
        { L"1E1"s        , 10.0               },
        { L"1E12"s       , 1000000000000.0    },
        { L"1E-1"s       , 0.1                },
        { L"1E-12"s      , 0.000000000001     },
        { L"1e+1"s       , 10.0               },
        { L"1e+12"s      , 1000000000000.0    },
        { L"1e1"s        , 10.0               },
        { L"1e12"s       , 1000000000000.0    },
        { L"1e-1"s       , 0.1                },
        { L"1e-12"s      , 0.000000000001     },
        { L"1.1"s        , 1.1                },
        { L"1.1E+1"s     , 11.0               },
        { L"1.1E+12"s    , 1100000000000.0    },
        { L"1.1E1"s      , 11.0               },
        { L"1.1E12"s     , 1100000000000.0    },
        { L"1.1E-1"s     , 0.11               },
        { L"1.1E-12"s    , 0.0000000000011    },
        { L"1.1e+1"s     , 11.0               },
        { L"1.1e+12"s    , 1100000000000.0    },
        { L"1.1e1"s      , 11.0               },
        { L"1.1e12"s     , 1100000000000.0    },
        { L"1.1e-1"s     , 0.11               },
        { L"1.1e-12"s    , 0.0000000000011    },
        { L"1.12"s       , 1.12               },
        { L"1.12E+1"s    , 11.2               },
        { L"1.12E+12"s   , 1120000000000.0    },
        { L"1.12E1"s     , 11.2               },
        { L"1.12E12"s    , 1120000000000.0    },
        { L"1.12E-1"s    , 0.112              },
        { L"1.12E-12"s   , 0.00000000000112   },
        { L"1.12e+1"s    , 11.2               },
        { L"1.12e+12"s   , 1120000000000.0    },
        { L"1.12e1"s     , 11.2               },
        { L"1.12e12"s    , 1120000000000.0    },
        { L"1.12e-1"s    , 0.112              },
        { L"1.12e-12"s   , 0.00000000000112   },
        { L"12"s         , 12.0               },
        { L"12E+1"s      , 120.0              },
        { L"12E+12"s     , 12000000000000.0   },
        { L"12E1"s       , 120.0              },
        { L"12E12"s      , 12000000000000.0   },
        { L"12E-1"s      , 1.2                },
        { L"12E-12"s     , 0.000000000012     },
        { L"12e+1"s      , 120.0              },
        { L"12e+12"s     , 12000000000000.0   },
        { L"12e1"s       , 120.0              },
        { L"12e12"s      , 12000000000000.0   },
        { L"12e-1"s      , 1.2                },
        { L"12e-12"s     , 0.000000000012     },
        { L"12.1"s       , 12.1               },
        { L"12.1E+1"s    , 121.0              },
        { L"12.1E+12"s   , 12100000000000.0   },
        { L"12.1E1"s     , 121.0              },
        { L"12.1E12"s    , 12100000000000.0   },
        { L"12.1E-1"s    , 1.21               },
        { L"12.1E-12"s   , 0.0000000000121    },
        { L"12.1e+1"s    , 121.0              },
        { L"12.1e+12"s   , 12100000000000.0   },
        { L"12.1e1"s     , 121.0              },
        { L"12.1e12"s    , 12100000000000.0   },
        { L"12.1e-1"s    , 1.21               },
        { L"12.1e-12"s   , 0.0000000000121    },
        { L"12.12"s      , 12.12              },
        { L"12.12E+1"s   , 121.2              },
        { L"12.12E+12"s  , 12120000000000.0   },
        { L"12.12E1"s    , 121.2              },
        { L"12.12E12"s   , 12120000000000.0   },
        { L"12.12E-1"s   , 1.212              },
        { L"12.12E-12"s  , 0.00000000001212   },
        { L"12.12e+1"s   , 121.2              },
        { L"12.12e+12"s  , 12120000000000.0   },
        { L"12.12e1"s    , 121.2              },
        { L"12.12e12"s   , 12120000000000.0   },
        { L"12.12e-1"s   , 1.212              },
        { L"12.12e-12"s  , 0.00000000001212   },
        { L"123"s        , 123.0              },
        { L"123E+1"s     , 1230.0             },
        { L"123E+12"s    , 123000000000000.0  },
        { L"123E1"s      , 1230.0             },
        { L"123E12"s     , 123000000000000.0  },
        { L"123E-1"s     , 12.3               },
        { L"123E-12"s    , 0.000000000123     },
        { L"123e+1"s     , 1230.0             },
        { L"123e+12"s    , 123000000000000.0  },
        { L"123e1"s      , 1230.0             },
        { L"123e12"s     , 123000000000000.0  },
        { L"123e-1"s     , 12.3               },
        { L"123e-12"s    , 0.000000000123     },
        { L"123.1"s      , 123.1              },
        { L"123.1E+1"s   , 1231.0             },
        { L"123.1E+12"s  , 123100000000000.0  },
        { L"123.1E1"s    , 1231.0             },
        { L"123.1E12"s   , 123100000000000.0  },
        { L"123.1E-1"s   , 12.31              },
        { L"123.1E-12"s  , 0.0000000001231    },
        { L"123.1e+1"s   , 1231.0             },
        { L"123.1e+12"s  , 123100000000000.0  },
        { L"123.1e1"s    , 1231.0             },
        { L"123.1e12"s   , 123100000000000.0  },
        { L"123.1e-1"s   , 12.31              },
        { L"123.1e-12"s  , 0.0000000001231    },
        { L"123.12"s     , 123.12             },
        { L"123.12E+1"s  , 1231.2             },
        { L"123.12E+12"s , 123120000000000.0  },
        { L"123.12E1"s   , 1231.2             },
        { L"123.12E12"s  , 123120000000000.0  },
        { L"123.12E-1"s  , 12.312             },
        { L"123.12E-12"s , 0.00000000012312   },
        { L"123.12e+1"s  , 1231.2             },
        { L"123.12e+12"s , 123120000000000.0  },
        { L"123.12e1"s   , 1231.2             },
        { L"123.12e12"s  , 123120000000000.0  },
        { L"123.12e-1"s  , 12.312             },
        { L"123.12e-12"s , 0.00000000012312   },
        { L"-0"s         , 0.0                },
        { L"-0E+1"s      , 0.0                },
        { L"-0E+12"s     , 0.0                },
        { L"-0E1"s       , 0.0                },
        { L"-0E12"s      , 0.0                },
        { L"-0E-1"s      , 0.0                },
        { L"-0E-12"s     , 0.0                },
        { L"-0e+1"s      , 0.0                },
        { L"-0e+12"s     , 0.0                },
        { L"-0e1"s       , 0.0                },
        { L"-0e12"s      , 0.0                },
        { L"-0e-1"s      , 0.0                },
        { L"-0e-12"s     , 0.0                },
        { L"-0.1"s       , -0.1               },
        { L"-0.1E+1"s    , -1.0               },
        { L"-0.1E+12"s   , -100000000000.0    },
        { L"-0.1E1"s     , -1.0               },
        { L"-0.1E12"s    , -100000000000.0    },
        { L"-0.1E-1"s    , -0.01              },
        { L"-0.1E-12"s   , -0.0000000000001   },
        { L"-0.1e+1"s    , -1.0               },
        { L"-0.1e+12"s   , -100000000000.0    },
        { L"-0.1e1"s     , -1.0               },
        { L"-0.1e12"s    , -100000000000.0    },
        { L"-0.1e-1"s    , -0.01              },
        { L"-0.1e-12"s   , -0.0000000000001   },
        { L"-0.12"s      , -0.12              },
        { L"-0.12E+1"s   , -1.2               },
        { L"-0.12E+12"s  , -120000000000.0    },
        { L"-0.12E1"s    , -1.2               },
        { L"-0.12E12"s   , -120000000000.0    },
        { L"-0.12E-1"s   , -0.012             },
        { L"-0.12E-12"s  , -0.00000000000012  },
        { L"-0.12e+1"s   , -1.2               },
        { L"-0.12e+12"s  , -120000000000.0    },
        { L"-0.12e1"s    , -1.2               },
        { L"-0.12e12"s   , -120000000000.0    },
        { L"-0.12e-1"s   , -0.012             },
        { L"-0.12e-12"s  , -0.00000000000012  },
        { L"-1"s         , -1.0               },
        { L"-1E+1"s      , -10.0              },
        { L"-1E+12"s     , -1000000000000.0   },
        { L"-1E1"s       , -10.0              },
        { L"-1E12"s      , -1000000000000.0   },
        { L"-1E-1"s      , -0.1               },
        { L"-1E-12"s     , -0.000000000001    },
        { L"-1e+1"s      , -10.0              },
        { L"-1e+12"s     , -1000000000000.0   },
        { L"-1e1"s       , -10.0              },
        { L"-1e12"s      , -1000000000000.0   },
        { L"-1e-1"s      , -0.1               },
        { L"-1e-12"s     , -0.000000000001    },
        { L"-1.1"s       , -1.1               },
        { L"-1.1E+1"s    , -11.0              },
        { L"-1.1E+12"s   , -1100000000000.0   },
        { L"-1.1E1"s     , -11.0              },
        { L"-1.1E12"s    , -1100000000000.0   },
        { L"-1.1E-1"s    , -0.11              },
        { L"-1.1E-12"s   , -0.0000000000011   },
        { L"-1.1e+1"s    , -11.0              },
        { L"-1.1e+12"s   , -1100000000000.0   },
        { L"-1.1e1"s     , -11.0              },
        { L"-1.1e12"s    , -1100000000000.0   },
        { L"-1.1e-1"s    , -0.11              },
        { L"-1.1e-12"s   , -0.0000000000011   },
        { L"-1.12"s      , -1.12              },
        { L"-1.12E+1"s   , -11.2              },
        { L"-1.12E+12"s  , -1120000000000.0   },
        { L"-1.12E1"s    , -11.2              },
        { L"-1.12E12"s   , -1120000000000.0   },
        { L"-1.12E-1"s   , -0.112             },
        { L"-1.12E-12"s  , -0.00000000000112  },
        { L"-1.12e+1"s   , -11.2              },
        { L"-1.12e+12"s  , -1120000000000.0   },
        { L"-1.12e1"s    , -11.2              },
        { L"-1.12e12"s   , -1120000000000.0   },
        { L"-1.12e-1"s   , -0.112             },
        { L"-1.12e-12"s  , -0.00000000000112  },
        { L"-12"s        , -12.0              },
        { L"-12E+1"s     , -120.0             },
        { L"-12E+12"s    , -12000000000000.0  },
        { L"-12E1"s      , -120.0             },
        { L"-12E12"s     , -12000000000000.0  },
        { L"-12E-1"s     , -1.2               },
        { L"-12E-12"s    , -0.000000000012    },
        { L"-12e+1"s     , -120.0             },
        { L"-12e+12"s    , -12000000000000.0  },
        { L"-12e1"s      , -120.0             },
        { L"-12e12"s     , -12000000000000.0  },
        { L"-12e-1"s     , -1.2               },
        { L"-12e-12"s    , -0.000000000012    },
        { L"-12.1"s      , -12.1              },
        { L"-12.1E+1"s   , -121.0             },
        { L"-12.1E+12"s  , -12100000000000.0  },
        { L"-12.1E1"s    , -121.0             },
        { L"-12.1E12"s   , -12100000000000.0  },
        { L"-12.1E-1"s   , -1.21              },
        { L"-12.1E-12"s  , -0.0000000000121   },
        { L"-12.1e+1"s   , -121.0             },
        { L"-12.1e+12"s  , -12100000000000.0  },
        { L"-12.1e1"s    , -121.0             },
        { L"-12.1e12"s   , -12100000000000.0  },
        { L"-12.1e-1"s   , -1.21              },
        { L"-12.1e-12"s  , -0.0000000000121   },
        { L"-12.12"s     , -12.12             },
        { L"-12.12E+1"s  , -121.2             },
        { L"-12.12E+12"s , -12120000000000.0  },
        { L"-12.12E1"s   , -121.2             },
        { L"-12.12E12"s  , -12120000000000.0  },
        { L"-12.12E-1"s  , -1.212             },
        { L"-12.12E-12"s , -0.00000000001212  },
        { L"-12.12e+1"s  , -121.2             },
        { L"-12.12e+12"s , -12120000000000.0  },
        { L"-12.12e1"s   , -121.2             },
        { L"-12.12e12"s  , -12120000000000.0  },
        { L"-12.12e-1"s  , -1.212             },
        { L"-12.12e-12"s , -0.00000000001212  },
        { L"-123"s       , -123.0             },
        { L"-123E+1"s    , -1230.0            },
        { L"-123E+12"s   , -123000000000000.0 },
        { L"-123E1"s     , -1230.0            },
        { L"-123E12"s    , -123000000000000.0 },
        { L"-123E-1"s    , -12.3              },
        { L"-123E-12"s   , -0.000000000123    },
        { L"-123e+1"s    , -1230.0            },
        { L"-123e+12"s   , -123000000000000.0 },
        { L"-123e1"s     , -1230.0            },
        { L"-123e12"s    , -123000000000000.0 },
        { L"-123e-1"s    , -12.3              },
        { L"-123e-12"s   , -0.000000000123    },
        { L"-123.1"s     , -123.1             },
        { L"-123.1E+1"s  , -1231.0            },
        { L"-123.1E+12"s , -123100000000000.0 },
        { L"-123.1E1"s   , -1231.0            },
        { L"-123.1E12"s  , -123100000000000.0 },
        { L"-123.1E-1"s  , -12.31             },
        { L"-123.1E-12"s , -0.0000000001231   },
        { L"-123.1e+1"s  , -1231.0            },
        { L"-123.1e+12"s , -123100000000000.0 },
        { L"-123.1e1"s   , -1231.0            },
        { L"-123.1e12"s  , -123100000000000.0 },
        { L"-123.1e-1"s  , -12.31             },
        { L"-123.1e-12"s , -0.0000000001231   },
        { L"-123.12"s    , -123.12            },
        { L"-123.12E+1"s , -1231.2            },
        { L"-123.12E+12"s, -123120000000000.0 },
        { L"-123.12E1"s  , -1231.2            },
        { L"-123.12E12"s , -123120000000000.0 },
        { L"-123.12E-1"s , -12.312            },
        { L"-123.12E-12"s, -0.00000000012312  },
        { L"-123.12e+1"s , -1231.2            },
        { L"-123.12e+12"s, -123120000000000.0 },
        { L"-123.12e1"s  , -1231.2            },
        { L"-123.12e12"s , -123120000000000.0 },
        { L"-123.12e-1"s , -12.312            },
        { L"-123.12e-12"s, -0.00000000012312  }
      };

      for (auto& [input, expected] : pairs)
      {
        auto tokens1 = JsonLinter::Read(              input );
        auto tokens2 = JsonLinter::Read(wstringstream(input));
        Assert::AreEqual<size_t>(1, tokens1.size());
        Assert::AreEqual<size_t>(1, tokens2.size());
        Assert::AreEqual<JsonTokenType>(JsonTokenType::Number, tokens1[0].first);
        Assert::AreEqual<JsonTokenType>(JsonTokenType::Number, tokens2[0].first);
        Assert::AreEqual<VALUE_TOKEN>(expected, tokens1[0].second);
        Assert::AreEqual<VALUE_TOKEN>(expected, tokens2[0].second);
      }

      auto pairs2 = vector<pair<wstring, string>>
      {
        { L"asd"s, "Expected digit at position Line: 1 Column: 1!"s },
        { L"1."s , "Expected digit at position Line: 1 Column: 3!"s },
        { L"1e"s , "Expected digit at position Line: 1 Column: 3!"s },
      };

      for (auto& [input, exceptionMessage] : pairs2)
      {
        ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
        ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestParseObject)
    {
      auto pairs = vector<pair<wstring, vector<TOKEN>>>
      {
        // Test whitespace handling
        { L"{}"s  , { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  }"s, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\":1337}"s,         { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\":1337}"s,       { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\"  :1337}"s,       { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :1337}"s,     { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\":  1337}"s,       { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\":  1337}"s,     { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\"  :  1337}"s,     { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :  1337}"s,   { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\":1337  }"s,       { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\":1337  }"s,     { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\"  :1337  }"s,     { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :1337  }"s,   { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\":  1337  }"s,     { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\":  1337  }"s,   { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\"  :  1337  }"s,   { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :  1337  }"s, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 }, { JsonTokenType::EndObject, L"}"s } } },
        // Simple object with 2 key value pair
        { L"{\"key1\":1337,\"key2\":\"value2\"}"s,
        { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Number, 1337.0 },
          { JsonTokenType::PropertyName, L"key2"s }, { JsonTokenType::String, L"value2"s }, { JsonTokenType::EndObject, L"}"s } } },
        // Complex object with all types of values (string, number, object, array, bool, null)
        { L"{ \"string\": \"string\", \"number\": 1337, \"object\": { \"key1\": \"value1\", \"key2\": \"value2\" }, \"array\": [ 1, 3, 3, 7 ], \"true\": true, \"false\": false, \"null\": null }"s,
        { { JsonTokenType::StartObject, L"{"s },
          { JsonTokenType::PropertyName, L"string"s }, { JsonTokenType::String, L"string"s },
          { JsonTokenType::PropertyName, L"number"s }, { JsonTokenType::Number, 1337.0 },
          { JsonTokenType::PropertyName, L"object"s }, { JsonTokenType::StartObject, L"{"s },
          { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::String, L"value1"s },
          { JsonTokenType::PropertyName, L"key2"s }, { JsonTokenType::String, L"value2"s }, { JsonTokenType::EndObject, L"}"s },
          { JsonTokenType::PropertyName, L"array"s }, { JsonTokenType::StartArray, L"["s },
          { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 3.0 }, { JsonTokenType::Number, 3.0 }, { JsonTokenType::Number, 7.0 }, { JsonTokenType::EndArray, L"]"s },
          { JsonTokenType::PropertyName, L"true"s }, { JsonTokenType::Boolean, true },
          { JsonTokenType::PropertyName, L"false"s }, { JsonTokenType::Boolean, false },
          { JsonTokenType::PropertyName, L"null"s }, { JsonTokenType::Null, nullptr },
          { JsonTokenType::EndObject, L"}"s } } }
      };

      for (auto& [input, expected] : pairs)
      {
        auto tokens1 = JsonLinter::Read(              input );
        auto tokens2 = JsonLinter::Read(wstringstream(input));
        Assert::AreEqual<size_t>(expected.size(), tokens1.size());
        Assert::AreEqual<size_t>(expected.size(), tokens2.size());
        for (int i = 0; i < expected.size(); ++i)
        {
          Assert::AreEqual<JsonTokenType>(expected[i].first, tokens1[i].first);
          Assert::AreEqual<JsonTokenType>(expected[i].first, tokens2[i].first);
          Assert::AreEqual<VALUE_TOKEN>(expected[i].second, tokens1[i].second);
          Assert::AreEqual<VALUE_TOKEN>(expected[i].second, tokens2[i].second);
        }
      }

      auto pairs2 = vector<pair<wstring, string>>
      {
        { L"{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{}}}}}}}}}}}}}}}}}}}}"s, "Depth is greater or equal to the maximum 20!"s },
        { L"{  \r\n  \"key\"  \r\n  ,}"s, "Expected ':' at position Line: 3 Column: 3!"s },
        { L"{  \r\n  \"key\"  \r\n  :  \r\n  \"value\", \"key2\" \r\n }"s, "Expected ':' at position Line: 5 Column: 2!"s },
        { L"{  \r\n  \"key\"  \r\n  :  \r\n  \"value\", \"key2\" \r\n : 1337"s, "Expected '}' at position Line: 5 Column: 8!"s },
        { L"{\"key\":1337} "s, "Unexpected ' ' at position Line: 1 Column: 13!"s },
      };

      for (auto& [input, exceptionMessage] : pairs2)
      {
        ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
        ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestParseArray)
    {
      auto pairs = vector<pair<wstring, vector<TOKEN>>>
      {
        // Test whitespace handling
        { L"[]"s,   { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  ]"s, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1,2]"s,         { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1,2]"s,       { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1  ,2]"s,       { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1,  2]"s,       { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1,2  ]"s,       { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,2]"s,     { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1,  2]"s,     { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1,2  ]"s,     { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1  ,  2]"s,     { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1  ,2  ]"s,     { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1,  2  ]"s,     { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,  2]"s,   { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,2  ]"s,   { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1,  2  ]"s,   { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1  ,  2  ]"s,   { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,  2  ]"s, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 2.0 }, { JsonTokenType::EndArray, L"]"s } } },
        // Complex array with all types of values (string, number, object, array, bool, null)
        { L"[ \"string\",1337, {   \"key1\":\"value1\" ,   \"key2\":\"value2\"  }, [ 1, 3, 3, 7 ], true, false, null    ]"s,
        { { JsonTokenType::StartArray, L"["s },
          { JsonTokenType::String, L"string"s },
          { JsonTokenType::Number, 1337.0 },
          { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::String, L"value1"s }, { JsonTokenType::PropertyName, L"key2"s }, { JsonTokenType::String, L"value2"s }, { JsonTokenType::EndObject, L"}"s },
          { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 1.0 }, { JsonTokenType::Number, 3.0 }, { JsonTokenType::Number, 3.0 }, { JsonTokenType::Number, 7.0 }, { JsonTokenType::EndArray, L"]"s },
          { JsonTokenType::Boolean, true },
          { JsonTokenType::Boolean, false },
          { JsonTokenType::Null, nullptr },
          { JsonTokenType::EndArray, L"]"s } }},
      };

      for (auto& [input, expected] : pairs)
      {
        auto tokens1 = JsonLinter::Read(              input );
        auto tokens2 = JsonLinter::Read(wstringstream(input));
        Assert::AreEqual<size_t>(expected.size(), tokens1.size());
        Assert::AreEqual<size_t>(expected.size(), tokens2.size());
        for (int i = 0; i < expected.size(); ++i)
        {
          Assert::AreEqual<JsonTokenType>(expected[i].first, tokens1[i].first);
          Assert::AreEqual<JsonTokenType>(expected[i].first, tokens2[i].first);
          Assert::AreEqual<VALUE_TOKEN>(expected[i].second, tokens1[i].second);
          Assert::AreEqual<VALUE_TOKEN>(expected[i].second, tokens2[i].second);
        }
      }

      auto pairs2 = vector<pair<wstring, string>>
      {
        { L"[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]"s, "Depth is greater or equal to the maximum 20!"s },
        { L"[  \r\n  ,]"s, "Unexpected ',' at position Line: 2 Column: 3!"s },
        { L"[  \r\n  1  \r\n  "s, "Expected ']' at position Line: 3 Column: 3!"s },
        { L"[1337] "s, "Unexpected ' ' at position Line: 1 Column: 7!"s },
      };

      for (auto& [input, exceptionMessage] : pairs2)
      {
        ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
        ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestWrite)
    {
      auto pairs = vector<pair<deque<TOKEN>, wstring>>
      {
        { { { JsonTokenType::Null, nullptr      } }, L"null"s  },
        { { { JsonTokenType::String      , L"\"💰\", A, Á, B, C"s } }, L"\"\\\"💰\\\", A, Á, B, C\""s },
        { { { JsonTokenType::PropertyName, L"\"💰\", A, Á, B, C"s } }, L"\"\\\"💰\\\", A, Á, B, C\""s },
        { { { JsonTokenType::Boolean, true      } }, L"true"s  },
        { { { JsonTokenType::Boolean, false     } }, L"false"s },
        { { { JsonTokenType::Number, 13.37      } }, L"13.37"s },
        { { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } }, L"{}"s },
        { { { JsonTokenType::StartArray , L"["s }, { JsonTokenType::EndArray , L"]"s } }, L"[]"s },
        { { { JsonTokenType::Undefined, nullptr } }, L"null"s  },
        { { { JsonTokenType::Undefined, L"\"💰\", A, Á, B, C"s } }, L"\"\\\"💰\\\", A, Á, B, C\""s },
        { { { JsonTokenType::Undefined, true    } }, L"true"s  },
        { { { JsonTokenType::Undefined, false   } }, L"false"s },
        { { { JsonTokenType::Undefined, 13.37   } }, L"13.37"s },
      };

      for (auto [input, expected] : pairs)
      {
        auto os = wstringstream();
        JsonLinter::Write(os, input, JsonDefault::Indentation);
        Assert::AreEqual<size_t>(0, input.size());
        Assert::AreEqual(expected, os.str());
      }
    }

    TEST_METHOD(TestWriteObject)
    {
      auto pairs = vector<tuple<uint8_t, deque<TOKEN>, wstring>>
      {
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } }, L"{}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Null, nullptr }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":null}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::String, L"\"💰\", A, Á, B, C"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":\"\\\"💰\\\", A, Á, B, C\"}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":true}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":false}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Number, 13.37 }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":13.37}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":{}}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":[]}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":{\"Key2\":true}}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":[true]}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":true,\"Key2\":false}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } }, L"{}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Null, nullptr }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": null\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::String, L"\"💰\", A, Á, B, C"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": \"\\\"💰\\\", A, Á, B, C\"\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": true\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": false\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Number, 13.37 }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": 13.37\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": {}\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": []\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": {\r\n  \"Key2\": true\r\n }\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": [\r\n  true\r\n ]\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": true,\r\n \"Key2\": false\r\n}"s },
      };

      for (auto [indentation, input, expected] : pairs)
      {
        auto os = wstringstream();
        JsonLinter::Write(os, input, indentation);
        Assert::AreEqual<size_t>(0, input.size());
        Assert::AreEqual(expected, os.str());
      }

      auto pairs2 = vector<tuple<deque<TOKEN>, string>>
      {
        { deque<TOKEN>{ }, "Parameter 'tokens' is empty!" },
        { { { JsonTokenType::StartObject, L"{"s } }, "Expected token: EndObject!" },
        { { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::Boolean, true } }, "Expected token: PropertyName!" },
        { { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Value does not exists"s } }, "Expected one of the following tokens: Null, String, Boolean, Number, StartObject or StartArray!" },
        { { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Value does not exists"s }, { JsonTokenType::PropertyName, L"Invalid value"s } }, "Expected one of the following tokens: Null, String, Boolean, Number, StartObject or StartArray!" },
      };

      for (auto[input, exceptionMessage] : pairs2)
      {
        auto os = wstringstream();
        auto inputRef = input;
        ExceptException<exception>([&]() { JsonLinter::Write(os, inputRef, 0ui8); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestWriteArray)
    {
      auto pairs = vector<tuple<uint8_t, deque<TOKEN>, wstring>>
      {
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s } }, L"[]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Null, nullptr }, { JsonTokenType::EndArray, L"]"s } }, L"[null]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::String, L"\"💰\", A, Á, B, C"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\"\\\"💰\\\", A, Á, B, C\"]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s } }, L"[true]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndArray, L"]"s } }, L"[false]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 13.37 }, { JsonTokenType::EndArray, L"]"s } }, L"[13.37]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndArray, L"]"s } }, L"[{}]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndArray, L"]"s } }, L"[[]]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndArray, L"]"s } }, L"[{\"Key2\":true}]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndArray, L"]"s } }, L"[[true]]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndArray, L"]"s } }, L"[true,false]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s } }, L"[]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Null, nullptr }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n null\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::String, L"\"💰\", A, Á, B, C"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n \"\\\"💰\\\", A, Á, B, C\"\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n true\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n false\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Number, 13.37 }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n 13.37\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n {}\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n []\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n {\r\n  \"Key2\": true\r\n }\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n [\r\n  true\r\n ]\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n true,\r\n false\r\n]"s },
      };

      for (auto [indentation, input, expected] : pairs)
      {
        auto os = wstringstream();
        JsonLinter::Write(os, input, indentation);
        Assert::AreEqual<size_t>(0, input.size());
        Assert::AreEqual(expected, os.str());
      }

      auto pairs2 = vector<tuple<deque<TOKEN>, string>>
      {
        { deque<TOKEN>{ }, "Parameter 'tokens' is empty!" },
        { { { JsonTokenType::StartArray, L"["s } }, "Expected token: EndArray!" },
        { { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true } }, "Expected token: EndArray!" },
        { { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::PropertyName, L"Invalid token"s } }, "Expected one of the following tokens: Null, String, Boolean, Number, StartObject or StartArray!" },
      };

      for (auto[input, exceptionMessage] : pairs2)
      {
        auto os = wstringstream();
        auto inputRef = input;
        ExceptException<exception>([&]() { JsonLinter::Write(os, inputRef, 0ui8); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestDump)
    {
      auto pairs = vector<pair<VALUE_TOKEN, wstring>>
      {
        { nullptr_t() , L"null"s },
        { L"\"💰\", A, Á, B, C"s, L"\"\\\"💰\\\", A, Á, B, C\""s },
        { true, L"true"s },
        { false, L"false"s },
        { 13.37, L"13.37"s }
      };

      for (auto [input, expected] : pairs)
      {
        Assert::AreEqual(expected, JsonLinter::Dump(input));
      }
    }
  };
}