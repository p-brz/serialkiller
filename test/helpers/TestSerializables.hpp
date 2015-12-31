#ifndef TESTSERIALIZABLES_HPP
#define TESTSERIALIZABLES_HPP

#include "serialization/Deserializable.hpp"
#include "serialization/Deserializer.h"
#include <string>

class TestSerializable : public Serializable, public Deserializable{
public:
    int int_value;
    std::string cstrValue;
    char char_value;
    long long_value;
    double double_value;

    TestSerializable()
        : int_value(100)
        , cstrValue("Cstring")
        , char_value('a')
        , long_value(100000)
        , double_value(1.00100)
    {}

    bool operator==(const TestSerializable & other) const{
        return int_value == other.int_value
                && cstrValue == other.cstrValue
                && char_value == other.char_value
                && long_value == other.long_value
                && double_value == other.double_value;
    }

    virtual void serialize(Serializer & serializer) const{
        serializer.put("int_value",int_value);
        serializer.put("cstrValue",cstrValue.c_str());
        serializer.put("char_value",char_value);
        serializer.put("long_value",long_value);
        serializer.put("double_value",double_value);
    }

    /*virtual void deserialize(Deserializer & deserializer){
        int_value = deserializer.getInt("int_value");
        //deserializer.get("cstrValue").into(cstrValue);
        char_value = (char)deserializer.getInt("char_value");
        long_value = deserializer.getLong("long_value");
        double_value = deserializer.getReal("double_value");
    }*/

    void deserialize(const GenericContainer & container){
        int_value = container.get("int_value").asNumber<int>();
        char_value = container.get("char_value").asNumber<char>();
        long_value = container.get("long_value").asNumber<long>();
        double_value = container.get("double_value").asNumber<double>();
        cstrValue = container.get("cstrValue").asString();
    }
};
class ParentSerializable : public Serializable, public Deserializable{
public:
    TestSerializable child1, child2;

    void serialize(Serializer & serializer) const{
        serializer.putChild("child1",child1);
        serializer.putChild("child2",child2);
    }
    /*void deserialize(Deserializer & deserializer){
        deserializer.getChild(std::string("child1"),this->child1);
        deserializer.getChild("child2",this->child2);
    }*/
    void deserialize(const GenericContainer & container){
        child1.deserialize(container.get("child1").as<GenericContainer>());
        child2.deserialize(container.get("child2").as<GenericContainer>());
    }
};


#endif // TESTSERIALIZABLES_HPP
