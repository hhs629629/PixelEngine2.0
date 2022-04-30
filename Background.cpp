
#include "Background.hpp"
#include "Textures.hpp"
#include <algorithm>

#define BUILD_WITH_EASY_PROFILER
#include <easy/profiler.h>

#include "ProfilerConfig.hpp"

BackgroundLayer::BackgroundLayer(SDL_Surface* texture, float parallaxX, float parallaxY, float moveX, float moveY) { // 생성자
    this->surface ={Textures::scaleTexture(texture, 1, 1), Textures::scaleTexture(texture, 2, 2), Textures::scaleTexture(texture, 3, 3)};
    this->parralaxX = parallaxX;
    this->parralaxY = parallaxY;
    this->moveX = moveX;
    this->moveY = moveY;
}

void BackgroundLayer::init() {
    EASY_FUNCTION();//Easy profiler의 프로파일링을 위함
    EASY_BLOCK("GPU_CopyImageFromSurface", GPU_PROFILER_COLOR);
    this->texture ={
        GPU_CopyImageFromSurface(this->surface[0]),//GPU를 활용하여 SDL_Surface로 부터 이미지를 얻음
        GPU_CopyImageFromSurface(this->surface[1]),
        GPU_CopyImageFromSurface(this->surface[2])
    };
    EASY_END_BLOCK;
    EASY_BLOCK("GPU_SetImageFilter", GPU_PROFILER_COLOR);
    GPU_SetImageFilter(this->texture[0], GPU_FILTER_NEAREST);//이미지 필터를 설정
    GPU_SetImageFilter(this->texture[1], GPU_FILTER_NEAREST);
    GPU_SetImageFilter(this->texture[2], GPU_FILTER_NEAREST);
    EASY_END_BLOCK;
}

Background::Background(Uint32 solid, std::vector<BackgroundLayer> layers) {
    this->solid = solid;
    this->layers = layers;
}

void Background::init() {
    EASY_FUNCTION();
    for(size_t i = 0; i < layers.size(); i++) {
        layers[i].init();//모든 레이어에 대해 초기화 진행
    }
}

std::vector<BackgroundLayer> testOverworldLayers = { //테스트를 위한 오브젝트 
    BackgroundLayer(Textures::loadTexture("assets/backgrounds/TestOverworld/layer2.png", SDL_PIXELFORMAT_ARGB8888), 0.125, 0.125, 1, 0),
    BackgroundLayer(Textures::loadTexture("assets/backgrounds/TestOverworld/layer3.png", SDL_PIXELFORMAT_ARGB8888), 0.25, 0.25, 0, 0),
    BackgroundLayer(Textures::loadTexture("assets/backgrounds/TestOverworld/layer4.png", SDL_PIXELFORMAT_ARGB8888), 0.375, 0.375, 4, 0),
    BackgroundLayer(Textures::loadTexture("assets/backgrounds/TestOverworld/layer5.png", SDL_PIXELFORMAT_ARGB8888), 0.5, 0.5, 0, 0)
};
Background Backgrounds::TEST_OVERWORLD = Background(0x7EAFCB, testOverworldLayers);

