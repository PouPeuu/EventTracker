#include <event_builder.hpp>

using namespace EventTracker;

EventBuilder::EventBuilder() {}

EventBuilder& EventBuilder::set_name(string name) {
    this->name = name;
    return *this;
}

string EventBuilder::get_name() {
    return this->name;
}

EventBuilder& EventBuilder::set_timestamp(time_t timestamp) {
    this->timestamp = timestamp;
    return *this;
}

time_t EventBuilder::get_timestamp() {
    return this->timestamp;
}

EventBuilder& EventBuilder::set_value(string key, ValueType value) {
	this->values[key] = value;
    return *this;
}

ValueType EventBuilder::get_value(string key) {
    return this->values[key];
}

EventBuilder& EventBuilder::add_flag(string flag) {
	this->flags.insert(flag);
    return *this;
}

EventBuilder& EventBuilder::remove_flag(string flag) {
	this->flags.erase(flag);
    return *this;
}

bool EventBuilder::has_flag(string flag) {
    return this->flags.count(flag);
}

Event EventBuilder::build() {
    Event event(this->name, this->timestamp, this->values, this->flags);
    return event;
}