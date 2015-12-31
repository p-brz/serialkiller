#include "catch.hpp"
#include <serialization/GenericContainer.h>

#include <string>
using namespace std;

SCENARIO("GenericContainer setting and getting generic values", "[GenericContainer]"){

    GIVEN("an GenericContainer instance"){
        GenericContainer container;
    GIVEN("some primitive values"){
        const int    initialIntValue    = 100;
        const bool   initialBoolValue   = true;
        const long   initialLongValue   = -100l;
        const double initialDoubleValue = 1.0;
        const string initialStringValue = string("asdasd");

        THEN("GenericContainer should allow put any primitive type"){
            try{
                container.put("int",initialIntValue);
                container.put("bool", initialBoolValue);
                container.put("long", initialLongValue);
                container.put("double", initialDoubleValue);
                container.put("string", initialStringValue);
                INFO("inserted value on generic container");
            }
            catch(exception & ex){
                FAIL("Error: " << ex.what());
            }

        AND_THEN("GenericContainer should allow get the inserted values"){
            try{
                CHECK(container.get("int")    == initialIntValue);
                CHECK(container.get("bool")   == initialBoolValue);
                CHECK(container.get("long")   == initialLongValue);
                CHECK(container.get("double") == initialDoubleValue);
                CHECK(container.get("string") == initialStringValue);
                INFO("get value from generic container");
            }
            catch(exception & ex){
                FAIL("Error: " << ex.what());
            }
        }
        AND_THEN("GenericContainer should allow assigning the inserted values to correspondent variables"){
            try{

                int intValue = container.get("int").as<int>();
                bool boolValue = container.get("bool").as<bool>();
                long longValue = container.get("long").as<long>();
                double doubleValue = container.get("double").as<double>();
                string stringValue = container.get("string").as<string>();
                CHECK(intValue    == initialIntValue);
                CHECK(boolValue   == initialBoolValue);
                CHECK(longValue   == initialLongValue);
                CHECK(doubleValue == initialDoubleValue);
                CHECK(stringValue == initialStringValue);
            }
            catch(exception & ex){
                FAIL("Error: " << ex.what());
            }
        }
        AND_THEN("GenericContainer should allow putting a nested GenericContainer"){
            GenericContainer child;
            child.put("child_StrValue",string("asdasda"));
            container.put("child", child);
            container.put("otherValue", true);
            CHECK(container.get("child").as<GenericContainer>().get("child_StrValue")   == string("asdasda"));
            CHECK(container.get("otherValue") == true);
        }

        }
    }
    }
}
