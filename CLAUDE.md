# CLAUDE.md — GHEControl

## What this is
GHEControl is a standalone Qt6/C++20 desktop app for controlling antenna switches
via the Green Heron Engineering (GHE) Everywhere TCP protocol. It is a companion
to AetherSDR but has no dependency on it.

## Build targets
- **Linux**: AppImage (primary)
- **Windows**: .exe installer via Inno Setup (primary)
- **macOS**: best-effort, not CI-tested yet

## Build instructions
```bash
# Linux
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -j$(nproc)

# Windows (from Developer PowerShell or after ilammy/msvc-dev-cmd)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

## Tech stack
- C++20, Qt 6.7+
- Qt modules: Core, Widgets, Network (nothing else)
- Build system: CMake 3.21+, Ninja
- No Qt Designer .ui files — all UI is code
- No third-party libraries beyond Qt

## GHE Protocol (fully reverse-engineered)
All messages are newline-terminated, dot-delimited ASCII.

### Server → Client
```
SWITCHADD.<radioCount>.<radioName>.<ant1>.<fLowInt>.<fLowFrac>.<fHighInt>.<fHighFrac>.[...]
SWITCHUPDATE.<radio>.<activeAntenna>.<pttState>.<signalDbm>
SWITCHLOCKS.<radio>.<lockedFromThisRadio>.<thisRadioActive>
```
Lone `.` messages are heartbeat noise — discard.

### Client → Server
```
SET_SWITCH.<radio>.<antenna>
SET_SWITCH.<radio>.OFF
```

### SWITCHADD details
- Sent once per radio on connect (one message per radio, not one for all)
- radioCount = total number of radios configured on server
- Each antenna entry: name + 4 fields = bandLowInt, bandLowFrac, bandHighInt, bandHighFrac
- Band fields form MHz range: "14.0.14.35" = 14.000–14.350 MHz
- "0.0.0.0" = no auto-band assignment (manual only)

### SWITCHLOCKS details
- Field 3 = antenna locked FROM this radio (owned by another radio)
- Field 4 = this radio's currently active antenna
- Both fields can be "OFF"
- Sent after every SWITCHUPDATE for every radio

## UI design
- Dark theme matching AetherSDR aesthetic (#1e1e1e background, #c0c0c0 text)
- One floating window per radio port (populated from SWITCHADD)
- Each window shows vertical list of antenna buttons
- Three button states:
  - Available: #3a3a3a bg, #555 border
  - SelectedSelf: #1a6b3c bg, #00a855 border (green)
  - LockedOut: #4a1515 bg, #7a2020 border, ForbiddenCursor, non-clickable
- Title bar shows radio name + signal dBm (e.g. "Radio 1 · -52dBm")
- Signal level sourced from SWITCHUPDATE field 5

## Key architecture decisions
- GheClient: persistent QTcpSocket, async readyRead() loop, newline-split buffer
- GheParser: pure parsing, no Qt dependencies beyond QString/QStringList
- AntennaModel: holds antenna list + per-radio active state, derives button states
- SwitchController: owns GheClient + AntennaModel, emits Qt signals to UI
- AntennaButton: custom QWidget (not QPushButton subclass), paintEvent-driven,
  handles three visual states via AntennaState enum
- All radio windows created dynamically after SWITCHADD messages arrive —
  no hardcoded antenna lists

## Enum reference
```cpp
enum class AntennaState { Available, SelectedSelf, LockedOut };
```

## Signal level
Displayed in dBm in window title bar. Sourced from SWITCHUPDATE field 5.
The -52 / -51 values in captures are the GHE wireless link quality, not RF signal.

## Callsign
WT2P (for window titles, about dialog, etc.)

## What NOT to do
- Do not use Qt Designer or .ui files
- Do not add Qt modules beyond Core/Widgets/Network
- Do not hardcode antenna names — always dynamic from SWITCHADD
- Do not use 0x1F as delimiter — real protocol uses dot (.)
- Do not implement Software Control / N1MM integration (future scope)
- Do not implement rotor control (future scope)