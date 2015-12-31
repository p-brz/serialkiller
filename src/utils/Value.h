#ifndef VALUE_H
#define VALUE_H

#include <typeinfo>
#include <stddef.h> //NULL
#include <cstdlib> //int types
#include "Exception.hpp"

EXCEPTION_CLASS(IllegalConversionException);

class ValueHolder{
public:
    virtual ~ValueHolder(){

    }

    template<typename OtherType>
    bool isTypeOf(){
        return getTypeId() == typeid(OtherType);
    }
    virtual const std::type_info & getTypeId() const =0;
    virtual bool isNumber() const =0;
    virtual bool isPrimitive() const =0;
    virtual bool isReal() const =0;
    virtual ValueHolder * clone() const =0;
};
template<typename T>
class GenericValueHolder : public ValueHolder{
private:
    T value;
    const std::type_info * typeInfo;
    bool number;
    bool primitive;
    bool real;
public:
    GenericValueHolder(const T & someValue)
        : value(someValue)
        , typeInfo(&typeid(T))
    {
        primitive = false;
        number = false;
        real = false;
        if(typeid(T) == typeid(char)
            || typeid(T) == typeid(unsigned char)
            || typeid(T) == typeid(short)
            || typeid(T) == typeid(unsigned short)
            || typeid(T) == typeid(int)
            || typeid(T) == typeid(unsigned int)
            || typeid(T) == typeid(long)
            || typeid(T) == typeid(unsigned long)
            || typeid(T) == typeid(long long)
            || typeid(T) == typeid(unsigned long long) )
        {
            number = true;
            primitive = true;
        }
        else if(typeid(T) == typeid(float)
                || typeid(T) == typeid(double))
        {
            number = true;
            primitive = true;
            real = true;
        }
        else if(typeid(T) == typeid(bool))
        {
            number = false;
            primitive = true;
        }
    }
    virtual ValueHolder * clone() const{
        return new GenericValueHolder(this->value);
    }
    const T & getValue() const{
        return value;
    }
    T & getValue(){
        return value;
    }
    template<typename OtherType>
    bool isTypeOf(){
        return getTypeId() == typeid(OtherType);
    }

    template<typename OtherType>
    T cast(){
        return (T)value;
    }

    const std::type_info & getTypeId() const{
        return *typeInfo;
    }
    bool isNumber() const { return number; }
    bool isPrimitive() const { return primitive; }
    bool isReal() const { return real; }
};

class Value{
public://Static
    static const Value & getNullValue(){
        static Value nullValue;
        return nullValue;
    }

private:
    ValueHolder * value;
public:
    class NullValue{};

    Value(){
        value = new GenericValueHolder<NullValue>(NullValue());
    }
    Value(const Value & other){
        value = other.value != NULL ? other.value->clone() : NULL;
    }
    template<typename T>
    Value(const T & someValue){
        value = new GenericValueHolder<T>(someValue);
    }
    template <typename T, size_t N>
    Value(T (&someValue)[N] ) {
        value = new GenericValueHolder<T *>((T *)someValue);
    }
    template <typename T, size_t N>
    Value( const T (&someValue)[N] ) {
        value = new GenericValueHolder<const T *>((const T *)someValue);
    }

    virtual ~Value(){
        if(value != NULL){
            delete value;
            value = NULL;
        }
    }

    Value & operator=(const Value & value){
        if(this != &value){
            Value copy = Value(value);
            swap(copy);
        }
        return *this;
    }

    void swap(Value & value){
        std::swap(this->value, value.value);
    }

    bool isEmpty() const{
        return value->isTypeOf<NullValue>();
    }
    template<typename T>
    bool isTypeOf() const{
        return value->isTypeOf<T>();
    }

    template<typename T>
    T& as(){
        return getValue<T>();
    }
    template<typename T>
    const T& as() const{
        return getValue<T>();
    }
    template<typename T>
    T& as(T & defaultValue){
        return getValue<T>(defaultValue);
    }
    template<typename T>
    const T& as(const T & defaultValue) const{
        return getValue<T>(defaultValue);
    }

    template<typename T>
    T& getValue(){
        return (T&)getValueImplementation<T>();
    }
    template<typename T>
    const T& getValue() const{
        return getValueImplementation<T>();
    }
    template<typename T>
    const T & getValue(const T & defaultValue) const{
        return getValueImplementation<T>(false, &defaultValue);
    }
    template<typename T>
    T & getValue(T & defaultValue){
        return (T &)getValueImplementation<T>(false, &defaultValue);
    }

    virtual std::string asString() const{
        std::string result;
        if(!tryCastToString(result)){
            throw IllegalConversionException("Cannot convert value to string");
        }
        return result;
    }
    virtual std::string asString(const std::string & defaultValue) const{
        std::string result;
        if(isEmpty() || !tryCastToString(result)){
            return defaultValue;
        }
        return result;
    }

    bool tryCastToString(std::string & result) const
    {
        if(isEmpty()){
            result = std::string();
            return true;
        }
        return  this->tryCast<std::string, std::string>(result)
                ||
                this->tryCast<const char *, std::string>(result)
                ||
                this->tryCast<char *, std::string>(result);
    }

    template<typename Number>
    Number asNumber() const{
        Number result;
        if(!tryCastToNumber(result)){
            throw IllegalConversionException("Cannot convert value to integer");
        }
        return result;
    }

    template<typename Number>
    Number asNumber(const Number & defaultValue) const{
        Number result;
        if(isEmpty() || !tryCastToNumber(result)){
            return defaultValue;
        }
        return result;
    }

    template<typename Number>
    bool tryCastToNumber(Number & result) const{
        if(isEmpty()){
            result = 0;
            return true;
        }
        bool converted =
                this->tryCast<Number, Number>(result)
                ||
                this->tryCast<u_int32_t, Number>(result)
                ||
                this->tryCast<int32_t, Number>(result)
                ||
                this->tryCast<int64_t, Number>(result)
                ||
                this->tryCast<u_int64_t, Number>(result)
                ||
                this->tryCast<int16_t, Number>(result)
                ||
                this->tryCast<u_int16_t, Number>(result)
                ||
                this->tryCast<int8_t, Number>(result)
                ||
                this->tryCast<u_int8_t, Number>(result)
                ||
                this->tryCast<float, Number>(result)
                ||
                this->tryCast<double, Number>(result);

        return converted;
    }

    template<typename T>
    const T& into(T & outValue) const{
        outValue = getValueImplementation<T>();
        return outValue;
    }
    template<typename FromType, typename ToType>
    bool tryCast(ToType & value) const{
        if(this->isTypeOf<ToType>()){
            value = getValue<ToType>();
            return true;
        }else if(this->isTypeOf<FromType>()){
            value = (ToType)getValue<FromType>();
            return true;
        }
        return false;
    }
    template<typename T>
    T& coerceTo(){
        return (T&)getValueImplementation<T>(false);
    }
    template<typename T>
    const T& coerceTo() const{
        return getValueImplementation<T>(false);
    }

    const std::type_info & getTypeId() const{
        return this->value->getTypeId();
    }

    template<typename T>
    bool operator==(const T& otherValue) const{
        if(this->value != NULL && this->value->isTypeOf<T>()){
            return this->getValue<T>() == otherValue;
        }
        return false;
    }
private:
    template<typename T>
    const T& getValueImplementation(bool forceSameType=true, const T* defaultValue=NULL) const{

        if(value == NULL || typeid(T) != value->getTypeId()){
            if(forceSameType){
                throw IllegalConversionException(value == NULL ? "Value is null" : "Type mismatch!");
            }
            else if(defaultValue != NULL){
                return *defaultValue;
            }
        }

        GenericValueHolder<T> * valueHolder = static_cast<GenericValueHolder<T> *>(value);
        return valueHolder->getValue();
    }
};

#endif // VALUE_H
