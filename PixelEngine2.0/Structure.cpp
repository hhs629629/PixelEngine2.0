#include "Structure.hpp"

#include "Macros.hpp"

Structure::Structure(int w, int h, MaterialInstance* tiles) {
    this->w = w;
    this->h = h;
    this->tiles = tiles;
}//생성자

Structure::Structure(SDL_Surface* texture, Material mat) {//생성자
    MaterialInstance* tiles = new MaterialInstance[texture->w * texture->h];//texture랑 같은 크기의 MaterialInstance 배열을 만듬
    for (int x = 0; x < texture->w; x++) {
        for (int y = 0; y < texture->h; y++) {
            Uint32 color = PIXEL(texture, x, y);//texture의 pixel을 얻어 색을 파악
            int alpha = 255;
            if (texture->format->format == SDL_PIXELFORMAT_ARGB8888) {//포맷이 ARGB8888이면
                alpha = (color >> 24) & 0xff;//alpha값 얻음
                if (alpha == 0) {
                    tiles[x + y * texture->w] = Tiles::NOTHING;//만약에 Alpha가 없으면 해당 MaterialInstance를 NOTHING으로 설정
                    continue;
                }
            }
            MaterialInstance prop = MaterialInstance(&mat, color);
            tiles[x + y * texture->w] = prop;//대상 타일을 texture에서 얻어낸 색을 가진 MaterialInstance로 설정
        }
    }
    this->w = texture->w;
    this->h = texture->h;
    this->tiles = tiles;//Texture에 있던 크기, 색 정보 등을 MaterialInstance에 동일하게 설정
}