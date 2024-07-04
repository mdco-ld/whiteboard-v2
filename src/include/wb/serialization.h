#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <istream>
#include <ostream>
#include <wb/drawing.h>

namespace serialization {

void serialize(std::ostream &, std::vector<wb::Drawing> &);
std::vector<wb::Drawing> deserialize(std::istream &);

}; // namespace serialization

#endif
