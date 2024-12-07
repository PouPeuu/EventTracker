#ifndef SERIALIZABLE_H 
#define SERIALIZABLE_H

#include <fstream>

using namespace std;

class Serializable {
public:
    virtual void serialize(ofstream &out) const = 0;
    virtual ~Serializable() = default;
};

#endif