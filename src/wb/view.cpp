#include <wb/geometry.h>
#include <wb/view.h>

#include <cmath>
#include <wb/utils.h>

namespace wb {

static const int DEFAULT_ZOOM = 5;
static const int MIN_ZOOM = 0;
static const int ZOOM_VALUES[] = {
    40, 48, 58, 69, 83, 100, 120, 144, 172, 207, 248, 300,
};

static const int MAX_ZOOM = sizeof(ZOOM_VALUES) / sizeof(*ZOOM_VALUES) - 1;

View::View(geometry::Vec2 position, geometry::Vec2 size) {
    m_position = position;
    m_size = size;
    m_zoom = DEFAULT_ZOOM;
}

void View::setPosition(geometry::Vec2 position) noexcept {
    m_position = position;
}

void View::setSize(geometry::Vec2 size) noexcept { m_size = size; }

void View::translate(geometry::Vec2 offset) noexcept {
    m_position = m_position - offset;
}

[[nodiscard]] geometry::Vec2
View::getViewPosition(geometry::Vec2 screenPosition) const noexcept {
    geometry::Vec2 position = screenPosition;
    position.x = position.x * 100 / getZoom();
    position.y = position.y * 100 / getZoom();
    position = m_position + position;
    return position;
}

[[nodiscard]] geometry::Vec2
View::getScreenPosition(geometry::Vec2 viewPosition) const noexcept {
    geometry::Vec2 position = viewPosition;
    position = position - m_position;
    position.x = position.x * getZoom() / 100;
    position.y = position.y * getZoom() / 100;
    return position;
}

[[nodiscard]] int View::getZoom() const noexcept { return ZOOM_VALUES[m_zoom]; }

void View::resetZoom() noexcept { m_zoom = DEFAULT_ZOOM; }

void View::zoom(geometry::Vec2 center, float z) noexcept {
    int previousZoom = getZoom();
    m_zoom = m_zoom + z;
    if (m_zoom > MAX_ZOOM) {
        m_zoom = MAX_ZOOM;
    }
    if (m_zoom < MIN_ZOOM) {
        m_zoom = MIN_ZOOM;
    }
    int newZoom = getZoom();

    if (newZoom == previousZoom) {
        return;
    }

    center = getViewPosition(center);
    geometry::Vec2 diff = m_position - center;
    diff.x = diff.x * previousZoom / newZoom;
    diff.y = diff.y * previousZoom / newZoom;
    m_position = center + diff;
}

geometry::Vec2 View::getPosition() const noexcept { return m_position; }

geometry::Vec2 View::getSize() const noexcept { return m_size; }

geometry::Box View::getBoundingBox() const noexcept {
    return geometry::Box{
        m_position,
        getViewPosition(m_size) - m_position,
    };
}

}; // namespace wb
