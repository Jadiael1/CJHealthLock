# CJHealthLock - GTA San Andreas

![GTA San Andreas](assets/images/CJHealthLock.png)

## Overview
A memory manipulation tool for GTA San Andreas that enables god mode (infinite health) for CJ. This trainer was specifically developed for GTA San Andreas version 1.0.1.32 running through the Rockstar Games Launcher.

## Features
- 🛡️ Infinite Health (180.0)
- ⌨️ Toggle with F3 key
- 🎮 Real-time memory manipulation
- 🔄 Automatic process detection

## Technical Details
- **Game Version**: 1.0.1.32 (Rockstar Games Launcher)
- **Development Date**: October 27, 2025 - 20:40
- **Memory Analysis**: Memory addresses were discovered using CheatEngine's "Pointer Scan For This Address" feature
- **Base Address**: 0x0080104C
- **Target Process**: gta_sa.exe

## Requirements
- Windows OS
- GTA San Andreas v1.0.1.32 (Rockstar Games Launcher Edition)
- Administrative privileges
- [TDM-GCC-32 compiler](https://jmeubank.github.io/tdm-gcc/download/)

## Building
```bash
g++ main.cpp -o CJHealthLock.exe -lpsapi
```

## Usage
1. Launch GTA San Andreas
2. Run CJHealthLock.exe as administrator
3. Press F3 to toggle god mode on/off
4. Enjoy immortality! 🎮

## Memory Analysis Process
The memory addresses were carefully mapped using CheatEngine's pointer scanning capabilities:
```
Base Address -> [BaseAddr] + 0x540 -> Health Value
```

## Disclaimer
⚠️ This tool is for educational purposes only. Use at your own risk. We are not affiliated with Rockstar Games or Take-Two Interactive.

## Technical Implementation
- Uses Windows API for process manipulation
- Implements multi-level pointer dereferencing
- Real-time memory writing at 200ms intervals
- Process validation and module enumeration

## Contributing
Feel free to fork and submit pull requests. All contributions are welcome!

---
*"Grove Street. Home. At least it was before I turned on infinite health." - CJ (probably)*

*Last updated: 27/10/2025 20:40*