# N-Body Simulation

A real-time 2D gravitational N-body simulator written in C++20.

---

## Scenarios

### Accretion Disk

https://github.com/user-attachments/assets/37972584-05e6-4069-b8ba-154faae19e12

*Demonstration of a simulation run with 10,000 particles*

---

### Cold Cluster Collapse

https://github.com/user-attachments/assets/71b44d24-63bf-4bed-b397-c19b67603f8d

*Demonstration of a simulation run with 10,000 particles*

---

### Galaxy Collision

https://github.com/user-attachments/assets/93036c4a-a434-41d9-85c1-6bdb3bb08188

*Demonstration of a simulation run with 10,000 particles*

---

### Tidal Disruption Event

https://github.com/user-attachments/assets/864a7dce-a3b0-4f25-8739-b993a5ec9d38

*Demonstration of a simulation run with 3,000 particles*

---

## Building

Requires CMake 3.8+, Ninja, and a C++20 compiler.

```bash
cmake --preset release
cmake --build out/build/release
```

The binary is `out/build/release/N-Body-Simulation.exe`.

---

## Usage

```bash
N-Body-Simulation.exe [scenario]
```

| Argument | Description |
|---|---|
| `disk` | Accretion disk (default) |
| `cluster` | Cold cluster collapse |
| `galaxy-collision` | Two-galaxy merger |
| `tidal` | Tidal disruption event |
| `path/to/file.json` | Load a custom scenario from JSON |

### Controls

| Input | Action |
|---|---|
| Left-click drag | Pan |
| Scroll wheel | Zoom in / out |
| `Escape` | Quit |

---

## Custom Scenarios (JSON)

Bodies can be loaded from a JSON file:

```json
{
  "bodies": [
    {
      "x": 0.0, "y": 0.0,
      "vx": 0.0, "vy": 0.0,
      "mass": 2e30,
      "radius": 6.957e9,
      "color": [1.0, 1.0, 1.0]
    }
  ]
}
```

Positions and velocities are in SI units (metres, metres per second). Color components are in the range `[0, 1]`.

---

## Configuration

Simulation and renderer settings (timestep, time scale, Barnes-Hut theta, softening, framerate cap, zoom, etc.) can be tuned in `include/constants.h`.
