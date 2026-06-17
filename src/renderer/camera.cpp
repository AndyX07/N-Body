#include "camera.h"
#include "constants.h"

Camera::Camera(double screenWidth, double screenHeight)
    : center(0.0, 0.0), zoomLevel(1.0), screenW(screenWidth), screenH(screenHeight) {
}

void Camera::pan(double dx, double dy) {
    center.x -= dx / zoomLevel;
    center.y -= dy / zoomLevel;
}

void Camera::zoom(double factor, double anchorScreenX, double anchorScreenY) {
    glm::dvec2 worldAnchor = screenToWorld(anchorScreenX, anchorScreenY);

    zoomLevel *= factor;
    if (zoomLevel < Constants::CAMERA_ZOOM_MIN) {
        zoomLevel = Constants::CAMERA_ZOOM_MIN;
    }
        
    glm::dvec2 worldAnchorAfter = screenToWorld(anchorScreenX, anchorScreenY);
    center += worldAnchor - worldAnchorAfter;
}

glm::dvec2 Camera::screenToWorld(double sx, double sy) const {
    double wx = center.x + (sx - screenW * 0.5) / zoomLevel;
    double wy = center.y + (sy - screenH * 0.5) / zoomLevel;
    return { wx, wy };
}

sf::Vector2f Camera::worldToScreen(const glm::dvec2& worldPos) const {
    double sx = (worldPos.x - center.x) * zoomLevel + screenW * 0.5;
    double sy = (worldPos.y - center.y) * zoomLevel + screenH * 0.5;
    return { static_cast<float>(sx), static_cast<float>(sy) };
}

void Camera::onResize(double newWidth, double newHeight) {
    screenW = newWidth;
    screenH = newHeight;
}