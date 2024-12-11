#include "event.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace EventTracker;

Event::Event(string name, time_t timestamp) {
	this->name = name;
	this->timestamp = timestamp;
}

Event::Event(string name, time_t timestamp, unordered_map<string, ValueType> values, unordered_set<string> flags) {
	this->name = name;
	this->timestamp = timestamp;
	this->values = values;
	this->flags = flags;
}

string Event::get_name() {
	return this->name;
}

time_t Event::get_timestamp() {
	return this->timestamp;
}

string Event::to_string() const {
	string str = "Event " + this->name + " at ";
	string datetime = ctime(&this->timestamp);
	str += datetime;

	str += "\nValues:\n";

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

	return str;
}

void EventTracker::Event::serialize(ofstream &out) const {
	Serializer::serialize(out, this->name);
	Serializer::serialize(out, this->timestamp);

	Serializer::serialize_sequence(out, this->flags);

	Serializer::serialize(out, this->values.size());
	for (const auto& [key, value] : this->values) {
		Serializer::serialize(out, key);

		Serializer::serialize_variant(out, value);
	}
}

EventTracker::Event::Event(ifstream &in) {
	this->name = Serializer::deserialize<string>(in);
	this->timestamp = Serializer::deserialize<time_t>(in);

	size_t n_flags = Serializer::deserialize<size_t>(in);
	for (size_t i = 0; i < n_flags; i++) {
		this->flags.insert(Serializer::deserialize<string>(in));
	}

	size_t n_values = Serializer::deserialize<size_t>(in);
	for (size_t i = 0; i < n_values; i++) {
		string key = Serializer::deserialize<string>(in);

		// FIXME
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