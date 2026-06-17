#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "body.h"
#include "camera.h"

class SFMLRenderer {
public:
    SFMLRenderer(unsigned int width, unsigned int height, const std::string& title);
    bool isOpen() const;
    bool pollEvents(Camera& camera);
    void render(const std::vector<Body>& bodies, Camera& camera);
    sf::RenderWindow& getWindow() { return window; }

private:
    sf::RenderWindow window;
    std::vector<sf::Vector2f> circleOffsets;
    std::vector<sf::Vertex> vertices;
    void appendBody(const Body& body, const Camera& camera);
};

#endif