# City-Building Simulator

A modular, object-oriented city-building simulator written in modern C++.

Simulate post-war reconstruction across various settlements using customizable plans, selection policies, and facilities — all managed with precise memory handling following the Rule of 5.

---

## Features

- Modular architecture — clean separation between core entities (Plans, Facilities, Settlements)
- Selection Policies — choose how your city evolves: `Naive`, `Balanced`, `Economy`, `Sustainability`
- Manual memory management — uses deep copies, destructors, and cloneable actions (Rule of 5)
- Command-based simulation — interactively build, simulate, and analyze urban development

---

## Build & Run

### Build

```bash
make
```

This compiles all source files into `./bin/main`.

### Run

```bash
./bin/main config_file.txt
```

The simulation will start and wait for interactive user commands.

---

## Project Structure

```
.
├── makefile                # Compilation script
├── bin/                   # Compiled binary goes here
├── include/               # Header files (.h)
├── src/                   # Source files (.cpp)
├── .vscode/               # VSCode settings (optional)
└── README.md
```

---

## Example Session

### Example Config (`config_file.txt`)
```txt
settlement KfarSPL 0
settlement KiryatSPL 2

facility hospital 0 4 5 2 1
facility market 1 3 1 5 1
facility solarFarm 2 3 2 1 5

plan KfarSPL eco
plan KiryatSPL bal
```

### User Interaction

```txt
The simulation has started
step 1
planStatus 0
planStatus 1
settlement anotherVillage 0
backup
plan anotherVillage bal
step 2
planStatus 2
close
```

### Sample Output

```txt
PlanID: 2
SettlementName: anotherVillage
LifeQualityScore: 1
EconomyScore: 1
EnvironmentScore: 1
```

---

## Core Concepts

| Concept        | Description |
|----------------|-------------|
| **Settlement** | Physical area (village/city/metropolis) with a construction limit |
| **Plan**       | A strategy for rebuilding a specific settlement using a defined policy |
| **FacilityType** | A reusable template describing a facility (hospital, road, etc.) |
| **Facility**   | An instance of a facility built as part of a plan |
| **SelectionPolicy** | Strategy determining which facility a plan should build next |
| **Rule of 5**  | All dynamic objects implement destructor, copy/move constructors, and assignment operators |
| **BaseAction** | Abstract base class for user commands, supporting polymorphism and cloning |

---

## Supported Commands

| Command                        | Description |
|-------------------------------|-------------|
| `step <N>`                    | Simulate N time units |
| `plan <settlement> <policy>` | Add a new plan |
| `facility <name> ...`        | Add a new facility type |
| `settlement <name> <type>`  | Add a new settlement |
| `planStatus <id>`           | Show status of a plan |
| `changePolicy <id> <type>` | Change plan selection strategy |
| `backup` / `restore`        | Save/restore current simulation state |
| `log`                       | View all executed actions |
| `close`                     | End simulation and print final results |

---

## Technologies

- **C++11**
- `Makefile`-based build system
- Valgrind-compatible
- Polymorphism & smart memory usage

---

## Notes

- Written with modern C++ best practices
- Designed to run on Linux-based university lab machines
- Avoids memory leaks using deep copies and destructors
- Suitable for academic evaluation and simulation-based logic testing

