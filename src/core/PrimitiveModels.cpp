#include "PrimitiveModels.h"
#include <raylib.h>

extern Texture2D g_defaultTexture;

static std::unordered_map<PrimitiveShape, Model> g_models;

void SetMeshTextureCoords(Mesh* mesh, const Vector2* texcoords) {
    if (!mesh || !texcoords) return;

    if (!mesh->texcoords)
        mesh->texcoords = (float*)RL_CALLOC(mesh->vertexCount * 2, sizeof(float));

    for (unsigned int i = 0; i < mesh->vertexCount; i++) {
        mesh->texcoords[i*2 + 0] = texcoords[i].x;
        mesh->texcoords[i*2 + 1] = texcoords[i].y;
    }
}

static Mesh GenMeshWedge() {
    Mesh mesh = {0};

    Vector3 vertices[] = {
        // bottom
        {-0.5, -0.5, -0.5},
        {0.5, -0.5, -0.5},
        {-0.5, -0.5, 0.5},
        {0.5, -0.5, 0.5},

        // top
        {-0.5, 0.5, -0.5},
        {0.5, 0.5, -0.5}
    };

    // TODO: correct UV's on back, left, and right faces
    Vector2 uv[] = {
        // bottom face
        {0,0}, {1,0}, {0,1}, {1,1},
        // top face
        {0,0}, {1,0}, {0,1}, {1,0}, {1,1}, {0,1},
        // left face
        {0,0}, {1,0}, {0,1},
        // right face
        {0,0}, {1,0}, {0,1}
    };

    unsigned short indices[] = {
        // bottom square
        0, 1, 2,
        1, 3, 2,

        // back square
        0, 4, 5,
        1, 0, 5,

        // left & right triangles
        0, 2, 4,
        1, 5, 3,

        // top face
        4, 2, 5,
        5, 2, 3
    };

    mesh.vertexCount = 6;
    mesh.triangleCount = 8;
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount*3*sizeof(float));
    for(int i=0;i<mesh.vertexCount;i++){
        mesh.vertices[i*3+0] = vertices[i].x;
        mesh.vertices[i*3+1] = vertices[i].y;
        mesh.vertices[i*3+2] = vertices[i].z;
    }

    SetMeshTextureCoords(&mesh, uv);

    mesh.indices = (unsigned short*)MemAlloc(mesh.triangleCount*3*sizeof(unsigned short));
    memcpy(mesh.indices, indices, mesh.triangleCount*3*sizeof(unsigned short));

    UploadMesh(&mesh, true);
    return mesh;
}

static Mesh GenMeshCornerWedge() {
    Mesh mesh = {0};

    Vector3 vertices[] = {
        // bottom square
        {-0.5, -0.5, -0.5},
        { 0.5, -0.5, -0.5},
        {-0.5, -0.5,  0.5},
        { 0.5, -0.5,  0.5},

        // top vertex
        {-0.5, 0.5, 0.5}
    };

    unsigned short indices[] = {
        // bottom square
        0, 1, 2,
        1, 3, 2,

        // right & front triangle
        0, 2, 4,
        4, 2, 3,

        // back & left triangle
        1, 0, 4,
        1, 4, 3
    };

    mesh.vertexCount = 5;
    mesh.triangleCount = 6;
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount*3*sizeof(float));
    for(int i=0;i<mesh.vertexCount;i++){
        mesh.vertices[i*3+0] = vertices[i].x;
        mesh.vertices[i*3+1] = vertices[i].y;
        mesh.vertices[i*3+2] = vertices[i].z;
    }

    mesh.indices = (unsigned short*)MemAlloc(mesh.triangleCount*3*sizeof(unsigned short));
    memcpy(mesh.indices, indices, mesh.triangleCount*3*sizeof(unsigned short));

    UploadMesh(&mesh, true);
    return mesh;
}

void InitPrimitiveModels() {
    Model block = LoadModelFromMesh(GenMeshCube(1.f, 1.f, 1.f));
    Model cylinder = LoadModelFromMesh(GenMeshCylinder(0.5f, 1.f, 16));
    Model sphere = LoadModelFromMesh(GenMeshSphere(0.5, 16, 16));
    Model wedge = LoadModelFromMesh(GenMeshWedge());
    Model cornerWedge = LoadModelFromMesh(GenMeshCornerWedge());

    block.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = g_defaultTexture;
    cylinder.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = g_defaultTexture;
    sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = g_defaultTexture;
    wedge.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = g_defaultTexture;
    cornerWedge.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = g_defaultTexture;

    g_models[PrimitiveShape::Block] = block;
    g_models[PrimitiveShape::Cylinder] = cylinder;
    g_models[PrimitiveShape::Sphere] = sphere;
    g_models[PrimitiveShape::Wedge] = wedge;
    g_models[PrimitiveShape::CornerWedge] = cornerWedge;
}

void UnloadPrimitiveModels() {
    for (auto& [_, model] : g_models)
        UnloadModel(model);
    g_models.clear();
}

Model* GetPrimitiveModel(PrimitiveShape shape) {
    auto it = g_models.find(shape);
    if (it != g_models.end())
        return &it->second;

    return nullptr;
}
