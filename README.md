# N-Body Simulation

A real-time 2D gravitational N-body simulator written in C++20.

---

## Scenarios

### Accretion Disk
<video src="assets/disk.mp4" controls muted></video>

*Demonstration of a simulation run with 10,000 particles*

---

### Cold Cluster Collapse
<video src="assets/cluster.mp4" controls muted></video>

*Demonstration of a simulation run with 10,000 particles*

---

### Galaxy Collision
<video src="assets/galaxy-collision.mp4" controls muted></video>

*Demonstration of a simulation run with 10,000 particles*

---

### Tidal Disruption Event
<video src="assets/tidal.mp4" controls muted></video>

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