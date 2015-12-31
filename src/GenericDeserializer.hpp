#ifndef GENERICDESERIALIZER_HPP
#define GENERICDESERIALIZER_HPP

#include "Deserializer.h"

template<typename ConcreteDeserializer>
class GenericDeserializer : public Deserializer{
private:
    ConcreteDeserializer * deserializer;
public:
    GenericDeserializer(ConcreteDeserializer * concreteSerializer=NULL)
        : deserializer(concreteSerializer)
    {}
public:
    const ConcreteDeserializer *getDeserializer() const     { return deserializer; }
    ConcreteDeserializer *getDeserializer()                 { return deserializer; }
    void setDeserializer(ConcreteDeserializer *value) { deserializer = value; }
public:
    virtual char & get(const std::string & name, char & outValue)
    {return getDeserializer()->get(name, outValue);}
    virtual int & get(const std::string & name, int & outValue)
    {return getDeserializer()->get(name, outValue);}
    virtual long & get(const std::string & name, long & outValue)
    {return getDeserializer()->get(name, outValue);}
    virtual double & get(const std::string & name, double & outValue)
    {return getDeserializer()->get(name, outValue);}
    virtual std::string & get(const std::string & name, std::string &  outValue)
    {return getDeserializer()->get(name, outValue);}
    virtual Value get(const std::string & name)
    {return getDeserializer()->get(name);}
};

#endif // GENERICDESERIALIZER_HPP

