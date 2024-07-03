#include <algorithm>
#include <wb/view.h>

#include <cmath>

namespace wb {

static const float DEFAULT_ZOOM = 1.0f;
static const float MIN_ZOOM = 0.1f;
static const float MAX_ZOOM = 5.0f;

View::View(geometry::Vec2 position, geometry::Vec2 size) {
    m_boundingBox.position = position;
    m_boundingBox.size = size;
    m_zoom = DEFAULT_ZOOM;
}

void View::setPosition(geometry::Vec2 position) noexcept {
    m_boundingBox.position = position;
}

void View::setSize(geometry::Vec2 size) noexcept {
    this->m_boundingBox.size = size;
}

void View::translate(geometry::Vec2 offset) noexcept {
    m_boundingBox.position = m_boundingBox.position - offset;
}

geometry::Vec2
View::getViewPosition(geometry::Vec2 screenPosition) const noexcept {
    geometry::Vec2 position = screenPosition;
    position.x = position.x * (1 / m_zoom);
    position.y = position.y * (1 / m_zoom);
    position = m_boundingBox.position + position;
    return position;
}

geometry::Vec2
View::getScreenPosition(geometry::Vec2 viewPosition) const noexcept {
    geometry::Vec2 position = viewPosition;
    position = position - m_boundingBox.position;
    position.x = position.x * m_zoom;
    position.y = position.y * m_zoom;
    return position;
}

float View::getZoom() const noexcept { return m_zoom; }

void View::resetZoom() noexcept { m_zoom = DEFAULT_ZOOM; }

void View::zoom(geometry::Vec2 center, float z) noexcept {
    float multiplier = std::pow(2.0f, z);
    float newZoom = std::clamp(m_zoom * multiplier, MIN_ZOOM, MAX_ZOOM);
    multiplier = newZoom / m_zoom;

    center = getViewPosition(center);
    geometry::Vec2 diff = m_boundingBox.position - center;
    diff.x = diff.x * (1 / multiplier);
    diff.y = diff.y * (1 / multiplier);
    m_boundingBox.position = center + diff;

    m_zoom = newZoom;
}

}; // namespace wb
