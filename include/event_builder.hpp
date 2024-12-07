#ifndef EVENT_BUILDER_H
#define EVENT_BUILDER_H

#include <event.hpp>

namespace EventTracker {
    class EventBuilder {
    private:
        string name;
        time_t timestamp;
		unordered_map<string, ValueType> values;
		unordered_set<string> flags;
    public:
        EventBuilder();

        EventBuilder& set_name(string name);
        string get_name();

        EventBuilder& set_timestamp(time_t timestamp);
        time_t get_timestamp();

        EventBuilder& set_value(string key, ValueType value);
        ValueType get_value(string key);

        EventBuilder& add_flag(string flag);
        EventBuilder& remove_flag(string flag);
        bool has_flag(string flag);

        Event build();
    };
}

#endif