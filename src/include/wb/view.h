#ifndef _VIEW_H_
#define _VIEW_H_

#include <wb/geometry.h>

namespace wb {

class View {
  public:
    View(geometry::Vec2 position, geometry::Vec2 size);
    void setPosition(geometry::Vec2) noexcept;
    void setSize(geometry::Vec2) noexcept;
    void translate(geometry::Vec2) noexcept;
    void zoom(geometry::Vec2 center, float z) noexcept;
    void resetZoom() noexcept;

    geometry::Vec2
    getViewPosition(geometry::Vec2 screenPosition) const noexcept;
    geometry::Vec2
    getScreenPosition(geometry::Vec2 viewPosition) const noexcept;
    float getZoom() const noexcept;

  private:
    geometry::Box m_boundingBox;
    float m_zoom;
};

}; // namespace wb

#endif