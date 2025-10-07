#include "SkyboxRenderer.h"

Skybox g_skybox;

static int u_inner = -1, u_outer = -1, u_transition = -1;

static const char* SKY_VS = R"(#version 330
in vec3 vertexPosition;
out vec3 dir;
uniform mat4 matView;
uniform mat4 matProjection;
void main() {
    dir = vertexPosition;
    mat3 R = mat3(matView);
    mat4 viewNoTrans = mat4(
        vec4(R[0], 0.0),
        vec4(R[1], 0.0),
        vec4(R[2], 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
    gl_Position = matProjection * viewNoTrans * vec4(vertexPosition, 1.0);
})";

static const char* SKY_FS = R"(#version 330
in vec3 dir;
out vec4 FragColor;
uniform vec3 innerColor; // bottom color (ground)
uniform vec3 outerColor; // top color (sky)
void main() {
    vec3 nd = normalize(dir);
    float t = clamp(nd.y * 0.5 + 0.5, 0.0, 1.0);
    vec3 col = mix(innerColor, outerColor, t);
    // Gamma out so sky matches lit pass
    FragColor = vec4(pow(col, vec3(1.0/2.2)), 1.0);
})";

void LoadSkybox() {
    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);

    g_skybox.model = LoadModelFromMesh(cubeMesh);
    g_skybox.shader = LoadShaderFromMemory(SKY_VS, SKY_FS);

    u_inner = GetShaderLocation(g_skybox.shader, "innerColor");
    u_outer = GetShaderLocation(g_skybox.shader, "outerColor");

    g_skybox.model.materials[0].shader = g_skybox.shader;
}

void UnloadSkybox() {
    UnloadModel(g_skybox.model);
    UnloadShader(g_skybox.shader);
}

void DrawSkybox() {
    float inner[3] = {0.25f, 0.55f, 0.65f};
    float outer[3] = {0.05f, 0.15f, 0.45f};

    rlDisableDepthTest();
    rlDisableDepthMask();
    rlDisableBackfaceCulling();
    BeginShaderMode(g_skybox.shader);

    SetShaderValue(g_skybox.shader, u_inner, inner, SHADER_UNIFORM_VEC3);
    SetShaderValue(g_skybox.shader, u_outer, outer, SHADER_UNIFORM_VEC3);
    DrawModel(g_skybox.model, {0, 0, 0}, 1000000.0f, WHITE);

    EndShaderMode();
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
    rlEnableDepthTest();
}
