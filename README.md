# GHEControl

A standalone Qt6/C++20 desktop app for controlling antenna switches via the
[Green Heron Engineering](http://www.greenheronengineering.com/) Everywhere TCP
protocol. Companion to AetherSDR — matches its dark aesthetic, no dependency on it.

**Callsign:** WT2P

---

## Features (planned)

- Connects to a GHE Everywhere server over TCP
- One floating window per radio, populated dynamically from `SWITCHADD` messages
- Three antenna button states: Available, Selected (self), Locked-out (another radio)
- Signal level (dBm) displayed live in each window title bar
- Dark theme matching AetherSDR (#1e1e1e / #c0c0c0)
- No hardcoded antenna lists — everything comes from the server on connect

## Status

Early scaffold. The window opens with a dark background and placeholder label.
Protocol parsing and UI are not yet implemented.

---

## Build

### Prerequisites

| Tool | Minimum version |
|------|----------------|
| CMake | 3.21 |
| Ninja | any recent |
| Qt6 | 6.7+ (Core, Widgets, Network) |
| Compiler | GCC 12 / Clang 15 / MSVC 2022 (C++20) |

### Linux

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -j$(nproc)
./build/GHEControl
```

### Windows (Developer PowerShell / after `ilammy/msvc-dev-cmd`)

```powershell
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
.\build\GHEControl.exe
```

---

## Project layout

```
GHEControl/
├── cmake/GHEControlVersion.cmake   # version (0.1.0)
├── src/
│   ├── main.cpp                    # QApplication entry, dark stylesheet
│   ├── MainWindow.h/.cpp           # top-level window
│   ├── core/
│   │   ├── GheClient.h/.cpp        # QTcpSocket wrapper
│   │   ├── GheParser.h/.cpp        # newline/dot-delimited ASCII parser
│   │   ├── AntennaModel.h/.cpp     # per-radio port data model
│   │   └── SwitchController.h/.cpp # orchestration
│   └── ui/
│       └── AntennaButton.h/.cpp    # paintEvent-driven button, AntennaState enum
├── packaging/
│   ├── linux/GHEControl.desktop    # AppImage desktop entry
│   └── windows/GHEControl.iss     # Inno Setup skeleton
└── .github/workflows/build.yml    # CI: ubuntu-22.04 + windows-latest
```

---

## GHE Everywhere protocol (quick reference)

All messages are **newline-terminated, dot-delimited ASCII**.
Lone `.` messages are heartbeat noise — discarded.

### Server → Client

```
SWITCHADD.<radioCount>.<radioName>.<ant1>.<bandLowInt>.<bandLowFrac>.<bandHighInt>.<bandHighFrac>.[...]
SWITCHUPDATE.<radio>.<activeAntenna>.<pttState>.<signalDbm>
SWITCHLOCKS.<radio>.<lockedFromThisRadio>.<thisRadioActive>
```

- `SWITCHADD` is sent once per radio on connect.
- Band range `"14.0.14.35"` = 14.000–14.350 MHz; `"0.0.0.0"` = manual only.
- `SWITCHLOCKS` field 3 = antenna locked by another radio; field 4 = this radio's active antenna. Both can be `"OFF"`.

### Client → Server

```
SET_SWITCH.<radio>.<antenna>
SET_SWITCH.<radio>.OFF
```

---

## Antenna button states

| State | Background | Border | Cursor |
|-------|-----------|--------|--------|
| Available | `#3a3a3a` | `#555555` | Arrow |
| SelectedSelf | `#1a6b3c` | `#00a855` | Arrow |
| LockedOut | `#4a1515` | `#7a2020` | Forbidden |

---

## CI

GitHub Actions builds on every push and PR:

| Job | Runner | Generator |
|-----|--------|-----------|
| `build-linux` | ubuntu-22.04 | Ninja |
| `build-windows` | windows-latest | Ninja + MSVC |

Qt 6.7.x is installed via [`jurplel/install-qt-action@v3`](https://github.com/jurplel/install-qt-action)
and cached between runs.

---

## License

See [LICENSE](LICENSE).
