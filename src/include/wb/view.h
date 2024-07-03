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

    [[nodiscard]] geometry::Vec2
    getViewPosition(geometry::Vec2 screenPosition) const noexcept;
    [[nodiscard]] geometry::Vec2
    getScreenPosition(geometry::Vec2 viewPosition) const noexcept;
    [[nodiscard]] int getZoom() const noexcept;

    [[nodiscard]] geometry::Vec2 getPosition() const noexcept;
    [[nodiscard]] geometry::Vec2 getSize() const noexcept;

  private:
    geometry::Box m_boundingBox;
    int m_zoom;
};

}; // namespace wb

#endif
