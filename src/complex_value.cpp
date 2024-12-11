#include "complex_value.hpp"
#include <sstream>
#include <fstream>

using namespace EventTracker;

using ValueType = variant<char, unsigned char, short, unsigned short, int, float, double, long, long long, unsigned int, unsigned long, unsigned long long, string, EventTracker::ComplexValue>;

ValueType EventTracker::ComplexValue::get_value(string key) {
    return this->values[key];
}

void EventTracker::ComplexValue::set_value(string key, ValueType value) {
    this->values[key] = value;
}

void EventTracker::ComplexValue::add_flag(string flag) {
    this->flags.insert(flag);
}

void EventTracker::ComplexValue::remove_flag(string flag) {
    this->flags.erase(flag);
}

bool EventTracker::ComplexValue::has_flag(string flag) {
    return this->flags.count(flag);
}

string EventTracker::ComplexValue::to_string() const {
    string final_str = "ComplexValue [\n";

    string str = "Values:\n";

	string values_str;

	for (const auto& [key, value] : this->values) {
		values_str += "[" + key + "] = ";
		visit([&values_str](auto&& v) {
			using T = decay_t<decltype(v)>;
			if constexpr (is_arithmetic_v<T>) {
				values_str += std::to_string(v);
			} else if constexpr (is_same_v<T, string>) {
				values_str += v;
			} else {
				values_str += v.to_string();
			}
		}, value);
		values_str += "\n";
	}

	istringstream iss(values_str);

	for(string line; getline(iss, line); ) {
		str += "\t" + line + "\n";
	}

	str += "\nFlags:\n";

	string flags_str;

	for (const string& flag : this->flags) {
		flags_str += flag + "\n";
	}

	iss = istringstream(flags_str);

	for(string line; getline(iss, line); ) {
		str += "\t" + line + "\n";
	}

    iss = istringstream(str);

	for(string line; getline(iss, line); ) {
		final_str += "\t" + line + "\n";
	}

    final_str += "]\n";
    return final_str;
}

void EventTracker::ComplexValue::serialize(ofstream &out) const {
	Serializer::serialize_sequence(out, this->flags);

	Serializer::serialize(out, this->values.size());
	for (const auto& [key, value] : this->values) {
		Serializer::serialize(out, key);

		Serializer::serialize_variant(out, value);
	}
}

EventTracker::ComplexValue::ComplexValue(ifstream &in) {
	size_t n_flags = Serializer::deserialize<size_t>(in);
	for (size_t i = 0; i < n_flags; i++) {
		this->flags.insert(Serializer::deserialize<string>(in));
	}

	size_t n_values = Serializer::deserialize<size_t>(in);
	for (size_t i = 0; i < n_values; i++) {
		string key = Serializer::deserialize<string>(in);
		try {
			visit([this, key](auto&& v) {
				this->values[key] = v;
			}, Serializer::deserialize_variant(in));
		} catch (bad_typeid e) {
			ComplexValue complex_value(in);
			this->values[key] = complex_value;
		}
	}
}