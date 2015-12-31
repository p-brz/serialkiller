#include "JsonSerializer.h"

#include "CommonExceptions.h"

using namespace CommonExceptions;
using CommonExceptions::ParseException;

Json::Value & operator<<(Json::Value & value, const JsonSerializer & serializer){
    return serializer.extractTo(value);
}
JsonSerializer & operator>>(JsonSerializer & serializer, Json::Value & value){
    serializer.extractTo(value);
    return serializer;
}

JsonSerializer &operator>>(Json::Value &value, JsonSerializer &serializer)
{
    serializer.deserialize(value);
    return serializer;
}

JsonSerializer &operator>>(const Json::Value &value, JsonSerializer &serializer)
{
    serializer.deserialize(value);
    return serializer;
}


bool JsonSerializer::getPrettyPrint() const
{
    return prettyPrint;
}

void JsonSerializer::setPrettyPrint(bool value)
{
    prettyPrint = value;
}
JsonSerializer::JsonSerializer()
    : rootJson(Json::nullValue)
    , currentIndex(0)
    , deserializedRoot(GenericContainer())
    , prettyPrint(true)
{
    valuesStack.push_back(&rootJson);

    this->setSerializer(this);

    deserializedStack.push_back(&deserializedRoot);
}

void JsonSerializer::clear(){
    rootJson = Json::Value(Json::nullValue);
    currentIndex = 0;
    valuesStack.clear();
    valuesStack.push_back(&rootJson);
    deserializedRoot = GenericContainer();
    deserializedStack.clear();
    deserializedStack.push_back(&deserializedRoot);
}

std::ostream &JsonSerializer::extractTo(std::ostream & outStream){
    //outStream << rootJson;
    if(getPrettyPrint()){
        Json::StyledStreamWriter jsonWriter;
        jsonWriter.write(outStream, this->rootJson);
    }
    else{
//        Json::StreamWriterBuilder builder;
//        builder["identation"] = "";
//        Json::StreamWriter * writer = builder.newStreamWriter();
//        writer->write(rootJson, &outStream);
//        delete writer;
        Json::FastWriter writer;
        outStream << writer.write(this->rootJson);
    }
    return outStream;
}

Json::Value &JsonSerializer::extractTo(Json::Value & outValue) const{
    outValue = rootJson;
    return outValue;
}

void JsonSerializer::startCollection(const std::string &name){
    pushJsonValue(new Json::Value(Json::arrayValue));
}


void JsonSerializer::endCollection(const std::string &name){
    Json::Value * previousValue = popJsonValue();
    getCurrent()[name] = *previousValue;
    delete previousValue;
}


void JsonSerializer::startCollection(){
    if(getCurrent().type() == Json::arrayValue){
        pushJsonValue(new Json::Value());
    }
}

void JsonSerializer::endCollection(){
    //Se "pai" é json, então um novo filho foi inserido na cadeia
    if(currentIndex > 0 && valuesStack[currentIndex - 1]->type() == Json::arrayValue){
        Json::Value * previousValue = popJsonValue();
        getCurrent().append(*previousValue);
        delete previousValue;
    }
}

GenericContainer &JsonSerializer::getDeserialized(){
    return this->deserializedRoot.as<GenericContainer>();
}

GenericIterator JsonSerializer::getAsCollection(){
    return this->deserializedRoot.as<GenericIterator>();
}

std::string JsonSerializer::jsonTypeToString(const Json::ValueType &valueType){
    switch (valueType) {
    case Json::arrayValue:
        return std::string("array");
    case Json::booleanValue:
        return std::string("boolean");
    case Json::intValue:
        return std::string("int");
    case Json::nullValue:
        return std::string("null");
    case Json::objectValue:
        return std::string("object");
    case Json::realValue:
        return std::string("real");
    case Json::stringValue:
        return std::string("string");
    case Json::uintValue:
        return std::string("uint");
    default:
        break;
    }
    return std::string("");
}

void JsonSerializer::deserialize(Deserializable &deserializable){
    Deserializer::deserialize(deserializable);
}

bool JsonSerializer::contains(const std::string &member) const
{
    return getCurrentDeserialized().as<GenericContainer>().contains(member);
}

const Value &JsonSerializer::get(const std::string &name) const{
    return getCurrentDeserialized().as<GenericContainer>().get(name);
}

void JsonSerializer::getChild(const std::string &name, Deserializable &deserializable){
    if(pushDeserializerValue(name)){
        deserializable.deserialize(getCurrentDeserialized().as<GenericContainer>());
        //deserializable.deserialize(*this);
        popDeserializerValue();
    }
}

GenericIterator JsonSerializer::getChildIterator(const std::string &name){
    Value child = this->get(name);
    if(child.isTypeOf<GenericIterator>()){
        return this->get(name).as<GenericIterator>();
    }
    return GenericIterator();
}

Value &JsonSerializer::getCurrentDeserialized(){
    return *deserializedStack[deserializedStack.size() -1];
}

const Value &JsonSerializer::getCurrentDeserialized() const{
    return *deserializedStack[deserializedStack.size() -1];
}

bool JsonSerializer::pushDeserializerValue(const std::string &name){
    GenericContainer & container = getCurrentDeserialized().as<GenericContainer>();
    if(container.contains(name)){
        deserializedStack.push_back(&container.get(name));
        return true;
    }
    return false;
}

void JsonSerializer::popDeserializerValue(){
    deserializedStack.pop_back();
}

Value JsonSerializer::get(const std::string &name){
    return getCurrentDeserialized().as<GenericContainer>().get(name);
}

void JsonSerializer::deserialize(Deserializable &deserializable, const Json::Value &value){
    this->deserialize(value);
    //deserializable.deserialize(*this);
    deserializable.deserialize(getCurrentDeserialized().as<GenericContainer>());
}

void JsonSerializer::deserialize(const Json::Value &value){
    if(value.type() != Json::objectValue && value.type() != Json::arrayValue){
        std::stringstream strStream;
        strStream << "invalid json type. Can only deserialize arrays or objects json!"
                  << "But type was: "
                  << jsonTypeToString(value.type());
        throw ParseException(strStream.str());
    }
    this->clear();
    this->rootJson = value;
    deserializedRoot = convertJsonValue(value);
}

Deserializer &JsonSerializer::deserialize(const std::string &value){
    Json::Value jsonValue;
    Json::Reader reader;
    if(reader.parse(value, jsonValue)){
        deserialize(jsonValue);
    }
    else{
        throw ParseException("Invalid Json");
    }

    return *this;
}

void JsonSerializer::deserialize(const char *jsonStr){
    deserialize(std::string(jsonStr));
}

std::istream &JsonSerializer::deserialize(std::istream &inStream){
    Json::Value jsonValue;
    Json::Reader reader;
    reader.parse(inStream, jsonValue);
    deserialize(jsonValue);

    return inStream;
}

Json::Value &JsonSerializer::getCurrent(){
    return *this->valuesStack[currentIndex];
}

void JsonSerializer::pushJsonValue(Json::Value *value){
    this->valuesStack.push_back(value);
    ++currentIndex;
}

Json::Value *JsonSerializer::popJsonValue(){
    Json::Value * previous = &getCurrent();
    this->valuesStack.pop_back();
    --currentIndex;
    return previous;
}

Value JsonSerializer::convertJsonValue(const Json::Value &jsonValue) const{
    if(jsonValue.type() == Json::objectValue){
        GenericContainer container;
        convertJsonObject(container, jsonValue);
        return container;
    }
    else if(jsonValue.type() == Json::arrayValue){
        return convertJsonArray(jsonValue);
    }
    return Value();
}

void JsonSerializer::convertJsonObject(GenericContainer &container, const Json::Value &jsonValue) const{
    Json::Value::Members members = jsonValue.getMemberNames();
    for(Json::Value::Members::const_iterator it = members.begin(); it != members.end(); ++it){
        const Json::Value & member = jsonValue[*it];
        container.put(*it, convertJson(member));
    }
}

Value JsonSerializer::convertJsonArray(const Json::Value &jsonValue) const{
    GenericIterator genericIterator;

    for(Json::ArrayIndex i = 0; i < jsonValue.size(); ++i){
        genericIterator.add(convertJson(jsonValue[i]));
    }

    return genericIterator;
}

Value JsonSerializer::convertJson(const Json::Value &jsonValue) const{
    Value value;
    switch (jsonValue.type()) {
    case Json::arrayValue :{
        value = convertJsonArray(jsonValue);
        break;
    }
    case Json::objectValue : { //Brackets are needed because of initialization of a variable on the case
        GenericContainer container;
        convertJsonObject(container, jsonValue);
        value = container;
        break;
    }
    case Json::booleanValue :
        value = jsonValue.asBool();
        break;
    case Json::intValue :
    case Json::uintValue :
        value = jsonValue.asInt();
        break;
    case Json::realValue :
        value = jsonValue.asDouble();
        break;
    case Json::stringValue :
        value = jsonValue.asString();
        break;
    default:
        break;
    }

    return value;
}


JsonSerializer &operator>>(const std::string &str, JsonSerializer &serializer){
    serializer.deserialize(str);
    return serializer;
}

JsonSerializer &operator>>(const Serializable &serializable, JsonSerializer &serializer)
{
    serializable.serialize(serializer);
    return serializer;
}
