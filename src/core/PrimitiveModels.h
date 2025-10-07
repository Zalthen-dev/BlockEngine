#pragma once
#include "raylib.h"
#include "raymath.h"
#include <unordered_map>
#include <cstring>

enum class PrimitiveShape {
    Block,
    Sphere,
    Cylinder,
    Wedge,
    CornerWedge,
};

void InitPrimitiveModels();
void UnloadPrimitiveModels();
Model* GetPrimitiveModel(PrimitiveShape shape);
