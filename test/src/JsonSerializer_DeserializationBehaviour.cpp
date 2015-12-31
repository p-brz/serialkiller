#include "catch.hpp"
#include "json/json.h"
#include <sstream>
#include <algorithm>
#include <string>

#include "serialization/json/JsonSerializer.h"
#include "mocks/TestSerializables.hpp"
#include "helpers/StreamsHelper.h"

using namespace std;

namespace Constants {
    static const char * BOOL_NAME = "bool_value";
    static const char * BOOL_VALUE_STR = "true";
    static const bool BOOL_VALUE = true;

    static const char * nullProperty = "nullProperty";
    static const int DEFAULT_INTEGER = 10;
    static const char * DEFAULT_CSTRING = "lorem ipsum";
    static const string DEFAULT_STRING = "lorem ipsum";
}//namespace

class JsonDeserializerFixture{
public:
    stringstream helperStrStream;
    Json::Reader reader;
    Json::Value tmpValue;
    JsonSerializer jsonSerializer;

    TestSerializable makeSerializable(){
        TestSerializable serializable;
        serializable.char_value = 'b';
        serializable.int_value = 1000;
        serializable.double_value = 0.0001;
        serializable.long_value = 100000000l;
        return serializable;
    }

    string getSerializableString(Serializable & serializable){
        jsonSerializer.clear();
        serializable.serialize(jsonSerializer);
        helperStrStream << jsonSerializer;

        return helperStrStream.str();
    }

    void assertTestSerializableEquals(const TestSerializable & serializable1, const TestSerializable & serializable2){
        CHECK(serializable1.char_value == serializable2.char_value);
        CHECK(serializable1.cstrValue == serializable2.cstrValue);
        CHECK(serializable1.double_value == serializable2.double_value);
        CHECK(serializable1.int_value == serializable2.int_value);
    }

    Json::Value makeNullObject(){
        Json::Value jsonValue(Json::objectValue);
        jsonValue[Constants::nullProperty] = Json::Value(Json::nullValue);
        return jsonValue;
    }
};

SCENARIO("Json deserialization", "[JsonSerializer]"){
    JsonDeserializerFixture fixture;
GIVEN("a JsonSerializer instance"){
    JsonSerializer & jsonSerializer = fixture.jsonSerializer;

GIVEN("a serializable instance"){
    TestSerializable serializable = fixture.makeSerializable();
GIVEN("a string with a serialized json version of that instance"){
    std::string serializedJson = fixture.getSerializableString(serializable);
    INFO("Serialized value: " << serializedJson);

WHEN("calling deserialize passing that string"){
    jsonSerializer.deserialize(serializedJson);

THEN("the serializer content should be equals to the serialized json"){
    string deserializedContent;
    jsonSerializer >> deserializedContent;
    CHECK(deserializedContent == serializedJson);
}//THEN

AND_WHEN("call deserialize using another serializable instance"){
    TestSerializable otherSerializable;
    jsonSerializer.deserialize(otherSerializable);

THEN("the values of new instance should be the same of the other"){
    CHECK(serializable.char_value == otherSerializable.char_value);
    CHECK(serializable.int_value == otherSerializable.int_value);
    CHECK(serializable.double_value == otherSerializable.double_value);
    CHECK(serializable.long_value == otherSerializable.long_value);
}//THEN
}//WHEN
}//WHEN
}//GIVEN


GIVEN("a (de)serializable instance with (de)serializable attributes"){
    ParentSerializable parentSerializable;
    parentSerializable.child1 = serializable;
GIVEN("a serialized json version of that instance"){
    parentSerializable.serialize(jsonSerializer);
    fixture.helperStrStream << jsonSerializer;
    INFO("Serialized value: " << fixture.helperStrStream.str());

WHEN("calling deserializing to another instance"){
    jsonSerializer.deserialize(fixture.helperStrStream.str());
    ParentSerializable otherParent;
    jsonSerializer.deserialize(otherParent);
THEN("the child values should be deserialized correctly"){
    fixture.assertTestSerializableEquals(parentSerializable.child1, otherParent.child1);
    fixture.assertTestSerializableEquals(parentSerializable.child2, otherParent.child2);
}
    INFO("Serialized: " <<  (jsonSerializer << parentSerializable)
      << "\nDeserialized: " << (jsonSerializer << otherParent));
}
}//GIVEN
}//GIVEN
}//GIVEN
}//GIVEN
}//SCENARIO

SCENARIO("Deserializer null value", "[JsonSerializer]"){
    JsonDeserializerFixture fixture;
GIVEN("an json object with an null value"){
    Json::Value objNullValue = fixture.makeNullObject();

WHEN("deserializing it"){
    objNullValue >> fixture.jsonSerializer;

WHEN("getting the null value"){
    Value nullValue = fixture.jsonSerializer.get(Constants::nullProperty);

THEN("it should return an valid Value with an null content"){
    CHECK(nullValue.isEmpty());

    WHEN("trying to convert it to a number"){
    THEN("it return 0"){
        CHECK(nullValue.asNumber<int>() == 0);
    }//THEN
    }//WHEN

    WHEN("trying to convert it to a string"){
    THEN("it should return an empty string"){
        CHECK(nullValue.asString() == string());
    }//THEN
    }//WHEN

    WHEN("trying to make other conversions"){
    THEN("it should throw an Exception"){
        CHECK_THROWS_AS(nullValue.as<int>(), IllegalConversionException);
        CHECK_THROWS_AS(nullValue.as<string>(), IllegalConversionException);
    }//THEN
    }//WHEN


    WHEN("trying to convert it to some type, and passing an default value"){
    THEN("it should return the default value"){
        CHECK(nullValue.asNumber<int>(Constants::DEFAULT_INTEGER) == Constants::DEFAULT_INTEGER);
        CHECK(nullValue.asString(Constants::DEFAULT_STRING) == Constants::DEFAULT_STRING);
        CHECK(nullValue.as<bool>(false) == false);
    }//THEN
    }//WHEN

}//THEN
}//WHEN
}//WHEN
}//GIVEN
}//SCENARIO
