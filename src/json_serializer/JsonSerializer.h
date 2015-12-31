#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <iostream>
#include <sstream>

#include "json/json.h"
#include "../Serializer.h"
#include "../Deserializer.h"
#include "../Deserializable.hpp"
#include "../GenericSerializer.hpp"
#include "../GenericDeserializer.hpp"
#include "utils/Value.h"
#include "../GenericContainer.h"

class JsonSerializer :
          public GenericSerializer<JsonSerializer>
        , public Deserializer
{
    typedef GenericSerializer<JsonSerializer> superSerializer;
private:
    Json::Value rootJson;
    unsigned currentIndex;
    std::vector<Json::Value *> valuesStack;

    Value deserializedRoot;
    std::vector<Value *> deserializedStack;

    bool prettyPrint;
public:
    JsonSerializer();

    virtual bool empty() const{
        return rootJson.type() == Json::nullValue;
    }
    virtual void clear();

    bool getPrettyPrint() const;
    void setPrettyPrint(bool value);
public: //Serialization
    std::ostream & extractTo(std::ostream & outStream);
    Json::Value & extractTo(Json::Value & outValue) const;

    //Serializable values
    virtual void putChild(const std::string & name, const Serializable & serializable)
    {putChildValues(name, serializable);}
    virtual void putChild(const Serializable & serializable)
    {putChildValues(serializable);}

    //Collections
    virtual void startCollection(const std::string & name);
    virtual void endCollection(const std::string & name);
    virtual void startCollection();
    virtual void endCollection();

public: //Deserialization
    virtual GenericContainer & getDeserialized();
    virtual GenericIterator getAsCollection();

    virtual std::istream & deserialize(std::istream & inStream);
    virtual void deserialize(const char * jsonStr);
    virtual Deserializer & deserialize(const std::string & value);
    std::string jsonTypeToString(const Json::ValueType & valueType);

    virtual void deserialize(const Json::Value & value);
    virtual void deserialize(Deserializable & deserializable, const Json::Value & value);

    virtual void deserialize(Deserializable & deserializable);

    virtual bool contains(const std::string & member) const;
    virtual Value get(const std::string & name);
    virtual const Value & get(const std::string & name) const;
    virtual void getChild(const std::string & name, Deserializable & deserializable);
    virtual GenericIterator getChildIterator(const std::string & name);
protected:
    Value & getCurrentDeserialized();
    const Value & getCurrentDeserialized() const;

protected:
    bool pushDeserializerValue(const std::string & name);
    void popDeserializerValue();

public: //Concrete Serialization
    template<typename Key, typename Value>
    void put(const Key & name, const Value & value){
        getCurrent()[name] = value;
    }
    template<typename Key>
    void put(const Key & name, const long & value){
        getCurrent()[name] = (Json::Int64)value;
    }
    template<typename T>
    void put(const T & value){
        getCurrent().append(value);
    }

protected: //Concrete Serialization
    template<typename SerializableType>
    void putChildValues(const std::string & name, const SerializableType & serializable);
    template<typename SerializableType>
    void putChildValues(const SerializableType & serializable);

    Json::Value & getCurrent();
    void pushJsonValue(Json::Value * value);
    Json::Value * popJsonValue();
protected:
    Value convertJsonValue(const Json::Value & jsonValue) const;
    void convertJsonObject(GenericContainer & container, const Json::Value & jsonValue) const;
    Value convertJsonArray(const Json::Value & jsonValue) const;
    Value convertJson(const Json::Value & jsonValue) const;

};


Json::Value & operator<<(Json::Value & value, const JsonSerializer & serializer);
JsonSerializer & operator>>(JsonSerializer & serializer, Json::Value & value);
JsonSerializer & operator>>(const std::string & string, JsonSerializer & serializer);
JsonSerializer & operator>>(const Json::Value & value, JsonSerializer & serializer);
JsonSerializer & operator>>(Json::Value & value, JsonSerializer & serializer);
//Overload operator in Serializer.h to allow chaining with Json::Value
JsonSerializer & operator>>(const Serializable & serializable, JsonSerializer & serializer);


template<typename SerializableType>
void JsonSerializer::putChildValues(const std::string & name, const SerializableType & serializable){
    Json::Value childJson;
    pushJsonValue(&childJson);

    serializable.serialize(*this);

    popJsonValue();
    getCurrent()[name] = childJson;
}
template<typename SerializableType>
void JsonSerializer::putChildValues(const SerializableType & serializable){
    Json::Value childJson;

    pushJsonValue(&childJson);

    serializable.serialize(*this);

    popJsonValue();
    getCurrent().append(childJson);
}

#endif // JSONSERIALIZER_H
