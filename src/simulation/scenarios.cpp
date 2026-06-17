#include "scenarios.h"
#include "constants.h"
#include <cmath>
#include <fstream>
#include <random>
#include <stdexcept>
#include <nlohmann/json.hpp>

void Scenarios::disk(Simulation& sim) {
    const int    N            = 10000;
    const double centralMass  = 2.0e30;
    const double particleMass = 1.0e24;
    const double rMin         = 0.4 * Constants::AU;
    const double rMax         = 2.0 * Constants::AU;

    Body star;
    star.position     = { 0.0, 0.0 };
    star.velocity     = { 0.0, 0.0 };
    star.acceleration = { 0.0, 0.0 };
    star.mass   = centralMass;
    star.radius = 6.957e9;
    star.color  = { 1.0f, 1.0f, 1.0f };
    sim.addBody(star);

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> randAngle(0.0, 2.0 * Constants::PI);
    std::uniform_real_distribution<double> randU(0.0, 1.0);
    std::uniform_real_distribution<double> randPerturb(-0.05, 0.05);

    for (int i = 0; i < N; i++) {
        double r     = std::sqrt(rMin * rMin + randU(rng) * (rMax * rMax - rMin * rMin));
        double angle = randAngle(rng);

        glm::dvec2 pos  = { r * std::cos(angle), r * std::sin(angle) };
        glm::dvec2 tang = { -std::sin(angle),     std::cos(angle) };

        double vcirc = std::sqrt(Constants::G * centralMass / r);
        double speed = vcirc * (1.0 + randPerturb(rng));

        Body body;
        body.position     = pos;
        body.velocity     = tang * speed;
        body.acceleration = { 0.0, 0.0 };
        body.mass   = particleMass;
        body.radius = 1.0e9;
        body.color  = { 1.0f, 1.0f, 1.0f };
        sim.addBody(body);
    }
}

void Scenarios::cluster(Simulation& sim) {
    const int    N            = 10000;
    const double totalMass    = 1.0e30;
    const double particleMass = totalMass / N;
    const double rMax         = 2.0 * Constants::AU;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> randAngle(0.0, 2.0 * Constants::PI);
    std::uniform_real_distribution<double> randU(0.0, 1.0);

    for (int i = 0; i < N; i++) {
        double r     = rMax * std::sqrt(randU(rng));
        double angle = randAngle(rng);

        glm::dvec2 pos = { r * std::cos(angle), r * std::sin(angle) };

        Body body;
        body.position     = pos;
        body.velocity     = { 0.0, 0.0 };
        body.acceleration = { 0.0, 0.0 };
        body.mass   = particleMass;
        body.radius = 1.0e9;
        body.color  = { 1.0f, 1.0f, 1.0f };
        sim.addBody(body);
    }
}

void Scenarios::galaxyCollision(Simulation& sim) {
    const int    N_per        = 5000;
    const double centralMass  = 1.5e30;
    const double particleMass = 1.0e24;
    const double rMin         = 0.3 * Constants::AU;
    const double rMax         = 1.5 * Constants::AU;
    const double offset       = 2.5 * Constants::AU;
    const double vApproach    = 6000.0;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> randAngle(0.0, 2.0 * Constants::PI);
    std::uniform_real_distribution<double> randU(0.0, 1.0);
    std::uniform_real_distribution<double> randPerturb(-0.05, 0.05);

    auto addGalaxy = [&](glm::dvec2 center, glm::dvec2 bulkVel, double spinSign, glm::vec3 color) {
        Body star;
        star.position     = center;
        star.velocity     = bulkVel;
        star.acceleration = { 0.0, 0.0 };
        star.mass   = centralMass;
        star.radius = 6.957e9;
        star.color  = { 1.0f, 1.0f, 1.0f };
        sim.addBody(star);

        for (int i = 0; i < N_per; i++) {
            double r     = std::sqrt(rMin * rMin + randU(rng) * (rMax * rMax - rMin * rMin));
            double angle = randAngle(rng);

            glm::dvec2 localPos = { r * std::cos(angle), r * std::sin(angle) };
            glm::dvec2 tang     = { -std::sin(angle) * spinSign, std::cos(angle) * spinSign };

            double vcirc = std::sqrt(Constants::G * centralMass / r);
            double speed = vcirc * (1.0 + randPerturb(rng));

            Body body;
            body.position     = center + localPos;
            body.velocity     = bulkVel + tang * speed;
            body.acceleration = { 0.0, 0.0 };
            body.mass   = particleMass;
            body.radius = 1.0e9;
            body.color  = color;
            sim.addBody(body);
        }
    };

    const double impact = 0.6 * Constants::AU;

    addGalaxy({ -offset,  impact }, {  vApproach, 0.0 },  1.0, { 0.3f, 0.5f, 1.0f });
    addGalaxy({  offset, -impact }, { -vApproach, 0.0 }, -1.0, { 1.0f, 0.2f, 0.2f });
}

void Scenarios::tidalDisruption(Simulation& sim) {
    const double bhMass        = 1.0e32;
    const int    N             = 3000;
    const double particleMass  = 1.0e24;
    const double clusterRadius = 0.1  * Constants::AU;
    const double rApocenter    = 5.0  * Constants::AU;
    const double rPericenter   = 0.8  * Constants::AU;

    Body bh;
    bh.position     = { 0.0, 0.0 };
    bh.velocity     = { 0.0, 0.0 };
    bh.acceleration = { 0.0, 0.0 };
    bh.mass   = bhMass;
    bh.radius = 1.0e10;
    bh.color  = { 1.0f, 0.8f, 0.2f };
    sim.addBody(bh);

    double a    = (rApocenter + rPericenter) / 2.0;
    double vApo = std::sqrt(Constants::G * bhMass * (2.0 / rApocenter - 1.0 / a));

    glm::dvec2 clusterCenter = { rApocenter, 0.0 };
    glm::dvec2 clusterVel    = { 0.0, vApo };

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> randAngle(0.0, 2.0 * Constants::PI);
    std::uniform_real_distribution<double> randU(0.0, 1.0);

    for (int i = 0; i < N; i++) {
        double r     = clusterRadius * std::sqrt(randU(rng));
        double angle = randAngle(rng);

        Body body;
        body.position     = clusterCenter + glm::dvec2{ r * std::cos(angle), r * std::sin(angle) };
        body.velocity     = clusterVel;
        body.acceleration = { 0.0, 0.0 };
        body.mass   = particleMass;
        body.radius = 1.0e9;
        body.color  = { 0.4f, 0.8f, 1.0f };
        sim.addBody(body);
    }
}

void Scenarios::loadFromFile(Simulation& sim, const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("cannot open scenario file: " + path);

    nlohmann::json j = nlohmann::json::parse(f);

    for (const auto& b : j.at("bodies")) {
        Body body;
        body.position     = { b.at("x").get<double>(), b.at("y").get<double>() };
        body.velocity     = { b.at("vx").get<double>(), b.at("vy").get<double>() };
        body.acceleration = { 0.0, 0.0 };
        body.mass         = b.at("mass").get<double>();
        body.radius       = b.at("radius").get<double>();
        auto c = b.at("color");
        body.color = { c[0].get<float>(), c[1].get<float>(), c[2].get<float>() };
        sim.addBody(body);
    }
}