#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <istream>
#include <ostream>
#include <wb/drawing.h>

namespace serialization {

template <typename T> void serialize(std::ostream &, T &);
template <typename T> void deserialize(std::istream &, T &);
template <typename T>
void serialize(std::ostream &out, std::vector<T> &values) {
    int length = values.size();
    serialize(out, length);
    for (T &value : values) {
        serialize(out, value);
    }
}
template <typename T>
void deserialize(std::istream &in, std::vector<T> &output) {
    int length;
    deserialize(in, length);
    if (!output.empty()) {
        output.clear();
    }
    output.resize(length);
    for (int i = 0; i < length; i++) {
        deserialize(in, output[i]);
    }
}

void serialize(std::ostream &, std::vector<wb::Drawing> &);
std::vector<wb::Drawing> deserialize(std::istream &);

}; // namespace serialization

#endif
