#include <serializer.hpp>
#include <iostream>

enum TypeIdentifier {
    TI_DOUBLE,
    TI_INT,
    TI_LONG,
    TI_STRING,
    TI_SERIALIZABLE
};

void Serializer::serialize(ofstream &out, SerializableType value) {
    visit([&out](auto&& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr(is_same_v<T, string>) {
            char type = TI_STRING;
            out.write(reinterpret_cast<const char*>(&type), sizeof(type));

            unsigned int v_size = v.size();
            out.write(reinterpret_cast<const char*>(&v_size), sizeof(v_size));
            out.write(v.c_str(), v_size);
        } else if constexpr(is_same_v<T, double>) {
            char type = TI_DOUBLE;
            out.write(reinterpret_cast<const char*>(&type), sizeof(type));
            out.write(reinterpret_cast<const char*>(&v), sizeof(v));
        } else if constexpr(is_same_v<T, const Serializable*>) {
            char type = TI_SERIALIZABLE;
            out.write(reinterpret_cast<const char*>(&type), sizeof(type));

            v->serialize(out);
        } else if constexpr(is_same_v<T, int>) {
            char type = TI_INT;
            out.write(reinterpret_cast<const char*>(&type), sizeof(type));
            out.write(reinterpret_cast<const char*>(&v), sizeof(v));
        } else if constexpr(is_same_v<T, long>) {
            char type = TI_LONG;
            out.write(reinterpret_cast<const char*>(&type), sizeof(type));
            out.write(reinterpret_cast<const char*>(&v), sizeof(v));
        }
    }, value);
}

DeserializableType Serializer::deserialize(ifstream &in) {
    char type; 
    in.read(reinterpret_cast<char*>(&type), sizeof(type));

    DeserializableType dt_value;

    switch (type) {
        case TI_DOUBLE:
            {
                double value;
                in.read(reinterpret_cast<char*>(&value), sizeof(double));
                dt_value = value;
                break;
            }
        case TI_INT:
            {
                int value;
                in.read(reinterpret_cast<char*>(&value), sizeof(int));
                dt_value = value;
                break;
            }
        case TI_LONG:
            {
                long value;
                in.read(reinterpret_cast<char*>(&value), sizeof(long));
                dt_value = value;
                break;
            }
        case TI_STRING:
            {
                unsigned int size;
                in.read(reinterpret_cast<char*>(&size), sizeof(size));
                char str[size + 1];
                in.read(str, size);
                str[size] = '\0';
                string value = str;
                dt_value = value;
                break;
            }
        default:
            throw invalid_argument("can't deserialize that");
    }

    return dt_value;
}