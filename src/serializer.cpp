#include <serializer.hpp>

Serializer::DeserializableType Serializer::deserialize_variant(ifstream &in) {
    TypeID type = TypeID(deserialize<char>(in));
    switch (type) {
        case TI_CHAR:
            return deserialize<char>(in);
            break;
        case TI_UNSIGNED_CHAR:
            return deserialize<unsigned char>(in);
            break;
        case TI_SHORT:
            return deserialize<short>(in);
            break;
        case TI_UNSIGNED_SHORT:
            return deserialize<unsigned short>(in);
            break;
        case TI_INT:
            return deserialize<int>(in);
            break;
        case TI_FLOAT:
            return deserialize<float>(in);
            break;
        case TI_DOUBLE:
            return deserialize<double>(in);
            break;
        case TI_LONG:
            return deserialize<long>(in);
            break;
        case TI_LONG_LONG:
            return deserialize<long long>(in);
            break;
        case TI_UNSIGNED_INT:
            return deserialize<unsigned int>(in);
            break;
        case TI_UNSIGNED_LONG:
            return deserialize<unsigned long>(in);
            break;
        case TI_UNSIGNED_LONG_LONG:
            return deserialize<unsigned long long>(in);
            break;
        case TI_STRING:
            return deserialize<string>(in);
            break;
    }
    
    throw bad_typeid();
    return "watesiggma";
}