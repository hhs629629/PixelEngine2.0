#include "stdafx.h"
#include "Texture.hpp"

// test textures
SDL_Surface* Texture::testTexture = Texture::loadTexture("assets/textures/test.png");
SDL_Surface* Texture::dirt1Texture = Texture::loadTexture("assets/textures/testDirt.png");
SDL_Surface* Texture::stone1Texture = Texture::loadTexture("assets/textures/testStone.png");

SDL_Surface* Texture::smoothStone = Texture::loadTexture("assets/textures/smooth_stone_128x.png");
SDL_Surface* Texture::cobbleStone = Texture::loadTexture("assets/textures/cobble_stone_128x.png");
SDL_Surface* Texture::flatCobbleStone = Texture::loadTexture("assets/textures/flat_cobble_stone_128x.png");
SDL_Surface* Texture::smoothDirt = Texture::loadTexture("assets/textures/smooth_dirt_128x.png");
SDL_Surface* Texture::cobbleDirt = Texture::loadTexture("assets/textures/cobble_dirt_128x.png");
SDL_Surface* Texture::flatCobbleDirt = Texture::loadTexture("assets/textures/flat_cobble_dirt_128x.png");
SDL_Surface* Texture::softDirt = Texture::loadTexture("assets/textures/soft_dirt.png");
SDL_Surface* Texture::cloud = Texture::loadTexture("assets/textures/cloud.png");
SDL_Surface* Texture::gold = Texture::loadTexture("assets/textures/gold.png");
SDL_Surface* Texture::goldMolten = Texture::loadTexture("assets/textures/moltenGold.png");
SDL_Surface* Texture::goldSolid = Texture::loadTexture("assets/textures/solidGold.png");
SDL_Surface* Texture::iron = Texture::loadTexture("assets/textures/iron.png");
SDL_Surface* Texture::obsidian = Texture::loadTexture("assets/textures/obsidian.png");

SDL_Surface* Texture::caveBG = Texture::loadTexture("assets/backgrounds/testCave.png");


SDL_Surface* Texture::loadTexture(std::string path) {
    return loadTexture(path, SDL_PIXELFORMAT_ARGB8888);
}

SDL_Surface* Texture::loadTexture(std::string path, Uint32 pixelFormat) {
    SDL_Surface* loadedSurface = SDL_ConvertSurfaceFormat(IMG_Load(path.c_str()), pixelFormat, 0);
    if (loadedSurface == NULL) {
        std::cout<<"Unable to load image SDL_image. Error:"<< IMG_GetError() <<std::endl;
    }

    return loadedSurface;
}

SDL_Surface* Texture::scaleTexture(SDL_Surface* src, float x, float y) {
    SDL_Surface* dest = SDL_CreateRGBSurface(src->flags, src->w * x, src->h * y, src->format->BitsPerPixel, src->format->Rmask, src->format->Gmask, src->format->Bmask, src->format->Amask);

    SDL_Rect* srcR = new SDL_Rect();
    srcR->w = src->w;
    srcR->h = src->h;

    SDL_Rect* dstR = new SDL_Rect();
    dstR->w = dest->w;
    dstR->h = dest->h;

    SDL_FillRect(dest, dstR, 0x00000000);

    SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE); // override instead of overlap (prevents transparent things darkening)

    SDL_BlitScaled(src, srcR, dest, dstR);

    delete srcR;
    delete dstR;

    return dest;
}
