#ifndef COMPLEX_VALUE_H
#define COMPLEX_VALUE_H

#include <unordered_map>
#include <string>
#include <unordered_set>
#include <variant>
#include <serializable.hpp>
#include <serializer.hpp>

using namespace std;

namespace EventTracker {
    class ComplexValue : public Serializable {
        using ValueType = variant<double, string, EventTracker::ComplexValue>;
        private:
            unordered_map<string, ValueType> values;
            unordered_set<string> flags;
        public:
            ComplexValue() = default;
            ComplexValue(ifstream &in);

            ValueType get_value(string key);
            void set_value(string key, ValueType value);

		    void add_flag(string flag);
		    void remove_flag(string flag);
		    bool has_flag(string flag);

            string to_string() const;

            void serialize(ofstream &out) const;
    };
}

#endif