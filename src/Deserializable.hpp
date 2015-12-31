#ifndef DESERIALIZABLE_H
#define DESERIALIZABLE_H

#include "GenericContainer.h"

class Deserializable
{
public:
    virtual ~Deserializable(){}
    virtual void deserialize(const GenericContainer & deserializer)=0;
};

#endif // DESERIALIZABLE_H
