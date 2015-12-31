
#include "catch.hpp"
#include "json/json.h"
#include <sstream>
#include <algorithm>

#include "serialization/json/JsonSerializer.h"
#include "mocks/TestSerializables.hpp"
#include "helpers/StreamsHelper.h"

using namespace std;

#define BOOL_NAME "bool_value"
#define BOOL_VALUE_STR "true"
#define BOOL_VALUE true

SCENARIO("Json serialization", "[JsonSerializer]"){
    stringstream helperStrStream;
    Json::Reader reader;
    Json::Value tmpValue;
    GIVEN("a JsonSerializer instance"){
        JsonSerializer jsonSerializer;
        THEN("it should be empty"){
            CHECK(jsonSerializer.empty());
        }
        WHEN("putting some boolean value with a string key : \"" BOOL_NAME "\" and value: " BOOL_VALUE_STR){
            jsonSerializer.put(BOOL_NAME, BOOL_VALUE);

            THEN("it should not be empty"){
                CHECK(!jsonSerializer.empty());
            }
            AND_WHEN("calling serialize"){
                jsonSerializer.extractTo(helperStrStream);
                THEN("it should return a valid json object"){
                    CHECK(reader.parse(helperStrStream, tmpValue));
                    CHECK(tmpValue.type() == Json::objectValue);
                AND_THEN("the object should contain a boolean parameter \"" BOOL_NAME "\" with value: " BOOL_VALUE_STR){
                    CHECK(tmpValue.get(BOOL_NAME, !BOOL_VALUE) == BOOL_VALUE);
                }
                }
             }
            AND_WHEN("calling clear"){
                jsonSerializer.clear();
                THEN("it should be empty again"){
                    CHECK(jsonSerializer.empty());
                }
            }
        }
        WHEN("putting any primitive type"){
            jsonSerializer.put("integer", -1);
            jsonSerializer.put("double" , 0.01);
            jsonSerializer.put("float"  , 10.01f);
            jsonSerializer.put("cstring", "Hello world!");
            jsonSerializer.put("std::string", string("Hello world!"));
        AND_WHEN("extracting the serializer content"){
            jsonSerializer.extractTo(helperStrStream);
            THEN("it should serialize to a json object with correct types"){
                INFO("Serialized values: " << helperStrStream.str());

                CHECK(reader.parse(helperStrStream, tmpValue));
                CHECK(tmpValue.type() == Json::objectValue);
                CHECK(tmpValue["integer"].type()     == Json::intValue);
                CHECK(tmpValue["double"].type()      == Json::realValue);
                CHECK(tmpValue["float"].type()       == Json::realValue);
                CHECK(tmpValue["cstring"].type()     == Json::stringValue);
                CHECK(tmpValue["std::string"].type() == Json::stringValue);
            }
        }
        }
        WHEN("putting values without key"){
            jsonSerializer.put("valueString");
            jsonSerializer.put(100);
            jsonSerializer.put(true);

            THEN("it should store the serialized content as an array"){
                jsonSerializer.extractTo(helperStrStream);
                reader.parse(helperStrStream, tmpValue);

                CHECK(tmpValue.type() == Json::arrayValue);
                AND_THEN("should have been appended to the array"){
                    INFO("Serialized values: " << helperStrStream.str());
                    CHECK(tmpValue[0] == "valueString");
                    CHECK(tmpValue[1] == 100);
                    CHECK(tmpValue[2] == true);
                }
            }
            AND_WHEN("putting values with string key"){
                THEN("it should call an exception"){
                    CHECK_THROWS(jsonSerializer.put("evilKey", "anyValue"));
                }
            }
        }
        WHEN("putting an serializable with object childrens"){
            ParentSerializable serializable;
            jsonSerializer.putChild("child",serializable);

            THEN("it should create a subobject with the child content"){
                jsonSerializer.extractTo(helperStrStream);
                reader.parse(helperStrStream, tmpValue);

                CHECK(tmpValue.isMember("child"));
                CHECK(tmpValue["child"].type() == Json::objectValue);

                AND_THEN("this subobject should have children too"){
                    CHECK(tmpValue["child"].isMember("child1"));
                    CHECK(tmpValue["child"].isMember("child2"));
                    CHECK(tmpValue["child"]["child1"].type() == Json::objectValue);
                    CHECK(tmpValue["child"]["child2"].type() == Json::objectValue);
                }
            }
        }
        GIVEN("a serializable instance"){
            TestSerializable serializable;
            WHEN("calling serialize on it"){
                serializable.serialize(jsonSerializer);
            AND_WHEN("serialize the JsonSerializer to an output stream"){
                helperStrStream << jsonSerializer;
                THEN("it should output a json object"){
                    CHECK(reader.parse(helperStrStream, tmpValue));
                    CHECK(tmpValue.type() == Json::objectValue);
                AND_THEN("the json object should contain the serializable fields and values"){
                    CHECK(tmpValue["int_value"] == serializable.int_value);
                    CHECK(tmpValue["cstrValue"] == serializable.cstrValue);
                    CHECK(tmpValue["char_value"] == serializable.char_value);
                    long longvalue = tmpValue.get("long_value",1).asLargestInt();
                    CHECK(longvalue == serializable.long_value);
                    CHECK(tmpValue["double_value"] == serializable.double_value);
                }
                }
            }
            }
        }
        GIVEN("a collection of some primitive type"){
            long valuesArray[] = {10l, 20l,-1l, 50000, 0, 0};
            vector<long> values = vector<long>(&valuesArray[0], &valuesArray[6]);
            WHEN("calling putCollection on serializer, passing an identifier, the begin and the of iterator"){
                string identifier = "collection";
                jsonSerializer.putCollection(identifier, values.begin(), values.end());
            AND_WHEN("serialize the JsonSerializer to an output stream"){
                helperStrStream << jsonSerializer;
                THEN("it should output a json object with a json array"){
                    CHECK(reader.parse(helperStrStream, tmpValue));

                    INFO("parsed json: " << tmpValue);

                    CHECK(tmpValue.type() == Json::objectValue);
                    CHECK(tmpValue.isMember(identifier));
                    CHECK(tmpValue[identifier].type() == Json::arrayValue);

                AND_THEN("each json value on collection should correspond to the original values"){
                    for(int i=0; i < tmpValue[identifier].size(); ++i){
                        CHECK(valuesArray[i] == tmpValue[identifier][i].asInt64());
                    }
                }
                }
            }
            }
            WHEN("calling putCollection without key argument"){
                CHECK(jsonSerializer.empty());
                jsonSerializer.putCollection(values.begin(), values.end());
                THEN("it should serialize to a json array"){
                    helperStrStream << jsonSerializer;
                    reader.parse(helperStrStream, tmpValue);
                    INFO("parsed value:" <<tmpValue);
                    CHECK(tmpValue.type() == Json::arrayValue);

                AND_THEN("each json value on collection should correspond to the original values"){
                    for(int i=0; i < tmpValue.size(); ++i){
                        CHECK(valuesArray[i] == tmpValue[i].asInt64());
                    }
                }
                }
            AND_WHEN("serializer alread contains an array"){
                helperStrStream << jsonSerializer;
                reader.parse(helperStrStream, tmpValue);
                CHECK(tmpValue.type() == Json::arrayValue);
                StreamsHelper::clearStream(helperStrStream);

                jsonSerializer.putCollection(values.begin(), values.end());
                THEN("it should serialize to a json array with another json array inside"){
                    helperStrStream << jsonSerializer;
                    reader.parse(helperStrStream, tmpValue);
                    INFO("parsed value:" <<tmpValue);
                    REQUIRE(tmpValue.type() == Json::arrayValue);

                    int subcollectionIndex = tmpValue.size() -1;
                    REQUIRE(tmpValue[subcollectionIndex].type() == Json::arrayValue);

                AND_THEN("each json value on sub-collection should correspond to the original values"){
                    for(int i=0; i < tmpValue[subcollectionIndex].size(); ++i){
                        CHECK(values[i] == tmpValue[subcollectionIndex][i].asInt64());
                    }
                }
                }
            }
            AND_WHEN("serializer contains an object"){
                jsonSerializer.clear();
                jsonSerializer.put("qualquer_valor","qualquer_valor");

                helperStrStream << jsonSerializer;
                reader.parse(helperStrStream, tmpValue);
                CHECK(tmpValue.type() == Json::objectValue);
                StreamsHelper::clearStream(helperStrStream);

                THEN("it should throws an exception"){
                    CHECK_THROWS(jsonSerializer.putCollection(values.begin(), values.end()));
                }
            }
            }
        }
        GIVEN("a collection of serializable objects"){
            vector<ParentSerializable> values = vector<ParentSerializable>();
            for(int i=0; i < 6; ++i){
                values.push_back(ParentSerializable());
            }

            WHEN("calling putCollection"){
                string identifier = "collection";
                jsonSerializer.putCollection(identifier, values.begin(), values.end());
            AND_WHEN("serialize the JsonSerializer to an output stream"){
                helperStrStream << jsonSerializer;
                THEN("it should output a correct json object with a json array"){
                    CHECK(reader.parse(helperStrStream, tmpValue));

                    INFO("parsed json: " << tmpValue);

                    CHECK(tmpValue.type() == Json::objectValue);
                    CHECK(tmpValue.isMember(identifier));
                    CHECK(tmpValue[identifier].type() == Json::arrayValue);

                AND_THEN("each json value on collection should have the correct type"){
                    for(int i=0; i < tmpValue[identifier].size(); ++i){
                        CHECK(tmpValue[identifier][i].type() == Json::objectValue);
                    }
                }
                }
            }
            }
        }
    }
}
