#ifndef GENERICSERIALIZER_HPP
#define GENERICSERIALIZER_HPP

#include "Serializer.h"

template<typename ConcreteSerializer>
class GenericSerializer : public Serializer{
private:
    ConcreteSerializer * serializer;
public:
    GenericSerializer(ConcreteSerializer * concreteSerializer=NULL)
        : serializer(concreteSerializer)
    {}
public:
    ConcreteSerializer *getSerializer() const     { return serializer; }
    void setSerializer(ConcreteSerializer *value) { serializer = value; }
public:
    virtual bool empty() const;
    virtual void clear();
    virtual void put(const std::string & name, const bool  & value);
    virtual void put(const std::string & name, const long long  & value);
    virtual void put(const std::string & name, const long  & value);
    virtual void put(const std::string & name, const int  & value);
    virtual void put(const std::string & name, const char  & value);
    virtual void put(const std::string & name, const float  & value);
    virtual void put(const std::string & name, const double  & value);
    virtual void put(const std::string & name, const char * value);
    virtual void put(const std::string & name, const std::string  & value);
    virtual void put(const bool  & value);
    virtual void put(const long long  & value);
    virtual void put(const long  & value);
    virtual void put(const int  & value);
    virtual void put(const char  & value);
    virtual void put(const float  & value);
    virtual void put(const double  & value);
    virtual void put(const char * value);
    virtual void put(const std::string  & value);
    virtual void put(const Serializable & serializable);

    virtual void putChild(const Serializable & serializable);
    virtual void putChild(const std::string & name, const Serializable & serializable);

    virtual void startCollection(const std::string & name);
    virtual void endCollection(const std::string & name);
    virtual void startCollection();
    virtual void endCollection();

    virtual void serialize(const Serializable & serializable);

    virtual std::ostream & extractTo(std::ostream & outStream) const;
    //friend std::ostream & operator<<(std::ostream & outStream, const Serializer & serializer);


    template<typename Iterator>
    void putCollection(const std::string & name, const Iterator & begin,const Iterator & end){
        this->startCollection(name);
        for(Iterator it=begin; it != end; ++it){
            this->put(*it);
        }
        this->endCollection(name);
    }

    template<typename Iterator>
    void putCollection(const Iterator & begin,const Iterator & end){
        this->startCollection();
        for(Iterator it=begin; it != end; ++it){
            this->put(*it);
        }
        this->endCollection();
    }
};

template<typename T>
bool GenericSerializer<T>::empty() const{
    return serializer->empty();
}
template<typename T>
void GenericSerializer<T>::clear(){
    serializer->clear();
}

template<typename T>
void GenericSerializer<T>::put(const std::string &name, const bool &value)
{ serializer->put(name, value);}
template<typename T>
void GenericSerializer<T>::put(const std::string &name, const double &value)
{ serializer->put(name, value);}
template<typename T>
void GenericSerializer<T>::put(const std::string &name, const long long &value)
{ serializer->put(name, value);}
template<typename T>
void GenericSerializer<T>::put(const std::string &name, const long &value)
{ serializer->put(name, (long long)value);}
template<typename T>
void GenericSerializer<T>::put(const std::string &name, const int &value)
{ serializer->put(name, value);}
template<typename T>
void GenericSerializer<T>::put(const std::string &name, const char &value)
{ serializer->put(name, value);}
template<typename T>
void GenericSerializer<T>::put(const std::string &name, const float &value)
{ serializer->put(name, value);}
template<typename T>
void GenericSerializer<T>::put(const std::string &name, const char *value)
{ this->put(name, std::string(value));}
template<typename T>
void GenericSerializer<T>::put(const std::string &name, const std::string &value)
{ serializer->put(name, value);}

template<typename T>
void GenericSerializer<T>::put(const bool &value)
{ serializer->put(value);}
template<typename T>
void GenericSerializer<T>::put(const long long &value)
{ serializer->put(value);}
template<typename T>
void GenericSerializer<T>::put(const long &value)
{ this->put((long long)value);}
template<typename T>
void GenericSerializer<T>::put(const int &value)
{ serializer->put(value);}
template<typename T>
void GenericSerializer<T>::put(const char &value)
{ serializer->put(value);}
template<typename T>
void GenericSerializer<T>::put(const float &value)
{ serializer->put(value);}
template<typename T>
void GenericSerializer<T>::put(const double &value)
{ serializer->put(value);}
template<typename T>
void GenericSerializer<T>::put(const char *value)
{ serializer->put(value);}
template<typename T>
void GenericSerializer<T>::put(const std::string &value)
{ serializer->put(value);}
template<typename T>
void GenericSerializer<T>::put(const Serializable &serializable){
    putChild(serializable);
}

template<typename T>
void GenericSerializer<T>::putChild(const std::string &name, const Serializable &serializable)
{ serializer->putChild(name, serializable);}
template<typename T>
void GenericSerializer<T>::putChild(const Serializable &serializable)
{ serializer->putChild(serializable);}

template<typename T>
void GenericSerializer<T>::startCollection(){
    serializer->startCollection();
}
template<typename T>
void GenericSerializer<T>::startCollection(const std::string &name){
    serializer->startCollection(name);
}
template<typename T>
void GenericSerializer<T>::endCollection(){
    serializer->endCollection();
}
template<typename T>
void GenericSerializer<T>::endCollection(const std::string &name){
    serializer->endCollection(name);
}


template<typename T>
void GenericSerializer<T>::serialize(const Serializable &serializable){
    serializable.serialize(*this);
}

template<typename T>
std::ostream &GenericSerializer<T>::extractTo(std::ostream &outStream) const
{
    return serializer->extractTo(outStream);
}

#endif // GENERICSERIALIZER_HPP


