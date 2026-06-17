#ifndef SCENARIOS_H
#define SCENARIOS_H

#include "simulation.h"
#include <string>

namespace Scenarios {
    void disk(Simulation& sim);
    void cluster(Simulation& sim);
    void galaxyCollision(Simulation& sim);
    void tidalDisruption(Simulation& sim);
    void loadFromFile(Simulation& sim, const std::string& path);
}

#endif
