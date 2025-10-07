#include <cmath>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "../luau/VM/include/lua.h"
#include "../luau/VM/include/lualib.h"
//#include "../luau/Compiler/include/luacode.h"

#include "datatypes/Task.h"
#include "instances/BasePart.h"
#include "instances/Part.h"
#include "raylib.h"
#include "raymath.h"

#include "core/Renderer.h"
#include "core/LuaBindings.h"

lua_State* L_main = nullptr;

Camera3D g_camera{};

static float gYaw = 0.0f;
static float gPitch = 0.0f;

static Color backgroundColor{0, 0, 0, 0};

std::string readFile(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void RenderFrame(Camera3D camera) {
    BeginDrawing();
    ClearBackground(backgroundColor);

    //BeginMode3D(camera);
    RenderScene(camera, g_instances);
    //EndMode3D();

    DrawText("WASD to move camera, Right Click to look around", 10, 10, 20, LIGHTGRAY);
    DrawText("Hold LSHIFT to move slower", 10, 40, 20, LIGHTGRAY);

    DrawRectangle(GetScreenWidth() - 30, GetScreenHeight() - 30, 20, 20, LIGHTGRAY);

    int fps = GetFPS();
    DrawText(TextFormat("FPS: %d", fps), 10, GetScreenHeight() - 30, 20, GREEN);

    EndDrawing();
}

int main(int argc, char** argv) {
    (void)(argc);
    (void)(argv);

    if (argc > 1) {
        printf("Lua script provided! Trying to load...\n");
        std::string scriptText = readFile(argv[1]);

        L_main = luaL_newstate();
        luaL_openlibs(L_main);

        LuaBindings::RegisterScriptBindings(L_main, g_instances, g_camera);

        if (!Task_RunScript(L_main, scriptText)) {
            printf("Failed to load lua script!\n");
            return 1;
        }

        printf("Executed lua script successfully!\n\n");
    } else {
        printf("No lua script provided! Pass the path to it to run it!\n");
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "BlockEngine");
    SetTargetFPS(500);

    PrepareRenderer();

    bool rotatingCamera = false;
    bool warpThisFrame = false;
    Vector2 anchorPos = {0, 0};
    Vector3 worldUp = {0, 1, 0};

    g_camera = {};
    g_camera.up = {0.0f, 1.0f, 0.0f};
    g_camera.fovy = 70.0f;
    g_camera.projection = CAMERA_PERSPECTIVE;
    g_camera.position = {0, 2, -5};
    g_camera.target = {0, 2, 0};

    Part baseplate{};
    baseplate.Color = Color3(Color{92, 92, 92, 0});
    baseplate.Position = Vector3Game{0, -8, 0};
    baseplate.Size = Vector3Game{2048, 16, 2048};

    g_instances.push_back(&baseplate);

    LoadSkybox();

    while (!WindowShouldClose()) {
        //const double time = GetTime();
        const double deltaTime = GetFrameTime();

        float mouseWheelDelta = GetMouseWheelMove();


        float moveSpeed = 25.0f * deltaTime;

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            moveSpeed *= 0.25;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            anchorPos = GetMousePosition();
            rotatingCamera = true;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            rotatingCamera = false;
        }

        if (rotatingCamera) {
            if (!warpThisFrame) {
                Vector2 delta = Vector2Subtract(GetMousePosition(), anchorPos);
                gYaw   += delta.x * 0.004f;
                gPitch += -delta.y * 0.004f;

                const float limit = PI/2 - 0.01f;
                if (gPitch > limit)  gPitch = limit;
                if (gPitch < -limit) gPitch = -limit;
            } else {
                SetMousePosition((int)anchorPos.x, (int)anchorPos.y);
            }

            warpThisFrame = !warpThisFrame;
        }

        Vector3 forward = {
            cosf(gPitch) * cosf(gYaw),
            sinf(gPitch),
            cosf(gPitch) * sinf(gYaw)
        };

        Vector3 right = Vector3CrossProduct(worldUp, forward);
        if (Vector3LengthSqr(right) < 1e-6f) right = Vector3{1,0,0};
        right = Vector3Normalize(right);

        Vector3 up = Vector3CrossProduct(forward, right);

        Vector3 delta = {0, 0, 0};
        if (IsKeyDown(KEY_W)) delta = Vector3Add(delta, Vector3Scale(forward, moveSpeed));
        if (IsKeyDown(KEY_S)) delta = Vector3Subtract(delta, Vector3Scale(forward, moveSpeed));
        if (IsKeyDown(KEY_D)) delta = Vector3Subtract(delta, Vector3Scale(right,   moveSpeed));
        if (IsKeyDown(KEY_A)) delta = Vector3Add(delta, Vector3Scale(right,   moveSpeed));
        if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_E)) delta = Vector3Add(delta, Vector3Scale(up, moveSpeed));
        if (IsKeyDown(KEY_Q)) delta = Vector3Subtract(delta, Vector3Scale(up, moveSpeed));

        g_camera.position = Vector3Add(g_camera.position, delta);
        g_camera.target = Vector3Add(g_camera.position, forward);
        g_camera.up = up;

        TaskScheduler_Step();
        RenderFrame(g_camera);
    }

    for (BasePart* inst : g_instances) {
        delete inst;
    }

    UnloadPrimitiveModels();

    g_tasks.clear();
    g_instances.clear();

    UnloadSkybox();
    CloseWindow();
    return 0;
}
