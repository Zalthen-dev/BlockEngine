# BlockEngine (Demo)
<img src="repo/BlockEngineCard.png"/>
<a href="https://discord.gg/xMPCsx3dNf" style="text-decoration:none;">
    <img src="https://img.shields.io/badge/Discord-5865F2?logo=discord&logoColor=white&style=for-the-badge" alt="Discord"/>
</a>

## Supported Platforms
- Linux
- Windows

## Dependencies
- [Luau](https://github.com/luau-lang/luau)
- [Raylib](https://github.com/raysan5/raylib)
- [ImGui](https://github.com/ocornut/imgui)
- [rlImGui](https://github.com/raylib-extras/rlImGui)

## How to Build

### Linux

1. **Clone the repository**
   ```bash
   git clone https://github.com/Zalthen-dev/BlockEngine.git
   cd BlockEngine
   ```
2. **Clone dependencies**
   ```bash
   git clone https://github.com/luau-lang/luau.git dependencies/luau
   git clone https://github.com/raysan5/raylib.git dependencies/raylib
   git clone https://github.com/ocornut/imgui.git dependencies/luau
   git clone https://github.com/raylib-extras/rlImGui.git dependencies/rlImGui
   ```

3. **Install required system libraries**
   ```bash
   sudo apt update
   sudo apt install -y build-essential cmake libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxxf86vm-dev
   ```

4. **Navigate to the build directory**
   ```bash
   cd build
   ```

5. **Configure the project**
   ```bash
   cmake ..
   ```

6. **Build the project**
   ```bash
   make -j$(nproc)
   ```

---

### Windows

1. **Clone the repository**
   ```bash
   git clone https://github.com/Zalthen-dev/BlockEngine.git
   cd BlockEngine
   ```

2. **Clone dependencies**
   ```bash
   git clone https://github.com/luau-lang/luau.git dependencies/luau
   git clone https://github.com/raysan5/raylib.git dependencies/raylib
   ```

3. **Install CMake and Visual Studio**

   - Install [CMake](https://cmake.org/download/)
   - Install Visual Studio with "Desktop development with C++" workload

4. **Navigate to build folder**
   ```bash
   cd build
   ```

5. **Configure the project**
   ```bash
   cmake ..
   ```

6. **Build the project**
   ```bash
   cmake --build .
   ```

## How to Use
BlockEngine supports executing a Luau Script by passing it as command-line argument, and with a familiar scripting API, you can create parts and edit them in real time!
```luau
local part1 = Instance.new("Part")
part1.Color = Color3.new(1,0,0)
part1.Position = Vector3.new(0, 2.5, 0)
part1.Size = Vector3.one * 2
part1.Shape = "CornerWedge"

local part2 = Instance.new("Part")
part2.Color = Color3.new(1,0,0)
part2.Position = Vector3.new(3, 2.5, 0)
part2.Size = Vector3.one * 2
part2.Shape = "Wedge"

local t = 0
while true do
    t += task.wait()
    part1.Rotation = Vector3.new(0, t * 100, 0)
    part1.Color = Color3.fromHSV((t*0.2)%1, 1, 1)

    part2.Rotation = Vector3.new(0, t * 100, 0)
    part2.Color = Color3.fromHSV((t*0.2)%1, 1, 1)
end
```

# Checklist
Below is what you can expect for the future in BlockEngine's development! Expect this big list to expand as time goes on!
- [ ] Limiting `os` library
- [ ] Signals in Lua
- [ ] Instance System
- [ ] Physics
- [ ] Data types
  - [ ] CFrame
  - [ ] Random
  - [ ] game
  - [ ] workspace
- [ ] Services
  - [ ] RunService
  - [ ] Lighting
  - [ ] StarterGui
- [ ] Lighting system
  - [ ] Sun lighting
  - [ ] Shadows
  - [ ] Lights from Instance Lights

# How to Help
You can help out by using BlockEngine and reporting any bugs you find!

If you know how to code, help out by creating pull requests to add and change code!
