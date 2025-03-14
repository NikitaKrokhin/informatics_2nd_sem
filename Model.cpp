#include "Model.hpp"

Model::Model()
{
    SDL_Log ("[Model::Model] %p", this);
}

Model::~Model()
{
    SDL_Log ("[Model::Model] %p", this);
}

SDL_AppResult Model::init()
{
    if (!SDL_INIT_VIDEO)
    {
        SDL_Log("Couldn't init SDL: %s" , SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if(!SDL_CreateWindowAndRenderer("Model" , 480 , 360 ,0 , &this->window, &this->renderer))
    {
        SDL_Log("Couldn't init SDL: %s" , SDL_GetError());
        return SDL_APP_FAILURE;
    }

    this->initMechanism();

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::onEvent(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}
SDL_AppResult Model::iterate()
{
    this->clearWindow();

    this->ecs.progress();

    SDL_RenderPresent(this->renderer);
    return SDL_APP_CONTINUE;
}

void Model::clearWindow()
{
    SDL_SetRenderDrawColorFloat(this->renderer,0.5f,0.5f,0.5f,SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(this->renderer);
}

void Model::initMechanism()
{
    this->renderMechanism = this->createRenderMechanismSystem();
    this->e0 = this->ecs.entity().insert([this](Node &n, Texture &t)
        {
            n.position = {100.0f , 200.0f};
            n.angle = 0.0;
            t = this->createBaseTexture();
        });
    this->e1 = this->ecs.entity().insert([this](Node &n, Texture &t)
    {
        n.position = {300.0f , 100.0f};
        n.angle = -45.0;
        t = this->createBaseTexture();
    });
}

flecs::system Model::createRenderMechanismSystem()
{
 return this->ecs.system<const Node, const Texture>().each([this](const Node &n , const Texture &t)
    {
        SDL_FRect r = {n.position.x - t.center.x , n.position.y - t.center.y , t.rect.w, t.rect.h};
        if(n.angle)
        {
            SDL_RenderTextureRotated(this->renderer, t.texture, &t.rect,&r,n.angle, &t.center, SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderTexture(this->renderer, t.texture, &t.rect, &r);
        }
    });

}

Texture Model::createBaseTexture()
{
    Texture result;
    result.rect = {0.0f, 0.0f, 100.0f , 100.0f};

    SDL_Surface *surface = SDL_CreateSurface(static_cast<int>(result.rect.w),static_cast<int>(result.rect.h) , SDL_PIXELFORMAT_RGBA32);//Ширина высота цветное изображение
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    SDL_FPoint base[4];
    result.center = base[0]= base[3]= {50.0f, 50.0f};
    base[1]= {10.0f , 90.0f};
    base[2]= {90.0f, 90.0f};

    SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.0f, 0.0f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderLines(renderer, base , SDL_arraysize(base));
    SDL_RenderPresent(renderer);
    result.texture = SDL_CreateTextureFromSurface(this->renderer,surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);

    return result;
}
