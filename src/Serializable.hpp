#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

class Serializer;
class Serializable{
public:
    virtual ~Serializable(){}
    virtual void serialize(Serializer & serializer) const =0;
};

#endif // SERIALIZABLE_HPP
