#pragma once
#include <cmath>
#include <cstring>
#include <ostream>
#include <raylib.h>

#include "../../luau/VM/include/lua.h"
#include "../../luau/VM/include/lualib.h"
#include "../../luau/Compiler/include/luacode.h"


struct Vector3Game {
    float x, y, z;

    Vector3Game() {x = 0; y = 0; z = 0;}
    Vector3Game(float X,float Y,float Z) {x = X; y = Y; z = Z; }

    ::Vector3 toRaylib() {return {x, y, z}; }
    Vector3Game fromRaylib(Vector3 v3) {return {v3.x, v3.y, v3.z}; }

    Vector3Game operator+(const Vector3Game& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vector3Game operator-(const Vector3Game& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vector3Game operator-() const { return {-x, -y, -z}; }
    Vector3Game operator*(float s) const { return {x * s, y * s, z * s}; }
    Vector3Game operator/(float s) const { return {x / s, y / s, z / s}; }

    float magnitude() const { return sqrtf(x*x + y*y + z*z); }
    Vector3Game unit() {
        float mag = magnitude();
        if (mag > 0.00001f) return *this / mag;
        return {0, 0, 0};
    }

    Vector3Game abs() const { return { fabsf(x), fabsf(y), fabsf(z)}; }
    Vector3Game ceil() const { return { float(int(x + 1)), float(int(y + 1)), float(int(z + 1))}; }
    Vector3Game floor() const { return { float(int(x)), float(int(y)), float(int(z))}; }
    float dot(const Vector3Game& v) const { return x*v.x + y*v.y + z*v.z; }
    Vector3Game cross(const Vector3Game& v) const { return { y * v.z - z * v.y,  z * v.x - x * v.z,  x * v.y - y * v.x }; }
    Vector3Game lerp(const Vector3Game& goal, float alpha) const { return *this + (goal - *this) * alpha; }
    bool fuzzyequal(const Vector3Game& v, float epsilon = 1e-5) const { return fabsf(magnitude()) - fabsf(v.magnitude())<epsilon ; }
};

void Vector3Game_Bind(lua_State* L);
