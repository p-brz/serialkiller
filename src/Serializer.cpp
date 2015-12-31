#include "Serializer.h"
#include <sstream>

std::string &Serializer::extractTo(std::string &outString) const{
    std::stringstream sstream;
    this->extractTo(sstream);
    outString = sstream.str();
    return outString;
}

std::ostream & operator<<(std::ostream & outStream, const Serializer & serializer){
    serializer.extractTo(outStream);
    return outStream;
}

const Serializer &operator>>(const Serializer &serializer, std::string &outString)
{
    serializer.extractTo(outString);
    return serializer;
}

Serializer & serializeFrom(const Serializable& serializable, Serializer& serializer)
{
    serializer.clear();
    serializer.serialize(serializable);
    return serializer;
}

Serializer & operator<<(Serializer & serializer, const Serializable & serializable){
    return serializeFrom(serializable, serializer);
}

Serializer & operator>>(const Serializable &serializable, Serializer &serializer)
{
    return serializeFrom(serializable, serializer);
}
