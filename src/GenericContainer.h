#ifndef GENERICCONTAINER_H
#define GENERICCONTAINER_H

#include <map>
#include <string>

#include "utils/Value.h"
class GenericContainer
{
private:
    std::map<const std::string, Value> values;
    typedef std::map<const std::string, Value>::const_iterator ValuesConstIterator;
    typedef std::map<const std::string, Value>::iterator ValuesIterator;
public:
    void put(const std::string & key, const Value & value)
    {
        values[key] = value;
    }
    Value & get(const std::string & key){
        return values[key];
    }
    const Value & get(const std::string & key) const{
        return this->get(key, Value::getNullValue());
    }
    Value & get(const std::string & key, Value & defaultValue){
        ValuesIterator it = values.find(key);
        return it != values.end() ? it->second : defaultValue;
    }
    const Value & get(const std::string & key, const Value & defaultValue) const{
        ValuesConstIterator it = values.find(key);
        return it != values.end() ? it->second : defaultValue;
    }
    bool contains(const std::string & key) const{
        ValuesConstIterator it = values.find(key);
        return it != values.end();
    }
};

#endif // GENERICCONTAINER_H
