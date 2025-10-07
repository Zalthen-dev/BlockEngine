#include "Renderer.h"

//TODO: add shadows and lights
const int SHADOW_MAP_SIZE = 1024;

Texture2D g_defaultTexture;

static Color Color3ToColor(const Color3& c) {
    return Color{
        (unsigned char)roundf(c.r * 255.0f),
        (unsigned char)roundf(c.g * 255.0f),
        (unsigned char)roundf(c.b * 255.0f),
        255
    };
}

Texture2D GenerateDefaultTexture(int width, int height) {
    Image img = GenImageColor(width, height, BLANK);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char n = (unsigned char)(rand() % 16 + 239);
            Color c = { n, n, n, 255 };
            ImageDrawPixel(&img, x, y, c);
        }
    }

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    return tex;
}

void DrawPart(const Part part) {
    rlPushMatrix();

    rlTranslatef(part.Position.x, part.Position.y, part.Position.z);

    rlRotatef(part.Rotation.x, 1, 0, 0);
    rlRotatef(part.Rotation.y, 0, 1, 0);
    rlRotatef(part.Rotation.z, 0, 0, 1);

    rlScalef(part.Size.x, part.Size.y, part.Size.z);

    Color color = Color3ToColor(part.Color);

    Model* model = nullptr;

    Vector3 pos = {0, 0, 0};
    Vector3 size = {1, 1, 1};

    if (part.Shape == "Block") {
        model = GetPrimitiveModel(PrimitiveShape::Block);
    } else if (part.Shape == "Sphere") {
        model = GetPrimitiveModel(PrimitiveShape::Sphere);
    } else if (part.Shape == "Cylinder") {
        model = GetPrimitiveModel(PrimitiveShape::Cylinder);
    } else if (part.Shape == "Wedge") {
        model = GetPrimitiveModel(PrimitiveShape::Wedge);
    } else if (part.Shape == "CornerWedge") {
        model = GetPrimitiveModel(PrimitiveShape::CornerWedge);
    } else {
        model = GetPrimitiveModel(PrimitiveShape::Block);
    }

    if (model) {
        model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = g_defaultTexture;
        DrawModel(*model, {0, 0, 0}, 1.0f, color);
    }

    rlPopMatrix();
}

void RenderScene(Camera3D g_camera, const std::vector<BasePart*> g_instances) {
    BeginMode3D(g_camera);
    DrawSkybox();

    for (BasePart* inst : g_instances) {
        if (inst->ClassName == "Part") {
            Part* p = dynamic_cast<Part*>(inst);
            if (p) DrawPart(*p);
        }
    }

    EndMode3D();
}

void PrepareRenderer() {
    g_defaultTexture = GenerateDefaultTexture();

    InitPrimitiveModels();
}
