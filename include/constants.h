#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace Constants {

    // Physical
    inline constexpr double G  = 6.67430e-11;
    inline constexpr double AU = 1.496e11;
    inline constexpr double PI = 3.14159265358979323846;

    // Window
    inline constexpr unsigned int WINDOW_W = 1280;
    inline constexpr unsigned int WINDOW_H = 720;

    // Simulation timing
    // ratio of real time to simulation time
    inline constexpr double TIME_SCALE = 86400.0 * 10.0;
    // the maximum time step between frames
    inline constexpr float  SIM_MAX_DT = 0.05f;

    // Renderer
    inline constexpr unsigned int RENDERER_AA_LEVEL           = 8;
    inline constexpr unsigned int RENDERER_FRAMERATE_CAP      = 60;
    inline constexpr unsigned int RENDERER_BODY_POINTS        = 32;
    inline constexpr double       RENDERER_ZOOM_STEP          = 1.15;
    inline constexpr uint8_t      BG_COLOR_R                  = 10;
    inline constexpr uint8_t      BG_COLOR_G                  = 10;
    inline constexpr uint8_t      BG_COLOR_B                  = 20;

    // Camera
    inline constexpr double CAMERA_ZOOM_MIN     = 1e-38;
    inline constexpr double CAMERA_INITIAL_ZOOM = 1.0 / 1.0e9;  // 1 px = 1e9 m
    inline constexpr double CAMERA_CENTER_X = 0.0;
    inline constexpr double CAMERA_CENTER_Y = 0.0;

    // Barnes-Hut tree
    inline constexpr double BH_THETA       = 0.5;
    inline constexpr double BH_SOFTENING_SQ = 1e18;
    inline constexpr double BH_POS_JITTER  = 1e-9;
}

#endif
