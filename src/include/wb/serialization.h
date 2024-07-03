#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <string>
#include <wb/drawing.h>

namespace serialization {

std::string serialize(std::vector<wb::Drawing> &);
std::vector<wb::Drawing> deserialize(std::string);

}; // namespace serialization

#endif
