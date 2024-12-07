#ifndef DATABASE_H
#define DATABASE_H

#include <event.hpp>
#include <vector>

// TODO: Predefined ComplexValues in database, events can reference those in place of new definitions for every event

namespace EventTracker {
    class Database : public Serializable {
    private:
        string name;
        time_t creation_timestamp;

        // TODO: Maybe change this to a map of timestamp: Event or timestamp : Name
        vector<Event> events;

        vector<ValueType> predefined_values;
    public:
        Database(string name, time_t creation_timestamp);
        Database(ifstream &in);

        void serialize(ofstream &out) const;

        string to_string();

        void add_event(Event event);
        Event& get_event(unsigned int index);
        unsigned int get_events_size();
    };
}

#endif