#ifndef SERIALIZER_H
#define SERIALIZER_H


#include<iostream>
#include "Serializable.hpp"

class Serializer{
public:
    virtual ~Serializer(){}
    virtual bool empty() const =0;
    virtual void clear() = 0;
    virtual void put(const std::string & name, const bool  & value)=0;
    virtual void put(const std::string & name, const long long  & value)=0;
    virtual void put(const std::string & name, const long  & value){
        this->put(name,(long long)value);
    }
    virtual void put(const std::string & name, const int  & value)=0;
    virtual void put(const std::string & name, const char  & value)=0;
    virtual void put(const std::string & name, const float  & value)=0;
    virtual void put(const std::string & name, const double  & value)=0;
    virtual void put(const std::string & name, const char * value){
        this->put(name, std::string(value));
    }
    virtual void put(const std::string & name, const std::string  & value)=0;
    virtual void putChild(const std::string & name, const Serializable & serializable)=0;

    template<typename Iterator>
    void putCollection(const std::string & name, const Iterator & begin,const Iterator & end){
        this->startCollection(name);
        for(Iterator it=begin; it != end; ++it){
            this->put(*it);
        }
        this->endCollection(name);
    }
    virtual void startCollection(const std::string & name)=0;
    virtual void endCollection(const std::string & name)=0;

    template<typename Iterator>
    void putCollection(const Iterator & begin,const Iterator & end){
        this->startCollection();
        for(Iterator it=begin; it != end; ++it){
            this->put(*it);
        }
        this->endCollection();
    }
    virtual void startCollection()=0;
    virtual void endCollection()=0;

    virtual void put(const bool  & value)=0;
    virtual void put(const long long  & value)=0;
    virtual void put(const long  & value){
        this->put((long long)value);
    }
    virtual void put(const int  & value)=0;
    virtual void put(const char  & value)=0;
    virtual void put(const float  & value)=0;
    virtual void put(const double  & value)=0;
    virtual void put(const char * value){
        this->put(std::string(value));
    }
    virtual void put(const std::string  & value)=0;

    virtual void put(const Serializable & serializable){
        putChild(serializable);
    }

    virtual void putChild(const Serializable & serializable)=0;

    virtual void serialize(const Serializable & serializable){
        this->clear();
        serializable.serialize(*this);
    }

    virtual std::ostream & extractTo(std::ostream & outStream) const =0;
    virtual std::string &extractTo(std::string & outString) const;
    /** Put serialized content in serializer into output stream. */
    friend std::ostream & operator<<(std::ostream & outStream, const Serializer & serializer);

    /** Serialize {@link serializable} into {@link serializer}.
        Allow chaining with output. Ex.: cout << serializer << serializable
    */
    friend Serializer & operator<<(Serializer & serializer, const Serializable & serializable);
    /** Serialize {@link serializable} into {@link serializer}.*/
    friend Serializer & operator>>(const Serializable & serializable, Serializer & serializer);
    /** Put serialized content from {@link serializer} into {@link outString}.*/
    friend const Serializer & operator>>(const Serializer & serializer, std::string & outString);
};

#endif // SERIALIZER_H
