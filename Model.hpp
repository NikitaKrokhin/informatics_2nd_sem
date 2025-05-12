#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/trigonometric.hpp> // для sin, cos, acos
#include <algorithm> // для std::clamp

#include <array>

#include "Node.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

class Model
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

public:
    Model();
    ~Model();

    SDL_AppResult init();
    SDL_AppResult onEvent(SDL_Event &event);
    SDL_AppResult onKeyDownEvent(SDL_KeyboardEvent &event);
    SDL_AppResult iterate();

protected:
    void clearWindow();

    void initMechanism();
    void solveMechanism();
    void updateCamera();
    void updateNodes();

    flecs::system createRenderMechaismSystem();

    Texture createBaseTexture(double w, double h);
    Texture createRectangleTexture(double w, double h);
    Texture createLinkTexture(double l);

    float calculatePhi2(float phi1, float lambda2, float lambda3, float lambda4);
    float calculatePhi3(float phi1, float lambda2, float lambda3, float lambda4);
    inline float sgn(float x);

    flecs::world ecs;
    flecs::system renderMechanism;
    flecs::entity e0, e1, e2, e3, e4, e5, e6;

    double scale = 100.0;

    Camera camera;

    double phi; // Угол, рад

    glm::dvec2 p0; // База

    double l1; // Длина звена 1, м
    double a1; // Угол поворота звена 1, рад

    double l2; // Длина звена 2, м
    double a2; // Угол поворота звена 2, рад

    double l3; // Длина звена 3, м
    double a3; // Угол поворота звена 3, рад

    glm::dvec2 p1, p2, p3;

    bool showBase = true;
};
