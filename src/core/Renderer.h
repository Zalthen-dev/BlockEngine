#include <vector>

#include "raylib.h"
#include "rlgl.h"

#include "../datatypes/Vector3.h"
#include "../datatypes/Color3.h"
#include "../datatypes/Instance.h"

#include "../instances/BasePart.h"
#include "../instances/Part.h"

#include "SkyboxRenderer.h"
#include "PrimitiveModels.h"

extern Texture2D g_defaultTexture;

Texture2D GenerateDefaultTexture(int width = 128, int height = 128);

void PrepareRenderer();
void RenderScene(Camera3D g_camera, std::vector<BasePart*> g_instances);
void UnprepareRenderer();
