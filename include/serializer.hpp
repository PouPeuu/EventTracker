#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <variant>
#include <string>
#include <fstream>
#include <memory>
#include <serializable.hpp>

using namespace std;

using SerializableType = variant<string, double, int, long, const Serializable*>;
using DeserializableType = variant<string, double, int, long>;

namespace Serializer {
    void serialize(ofstream &out, SerializableType value);
    DeserializableType deserialize(ifstream &in);

    template <typename T>
    void serialize_sequence(ofstream &out, T sequence) {
        serialize(out, (int) sequence.size());
        for (const auto& value : sequence) {
            serialize(out, value);
        }
    }
}

#endif