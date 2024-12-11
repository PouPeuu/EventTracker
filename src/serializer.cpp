#include <serializer.hpp>

Serializer::DeserializableType Serializer::deserialize_variant(ifstream &in) {
    TypeID type = TypeID(deserialize<char>(in));
    switch (type) {
        case TI_CHAR:
            cout << "Variant type recognized: TI_CHAR\n";
            return deserialize<char>(in);
            break;
        case TI_UNSIGNED_CHAR:
            cout << "Variant type recognized: TI_UNSIGNED_CHAR\n";
            return deserialize<unsigned char>(in);
            break;
        case TI_SHORT:
            cout << "Variant type recognized: TI_SHORT\n";
            return deserialize<short>(in);
            break;
        case TI_UNSIGNED_SHORT:
            cout << "Variant type recognized: TI_UNSIGNED_SHORT\n";
            return deserialize<unsigned short>(in);
            break;
        case TI_INT:
            cout << "Variant type recognized: TI_INT\n";
            return deserialize<int>(in);
            break;
        case TI_FLOAT:
            cout << "Variant type recognized: TI_FLOAT\n";
            return deserialize<float>(in);
            break;
        case TI_DOUBLE:
            cout << "Variant type recognized: TI_DOUBLE\n";
            return deserialize<double>(in);
            break;
        case TI_LONG:
            cout << "Variant type recognized: TI_LONG\n";
            return deserialize<long>(in);
            break;
        case TI_LONG_LONG:
            cout << "Variant type recognized: TI_LONG_LONG\n";
            return deserialize<long long>(in);
            break;
        case TI_UNSIGNED_INT:
            cout << "Variant type recognized: TI_UNSIGNED_INT\n";
            return deserialize<unsigned int>(in);
            break;
        case TI_UNSIGNED_LONG:
            cout << "Variant type recognized: TI_UNSIGNED_LONG\n";
            return deserialize<unsigned long>(in);
            break;
        case TI_UNSIGNED_LONG_LONG:
            cout << "Variant type recognized: TI_UNSIGNED_LONG_LONG\n";
            return deserialize<unsigned long long>(in);
            break;
        case TI_STRING:
            cout << "Variant type recognized: TI_STRING\n";
            return deserialize<string>(in);
            break;
    }
    
    cout << "Variant type other\n";
    throw bad_typeid();
    return "watesiggma";
}