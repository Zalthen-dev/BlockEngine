#pragma once
#include <algorithm>
#include <cmath>
#include <raylib.h>

#include "../../dependencies/luau/VM/include/lua.h"
#include "../../dependencies/luau/VM/include/lualib.h"
#include "../../dependencies/luau/Compiler/include/luacode.h"

struct Color3 {
    float r, g, b;

    Color3() : r(0), g(0), b(0) {}
    Color3(float red, float green, float blue)
    : r(std::clamp(red, 0.0f, 1.0f)),
    g(std::clamp(green, 0.0f, 1.0f)),
    b(std::clamp(blue, 0.0f, 1.0f)) {}

    Color3(Color rayColor)
    : r((float)rayColor.r / 255.f),
    g((float)rayColor.g / 255.f),
    b((float)rayColor.b / 255.f) {}

    static Color3 fromRGB(int r, int g, int b) {
        return Color3(r / 255.0f, g / 255.0f, b / 255.0f);
    }

    static Color3 fromHSV(float h, float s, float v);
    Color3 Lerp(const Color3& other, float alpha) const;
};

void Color3_Bind(lua_State* L);
