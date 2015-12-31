#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <string>
#include <cstdlib>
#include <istream>
#include "utils/Value.h"
#include "Deserializable.hpp"
#include "GenericIterator.h"
#include "GenericContainer.h"

class Deserializer
{
public:
    virtual ~Deserializer(){}
    virtual bool contains(const std::string & member) const =0;
    virtual Value get(const std::string & name)=0;
    virtual Value get(const std::string & name, const Value & defaultValue);
    virtual std::istream& deserialize(std::istream& is)=0;
    virtual Deserializer& deserialize(const std::string& str);

    virtual void deserialize(Deserializable & deserializable);


    virtual GenericContainer & getDeserialized()=0;
    virtual GenericIterator getAsCollection()=0;

    /** Try deserialize recursively property with name 'name', passing it do deserializable. */
    virtual void getChild(const std::string & name, Deserializable & deserializable)=0;
    virtual GenericIterator getChildIterator(const std::string & name)=0;

    template<typename DeserializableType, typename OutputIterator>
    void getChildCollection(const std::string & name, OutputIterator iter);

    template<typename OutputIterator, typename Converter>
    void getChildCollection(const std::string & name, OutputIterator iter, const Converter & converter);

    virtual int getInt(const std::string & name){
        return get(name).asNumber<int>();
    }
    virtual long getLong(const std::string & name){
        return this->get(name).asNumber<long>();
    }
    virtual double getReal(const std::string & name){
        return this->get(name).asNumber<double>();
    }
    virtual std::string getString(const std::string & name){
        return get(name).asString();
    }

    friend Deserializer& operator>>(const std::string & str, Deserializer& deserializer);
    friend Deserializer& operator>>(std::istream& is, Deserializer& deserializer);
    friend Deserializer& operator>>(Deserializer& deserializer, Deserializable & deserializable);
};

template<typename DeserializableType>
DeserializableType converter(const Value & value){
    DeserializableType deserializable;
    if(value.isTypeOf<GenericContainer>()){
        deserializable.deserialize(value.as<GenericContainer>());
    }
    return deserializable;
}

template<typename DeserializableType, typename OutputIterator>
void Deserializer::getChildCollection(const std::string &name, OutputIterator iter){
    GenericIterator genericIterator = this->getChildIterator(name);
    genericIterator.asCollection(iter, converter<DeserializableType>);
}

template<typename OutputIterator, typename Converter>
void Deserializer::getChildCollection(const std::string & name, OutputIterator iter, const Converter & converter){
    this->getChildIterator(name).asCollection(iter, converter);
}

#endif // DESERIALIZER_H
