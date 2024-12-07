#include <database.hpp>
#include <iostream>

EventTracker::Database::Database(string name, time_t creation_timestamp) 
: name(name),
  creation_timestamp(creation_timestamp) {
    
}

void EventTracker::Database::serialize(ofstream &out) const {
    Serializer::serialize(out, this->name);
    Serializer::serialize(out, (long) this->creation_timestamp);

    Serializer::serialize(out, (int)this->events.size());
    for (const Event& event : this->events) {
        Serializer::serialize(out, &event);
    }
}

EventTracker::Database::Database(ifstream &in) {
    this->name = get<string>(Serializer::deserialize(in));
    this->creation_timestamp = get<long>(Serializer::deserialize(in));

    unsigned int events_size = get<int>(Serializer::deserialize(in));

    for (unsigned int i = 0; i < events_size; i++) {
        in.seekg(sizeof(char), ios_base::cur);
        this->events.push_back(Event(in));
    }
}

string EventTracker::Database::to_string() {
    string str = "Database " + this->name + "\n";
    string datetime = ctime(&this->creation_timestamp);
    str += "Created at " + datetime + "\n";
    str += "Events:\n";

    string events_str;

    for (const Event& event : this->events) {
        events_str += event.to_string() + "\n";
    }

    istringstream iss(events_str);

    for (string line; getline(iss, line); ) {
        str += "\t" + line + "\n";
    }

    return str;
}

void EventTracker::Database::add_event(Event event) {
    this->events.push_back(event);
}

EventTracker::Event& EventTracker::Database::get_event(unsigned int index) {
    return this->events[index];
}

unsigned int EventTracker::Database::get_events_size() {
    return this->events.size();
}