#include "SFMLrenderer.h"
#include "constants.h"
#include <cmath>

SFMLRenderer::SFMLRenderer(unsigned int width, unsigned int height, const std::string& title)
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = Constants::RENDERER_AA_LEVEL;

    window.create(sf::VideoMode({ width, height }), title, sf::State::Windowed, settings);
    window.setFramerateLimit(Constants::RENDERER_FRAMERATE_CAP);

    circleOffsets.reserve(Constants::RENDERER_BODY_POINTS);
    for (unsigned int i = 0; i < Constants::RENDERER_BODY_POINTS; i++) {
        double angle = (2.0 * Constants::PI * i) / Constants::RENDERER_BODY_POINTS;
        circleOffsets.push_back({ static_cast<float>(std::cos(angle)), static_cast<float>(std::sin(angle)) });
    }
}

bool SFMLRenderer::isOpen() const {
    return window.isOpen();
}

bool SFMLRenderer::pollEvents(Camera& camera) {
    static bool dragging = false;
    static sf::Vector2i lastMouse;

    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return false;
        }

        if (const auto* e = event->getIf<sf::Event::Resized>()) {
            camera.onResize(static_cast<double>(e->size.x), static_cast<double>(e->size.y));
        }

        if (const auto* e = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (e->button == sf::Mouse::Button::Left) {
                dragging = true;
                lastMouse = { e->position.x, e->position.y };
            }
        }

        if (const auto* e = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (e->button == sf::Mouse::Button::Left)
                dragging = false;
        }

        if (const auto* e = event->getIf<sf::Event::MouseMoved>()) {
            if (dragging) {
                sf::Vector2i cur{ e->position.x, e->position.y };
                camera.pan(static_cast<double>(cur.x - lastMouse.x), static_cast<double>(cur.y - lastMouse.y));
                lastMouse = cur;
            }
        }

        if (const auto* e = event->getIf<sf::Event::MouseWheelScrolled>()) {
            double factor = (e->delta > 0) ? Constants::RENDERER_ZOOM_STEP : 1.0 / Constants::RENDERER_ZOOM_STEP;
            camera.zoom(factor, static_cast<double>(e->position.x), static_cast<double>(e->position.y));
        }

        if (const auto* e = event->getIf<sf::Event::KeyPressed>()) {
            if (e->code == sf::Keyboard::Key::Escape) {
                window.close();
            }
        }
    }
    return true;
}

void SFMLRenderer::render(const std::vector<Body>& bodies, Camera& camera) {
    window.clear(sf::Color(Constants::BG_COLOR_R, Constants::BG_COLOR_G, Constants::BG_COLOR_B));
    window.setView(window.getDefaultView());

    vertices.clear();
    vertices.reserve(bodies.size() * Constants::RENDERER_BODY_POINTS * 3);

    for (const auto& body : bodies)
        appendBody(body, camera);

    if (!vertices.empty())
        window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles);

    window.display();
}

void SFMLRenderer::appendBody(const Body& body, const Camera& camera) {
    sf::Vector2f screenPos = camera.worldToScreen(body.position);
    float screenRadius = static_cast<float>(body.radius * camera.getZoom());

    sf::Color color(
        static_cast<uint8_t>(body.color.r * 255),
        static_cast<uint8_t>(body.color.g * 255),
        static_cast<uint8_t>(body.color.b * 255)
    );

    size_t n = circleOffsets.size();
    for (size_t i = 0; i < n; i++) {
        sf::Vector2f p0 = screenPos + circleOffsets[i] * screenRadius;
        sf::Vector2f p1 = screenPos + circleOffsets[(i + 1) % n] * screenRadius;
        vertices.push_back({ screenPos, color });
        vertices.push_back({ p0, color });
        vertices.push_back({ p1, color });
    }
}
