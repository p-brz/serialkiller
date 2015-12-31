
#include "Deserializer.h"
#include "Deserializable.hpp"
#include <sstream>
#include "CommonExceptions.h"

Value Deserializer::get(const std::string &name, const Value &defaultValue)
{
    if(!this->contains(name)){
        return defaultValue;
    }
    return this->get(name);
}

Deserializer &Deserializer::deserialize(const std::string &str){
    std::stringstream sstream(str);
    this->deserialize(sstream);
    return *this;
}

void Deserializer::deserialize(Deserializable &deserializable){
    try{
        GenericContainer &container = this->getDeserialized();
        deserializable.deserialize(container);
    }
    catch(IllegalConversionException & ex){
        throw CommonExceptions::ParseException("Failed to deserialize deserializable", ex);
    }
}

Deserializer &operator>>(const std::string &str, Deserializer &deserializer){
    return deserializer.deserialize(str);
}

Deserializer &operator>>(std::istream &is, Deserializer &deserializer)
{
    deserializer.deserialize(is);
    return deserializer;
}

Deserializer &operator>>(Deserializer &deserializer, Deserializable &deserializable){
    deserializer.deserialize(deserializable);

    return deserializer;
}
