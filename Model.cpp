#include "Model.hpp"


Model::Model()
{
    SDL_Log("[Model::Model] %p", this);
}

Model::~Model()
{
    SDL_Log("[Model::~Model] %p", this);
}

SDL_AppResult Model::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't init SDL: %s"
                , SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(
            "Model"
            , 600, 600
            , SDL_WINDOW_RESIZABLE
            , &this->window
            , &this->renderer
            )
        )
    {
        SDL_Log("Couldn't create window/renderer: %s"
                , SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Инициализация механизма
    this->initMechanism();

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::onEvent(SDL_Event &event)
{
    switch (event.type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        return this->onKeyDownEvent(event.key);
    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::onKeyDownEvent(SDL_KeyboardEvent &event)
{
    switch (event.key) {
    case SDLK_B:
        this->showBase = !this->showBase;
        if (this->showBase)
        {
            this->e0.enable();
            this->e3.enable();
        } else {
            this->e0.disable();
            this->e3.disable();
        }
        break;
    case SDLK_SPACE:
        this->phi += glm::radians(5.0);
        break;
    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::iterate()
{
    this->clearWindow();

    this->solveMechanism();

    this->updateCamera();

    this->updateNodes();

    this->ecs.progress();

    SDL_RenderPresent(this->renderer);

    return SDL_APP_CONTINUE;
}

void Model::clearWindow()
{
    // Задаем цвет фона
    SDL_SetRenderDrawColorFloat(
        this->renderer
        , 0.6f, 0.8f, 0.9f
        , SDL_ALPHA_OPAQUE_FLOAT);
    // Очищаем экран
    SDL_RenderClear(this->renderer);
}

void Model::initMechanism()
{
    this->phi = 0.0;

    this->p0 = {0.0, 0.0}; //первая база
    this->p2 = {1.0, 0.0}; //вторая база

    this->l1 = 0.5;
    this->l2 = 1.0;
    this->l3 = 3.5;

    this->renderMechanism = this->createRenderMechaismSystem();

    this->e0 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createBaseTexture(0.4, 0.2);
        });

    this->e1 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createLinkTexture(this->l1);
        });

    this->e2 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createLinkTexture(this->l2);
        });

    this->e3 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createBaseTexture(0.4, 0.2);
        });
    this->e4 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createLinkTexture(this->l3);
        });
    this->e5 = this->ecs.entity()
        .insert([this](Texture &t)
        {
             t = this->createRectangleTexture(0.4, 0.2);
        });
    this->e6 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createRectangleTexture(0.4, 0.2);
        });
}

void Model::solveMechanism()
{
    float lambda2 = 1.0f/0.5f;
    float lambda3 = 1.0f/0.5f;
    float lambda4 = 1.0f/0.5f;

    this->a1 = phi;
    this->p1 = {this->l1, 0.0};
    this->p1 = glm::rotate(this->p1, this->a1);
    this->p1 = this->p0 + this->p1;

    this->a2 = calculatePhi2(a1, lambda2, lambda3, lambda4);
    this->a3 = calculatePhi3(a1, lambda2, lambda3, lambda4);

    this->p3 = {((1.5*glm::cos(a3))/glm::sin(a3)+1.0), 1.5};
}

void Model::updateNodes()
{
    this->e0.set<Node>(this->camera.toRendererNode(
        this->p0
    ));
    this->e1.set<Node>(this->camera.toRendererNode(
        this->p0,
        this->a1
    ));
    this->e2.set<Node>(this->camera.toRendererNode(
        this->p1,
        this->a2
    ));
    this->e3.set<Node>(this->camera.toRendererNode(
        this->p2
    ));
    this->e4.set<Node>(this->camera.toRendererNode(
        this->p2 ,
        this->a3
        ));
    this->e5.set<Node>(this->camera.toRendererNode(
        this->p3 ,
        this->a3
        ));
    this->e6.set<Node>(this->camera.toRendererNode(
        this->p3
        ));
}

void Model::updateCamera()
{
    int w, h;
    if (!SDL_GetWindowSize(this->window, &w, &h))
    {
        SDL_Log("Couldn't get window size: %s",
                SDL_GetError());
        return;
    }
    SDL_FRect rect = {
        0.0f, 0.0f,
        static_cast<float>(w),
        static_cast<float>(h),
    };
    this->camera.setRendererRect(rect);
    glm::dvec2 pos = {-2.5, -2.0};
    glm::dvec2 size = {
        static_cast<double>(w) / this->scale,
        static_cast<double>(h) / this->scale
    };
    this->camera.setSceneRect(pos, size);
}

flecs::system Model::createRenderMechaismSystem()
{
    return this->ecs.system<const Node, const Texture>()
        .each([this](const Node &n, const Texture &t)
        {
            SDL_FRect r = {
                n.position.x - t.center.x
                , n.position.y - t.center.y
                , t.rect.w, t.rect.h};
            if (n.angle)
            {
              SDL_RenderTextureRotated(
                  this->renderer
                  , t.texture
                  , &t.rect
                  , &r
                  , n.angle
                  , &t.center
                  , SDL_FLIP_NONE
                  );
            }
            else
            {
              SDL_RenderTexture(
                  this->renderer
                  , t.texture
                  , &t.rect
                  , &r
                  );
            }
        });
}

Texture Model::createBaseTexture(double w, double h)
{
    Texture result;

    Camera camera;
    glm::dvec2 pos = {-w / 2.0 - 1.0, -h - 1.0};
    glm::dvec2 size = {w + 2.0, h + 2.0};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast<float>(-pos.x * this->scale),
        static_cast<float>(-pos.y * this->scale)
    };
    result.rect = {
        0.0f, 0.0f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);

    SDL_Surface *surface = SDL_CreateSurface(
        static_cast<int>(result.rect.w) // Ширина
        , static_cast<int>(result.rect.h) // Высота
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer *renderer =
        SDL_CreateSoftwareRenderer(surface);
    SDL_FPoint base[4];
    result.center = base[0] = base[3] =
        camera.toRenderer({0.0, 0.0});
    base[1] = camera.toRenderer({-w / 2.0f, -h});
    base[2] = camera.toRenderer({w / 2.0f, -h});
    SDL_SetRenderDrawColorFloat(
        renderer
        , 0.0f, 0.0f, 0.0f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderLines(
        renderer
        , base
        , SDL_arraysize(base)
        );
    SDL_RenderPresent(renderer);
    result.texture =
        SDL_CreateTextureFromSurface(
            this->renderer
            , surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);
    return result;
}

Texture Model::createLinkTexture(double l)
{
    Texture result;

    Camera camera;
    glm::dvec2 pos = {-1.0, -1.0};
    glm::dvec2 size = {l + 2.0, 2.0};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast<float>(-pos.x * this->scale),
        static_cast<float>(-pos.y * this->scale)
    };
    result.rect = {
        0.0f, 0.0f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);

    std::array<SDL_FPoint, 2> link;
    link[0] = camera.toRenderer({0.0, 0.0});
    link[1] = camera.toRenderer({l, 0.0});

    SDL_Surface *surface = SDL_CreateSurface(
        static_cast<int>(result.rect.w) // Ширина
        , static_cast<int>(result.rect.h) // Высота
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer *renderer =
        SDL_CreateSoftwareRenderer(surface);

    SDL_SetRenderDrawColorFloat(
        renderer
        , 0.0f, 0.0f, 0.0f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderLines(
        renderer
        , link.data()
        , link.size()
        );
    SDL_RenderPresent(renderer);

    result.texture =
        SDL_CreateTextureFromSurface(
            this->renderer
            , surface);

    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);

    return result;
}

inline float Model::sgn(float x) {
    return (x > 0.0f) ? 1.0f : ((x < 0.0f) ? -1.0f : 0.0f);
}

float Model::calculatePhi2(float phi1, float lambda2, float lambda3, float lambda4) {
    using namespace glm;

    const float sinPhi1 = sin(phi1);
    const float cosPhi1 = cos(phi1);

    // Первое слагаемое
    const float denominator = sqrt(1.0f + lambda4*lambda4 - 2.0f*lambda4*cosPhi1);
    if (denominator <= 0.0001f) return 0.0f; // Защита от деления на ноль

    const float arg1 = (lambda4 - cosPhi1) / denominator;
    const float clampedArg1 = std::clamp(arg1, -1.0f, 1.0f); // Ограничение для acos
    const float term1 = sgn(-sinPhi1) * acos(clampedArg1);

    // Второе слагаемое
    const float numerator = 1.0f + lambda2*lambda2 + lambda4*lambda4
                            - lambda3*lambda3 - 2.0f*lambda4*cosPhi1;
    const float denominator2 = 2.0f * lambda2 * denominator;
    if (fabs(denominator2) < 0.0001f) return 0.0f;

    const float arg2 = numerator / denominator2;
    const float clampedArg2 = std::clamp(arg2, -1.0f, 1.0f);
    const float term2 = acos(clampedArg2);

    return term1 + term2;
}

float Model::calculatePhi3(float phi1, float lambda2, float lambda3, float lambda4) {
    using namespace glm;

    const float sinPhi1 = sin(phi1);
    const float cosPhi1 = cos(phi1);

    // Общий знаменатель для обоих слагаемых
    const float denominator = sqrt(1.0f + lambda4*lambda4 - 2.0f*lambda4*cosPhi1);
    if (denominator <= 0.0001f) return 0.0f; // Защита от деления на ноль

    // Первое слагаемое (аналогично формуле для phi2)
    const float arg1 = (lambda4 - cosPhi1) / denominator;
    const float clampedArg1 = std::clamp(arg1, -1.0f, 1.0f);
    const float term1 = sgn(-sinPhi1) * acos(clampedArg1);

    // Второе слагаемое (новый числитель и знаменатель)
    const float numerator = lambda2*lambda2 - lambda3*lambda3 - lambda4*lambda4
                            + 2.0f*lambda4*cosPhi1 - 1.0f;
    const float denominator2 = 2.0f * lambda3 * denominator;
    if (fabs(denominator2) < 0.0001f) return 0.0f; // Проверка знаменателя

    const float arg2 = numerator / denominator2;
    const float clampedArg2 = std::clamp(arg2, -1.0f, 1.0f);
    const float term2 = acos(clampedArg2);

    return term1 + term2;
}
Texture Model::createRectangleTexture(double w,double h)
{
    Texture result;

    Camera camera;
    glm::dvec2 pos =  {0.0, 0.0};
    glm::dvec2 size = {w, h};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast <float>(size.x/2. * this->scale),
        static_cast <float>(size.y/2. * this->scale)
    };
    result.rect = {
        0.0f, 0.0f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);
    std::array<SDL_FPoint, 5> dot;
    dot[0] = camera.toRenderer({0.01       , .01  });
    dot[1] = camera.toRenderer({size.x-.01, .01   });
    dot[2] = camera.toRenderer({size.x-.01, size.y-0.01});
    dot[3] = camera.toRenderer({0.01       , size.y-0.01});
    dot[4] = camera.toRenderer({0.01       , .01   });

    SDL_Surface *surface = SDL_CreateSurface(
        static_cast<int>(result.rect.w) // Ширина
        , static_cast<int>(result.rect.h) // Высота
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer *renderer =
        SDL_CreateSoftwareRenderer(surface);

    SDL_SetRenderDrawColorFloat(
        renderer
        , 0.0f, 0.0f, 0.0f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderLines(
        renderer
        , dot.data()
        , dot.size()
        );
    SDL_RenderPresent(renderer);

    result.texture =
        SDL_CreateTextureFromSurface(
            this->renderer
            , surface);

    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);

    return result;
}
