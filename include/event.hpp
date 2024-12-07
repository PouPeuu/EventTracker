#ifndef EVENT_H
#define EVENT_H

#include <ctime>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <variant>
#include <filesystem>
#include <serializable.hpp>
#include <serializer.hpp>
#include <complex_value.hpp>
#include <string>

using namespace std;
using ValueType = variant<double, string, EventTracker::ComplexValue>;
namespace fs = filesystem;

namespace EventTracker {
	class Event : public Serializable {
	private:
		string name;
		time_t timestamp;
		unordered_map<string, ValueType> values;
		unordered_set<string> flags;
	public:
		Event(string name, time_t timestamp, unordered_map<string, ValueType> values, unordered_set<string> flags);
		Event(ifstream &in);

		string get_name();
		time_t get_timestamp();
		
		ValueType get_value(string key);

		bool has_flag(string flag);

		string to_string() const;

		void serialize(ofstream &out) const;
	};
}

#endif
