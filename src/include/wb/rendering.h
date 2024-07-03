#ifndef _RENDERING_H_
#define _RENDERING_H_

#include <string_view>
#include <wb/drawing.h>
#include <wb/geometry.h>
#include <wb/view.h>

namespace rendering {

void render(wb::View &, geometry::Vec2);
void render(wb::View &, geometry::LineSegment);
void render(wb::View &, wb::PartialDrawing &);
void render(wb::View &, wb::Drawing &);
void renderStatusBar(wb::View&, std::string_view message);

}; // namespace rendering

#endif
