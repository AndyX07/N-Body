#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(double screenWidth, double screenHeight);
    void pan(double dx, double dy);
    void zoom(double factor, double anchorScreenX, double anchorScreenY);
    glm::dvec2 screenToWorld(double sx, double sy) const;
    sf::Vector2f worldToScreen(const glm::dvec2& worldPos) const;
    glm::dvec2 getCenter() const { return center; }
    double getZoom() const { return zoomLevel; }
    void setCenter(const glm::dvec2& c) { center = c; }
    void setZoom(double z) { zoomLevel = z; }
    void onResize(double newWidth, double newHeight);

private:
    glm::dvec2  center;
    double       zoomLevel;
    double       screenW;
    double       screenH;
};

#endif