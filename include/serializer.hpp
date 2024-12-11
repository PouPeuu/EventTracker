#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <variant>
#include <string>
#include <fstream>
#include <memory>
#include <serializable.hpp>
#include <iostream>

using namespace std;

namespace Serializer {
    using DeserializableType = variant<char, unsigned char, short, unsigned short, int, float, double, long, long long, unsigned int, unsigned long, unsigned long long, string>;

    enum TypeID {
        TI_NULL,
        TI_CHAR,
        TI_UNSIGNED_CHAR,
        TI_SHORT,
        TI_UNSIGNED_SHORT,
        TI_INT,
        TI_FLOAT,
        TI_DOUBLE,
        TI_LONG,
        TI_LONG_LONG,
        TI_UNSIGNED_INT,
        TI_UNSIGNED_LONG,
        TI_UNSIGNED_LONG_LONG,
        TI_STRING,
        TI_SERIALIZABLE
    };

    template <typename T>
    constexpr TypeID get_type_id() {
        if constexpr(is_same_v<T, char>) {
            return TI_CHAR;
        }
        else if constexpr(is_same_v<T, unsigned char>) {
            return TI_UNSIGNED_CHAR;
        }
        else if constexpr(is_same_v<T, short>) {
            return TI_SHORT;
        }
        else if constexpr(is_same_v<T, unsigned short>) {
            return TI_UNSIGNED_SHORT;
        }
        else if constexpr(is_same_v<T, int>) {
            return TI_INT;
        }
        else if constexpr(is_same_v<T, float>) {
            return TI_FLOAT;
        }
        else if constexpr(is_same_v<T, double>) {
            return TI_DOUBLE;
        }
        else if constexpr(is_same_v<T, long>) {
            return TI_LONG;
        }
        else if constexpr(is_same_v<T, long long>) {
            return TI_LONG_LONG;
        }
        else if constexpr(is_same_v<T, unsigned int>) {
            return TI_UNSIGNED_INT;
        }
        else if constexpr(is_same_v<T, unsigned long>) {
            return TI_UNSIGNED_LONG;
        }
        else if constexpr(is_same_v<T, unsigned long long>) {
            return TI_UNSIGNED_LONG_LONG;
        }
        else if constexpr(is_same_v<T, string>) {
            return TI_STRING;
        }
        else if constexpr(is_base_of_v<Serializable, T>) {
            return TI_SERIALIZABLE;
        } else { 
            static_assert(false, "wtf is this type");
            return TI_NULL;
        }
    }

    /*bool typeid_is_arithmetic(TypeID type) {
        switch(type) {
            case TI_CHAR:
            case TI_UNSIGNED_CHAR:
            case TI_SHORT:
            case TI_UNSIGNED_SHORT:
            case TI_INT:
            case TI_FLOAT:
            case TI_DOUBLE:
            case TI_LONG:
            case TI_LONG_LONG:
            case TI_UNSIGNED_INT:
            case TI_UNSIGNED_LONG:
            case TI_UNSIGNED_LONG_LONG:
                return true;
            default:
                return false;
        }
    }*/

    template <typename T>
    void serialize(ofstream &out, T value, bool serialize_type = false) {
        if (serialize_type) {
            char type = get_type_id<T>();
            out.write(reinterpret_cast<const char*>(&type), sizeof(type));
        }
        if constexpr(is_arithmetic_v<T>){
            out.write(reinterpret_cast<const char*>(&value), sizeof(value));
        } else if constexpr(is_same_v<T, string>) {
            size_t size = value.size();
            out.write(reinterpret_cast<const char*>(&size), sizeof(size));
            out.write(value.c_str(), size);
        } else if constexpr(is_base_of_v<Serializable, T>) {
            value.serialize(out);
        } else {
            static_assert(false, "Can't serialize that");
        }
    }

    template <typename T>
    T deserialize(ifstream &in) {
        if constexpr(is_arithmetic_v<T>){
            T result;
            in.read(reinterpret_cast<char*>(&result), sizeof(T));
            cout << "Deserialize: " << to_string(result) << endl;
            return result;
        } else if constexpr(is_same_v<T, string>) {
            size_t size;
            in.read(reinterpret_cast<char*>(&size), sizeof(size));
            char str[size+1]{};
            in.read(str, size);
            cout << "Deserialize: \"" << str << "\"\n";
            return string(str);
        } else {
            throw bad_typeid();
        }
    }

    template <typename T>
    void serialize_sequence(ofstream &out, T sequence) {
        serialize(out, sequence.size());
        for (const auto& value : sequence) {
            serialize(out, value);
        }
    }

    template <typename... Types>
    void serialize_variant(ofstream &out, const variant<Types...> &var) {
        std::visit([&out](const auto &value) {
            serialize(out, value, true);
        }, var);
    }

    DeserializableType deserialize_variant(ifstream &in);
}

#endif