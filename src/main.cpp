#include "constants.h"
#include "SFMLrenderer.h"
#include "camera.h"
#include "simulation.h"
#include "scenarios.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    SFMLRenderer renderer(Constants::WINDOW_W, Constants::WINDOW_H, "N-Body Simulation");
    Camera camera(static_cast<double>(Constants::WINDOW_W), static_cast<double>(Constants::WINDOW_H));
    Simulation sim;

    camera.setZoom(Constants::CAMERA_INITIAL_ZOOM);
    camera.setCenter({ Constants::CAMERA_CENTER_X, Constants::CAMERA_CENTER_Y });

    try {
        std::string scenario = (argc >= 2) ? argv[1] : "disk";

        if      (scenario == "disk")             Scenarios::disk(sim);
        else if (scenario == "cluster")          Scenarios::cluster(sim);
        else if (scenario == "galaxy-collision") Scenarios::galaxyCollision(sim);
        else if (scenario == "tidal")            Scenarios::tidalDisruption(sim);
        else Scenarios::loadFromFile(sim, scenario);
    } catch (const std::exception& e) {
        std::cerr << "Error loading scenario: " << e.what() << "\n";
        return 1;
    }

    sim.computeForces();

    sf::Clock clock;
    while (renderer.isOpen()) {
        float dt = clock.restart().asSeconds();
        float fps = 1/dt;
        renderer.getWindow().setTitle("N-Body Simulation - " + std::format("{:.2f}", fps) + " FPS");
        if (dt > Constants::SIM_MAX_DT) dt = Constants::SIM_MAX_DT;

        if (!renderer.pollEvents(camera)) break;

        sim.update(static_cast<double>(dt) * Constants::TIME_SCALE);
        renderer.render(sim.getBodies(), camera);
    }

    return 0;
}
