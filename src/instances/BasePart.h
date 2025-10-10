#pragma once
#include <climits>
#include <cstring>

#include "../datatypes/Vector3.h"
#include "../datatypes/Color3.h"

#include "../core/Signal.h"

#include "Instance.h"

#include "../../dependencies/luau/VM/include/lua.h"
#include "../../dependencies/luau/VM/include/lualib.h"
#include "../../dependencies/luau/Compiler/include/luacode.h"

struct BasePart : public Instance {
    //-- Properties --//
    std::string Name = ClassName;

    Vector3Game Position = Vector3Game{0, 0.5, 0};
    Vector3Game Rotation = Vector3Game{0, 0, 0};
    Vector3Game Size = Vector3Game{4, 1, 2};

    Vector3Game Velocity = Vector3Game{0, 0, 0};
    Vector3Game RotationVelocity = Vector3Game{0, 0, 0};
    bool Anchored = true;
    bool CanCollide = true;
    bool CanQuery = true;
    bool CanTouch = true;
    double Mass;

    Color3 Color = Color3{163.f/255, 162.f/255, 165.f/255};
    bool CastShadow = true;
    float Transparency = 0;
    // Material

    //-- Events --//
    Signal Touched;
    Signal TouchEnded;


    //-- Methods --//

    virtual ~BasePart() = default;

    double GetMass();
};

// Binding

void BasePart_Bind(lua_State* L);
