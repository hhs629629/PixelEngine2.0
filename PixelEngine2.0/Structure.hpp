
#ifndef INC_Tiles
#include "Tiles.hpp"
#endif // !INC_Tiles

#define INC_Structure

class Structure {
public:
    MaterialInstance* tiles;
    int w;
    int h;//Width, Height로 전체 Structure의 크기 설정

    Structure(int w, int h, MaterialInstance* tiles);
    Structure(SDL_Surface* texture, Material templ);
    Structure() = default;//일반 생성자의 선언으로 인한 디폴트 생성자의 삭제를 막기위함
};
