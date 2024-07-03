#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <vector>
#include <wb/geometry.h>

namespace wb {

class PartialDrawing {
  public:
    PartialDrawing();
    void addPoint(geometry::Vec2) noexcept;

    [[nodiscard]] const std::vector<geometry::Vec2> &getPoints() noexcept;

  private:
    std::vector<geometry::Vec2> m_points;
};

}; // namespace wb

#endif
