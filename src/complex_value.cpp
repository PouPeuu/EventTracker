#include "complex_value.hpp"
#include <sstream>
#include <fstream>

using namespace EventTracker;

using ValueType = variant<double, string, EventTracker::ComplexValue>;

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
			if constexpr (is_same_v<T, double>) {
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

	Serializer::serialize(out, (int) this->values.size());
	for (const auto& [key, value] : this->values) {
		Serializer::serialize(out, key);

		if (holds_alternative<EventTracker::ComplexValue>(value)) {
			Serializer::serialize(out, &get<EventTracker::ComplexValue>(value));
		} else if (holds_alternative<double>(value)) {
			Serializer::serialize(out, get<double>(value));
		} else if (holds_alternative<string>(value)) {
			Serializer::serialize(out, get<string>(value));
		}
	}
}

EventTracker::ComplexValue::ComplexValue(ifstream &in) {
	unsigned int flags_size = get<int>(Serializer::deserialize(in));

	for (unsigned int i = 0; i < flags_size; i++) {
		this->flags.insert(get<string>(Serializer::deserialize(in)));
	}

	unsigned int values_size = get<int>(Serializer::deserialize(in));

	for (unsigned int i = 0; i < values_size; i++) {
		string key = get<string>(Serializer::deserialize(in));

		try {
			auto value = Serializer::deserialize(in);
			if (holds_alternative<double>(value)) {
				this->values[key] = get<double>(value);
			} else if (holds_alternative<string>(value)) {
				this->values[key] = get<string>(value);
			}
		} catch (const invalid_argument& e) {
			this->values[key] = ComplexValue(in);
		}
	}
}