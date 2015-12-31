#ifndef GENERICITERATOR_H
#define GENERICITERATOR_H

#include <vector>
#include "utils/Value.h"

class GenericIterator
{
private:
    int index;
    std::vector<Value> values;
public:
    GenericIterator()
    {
        toBegin();
    }

    bool hasNext() const{
        return index + 1 < (int)values.size();
    }
    bool next(){
        if(hasNext()){
            ++index;
            return true;
        }
        return false;
    }
    GenericIterator & toBegin(){
        index = -1;
        return *this;
    }

    const Value & getValue() const{
        return values[index];
    }

    void add(const Value & value){
        values.push_back(value);
    }

    template<typename OutputIterator, typename Converter>
    void asCollection(OutputIterator & outIter, Converter converter){
        this->toBegin();
        while(next()){
            *outIter++ = converter(getValue());
        }
    }

    template<typename T>
    class NumberConverter{
    public:
        T operator()(const Value & v) const{
            return v.asNumber<T>();
        }
    };
};

#endif // GENERICITERATOR_H
