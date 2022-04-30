
#include "Structures.hpp"
#include <algorithm>

#ifndef INC_Textures
#include "Textures.hpp"
#endif

#undef min
#undef max

Structure Structures::makeTree(World world, int x, int y) {//나무 구조를 만드는 팩터리 메서드
    int w = 50 + rand() % 10;
    int h = 80 + rand() % 20;//무작위의 나무 구조를 만들기 위한 랜덤
    MaterialInstance* tiles = new MaterialInstance[w * h]; //나무에 필요한 MaterialInstance 를 적절한 크기로 구성

    for (int tx = 0; tx < w; tx++) {
        for (int ty = 0; ty < h; ty++) {
            tiles[tx + ty * w] = Tiles::NOTHING; // tile값 NOTHING으로 초기화
        }
    }

    int trunk = 3 + rand() % 2;

    float cx = w / 2;
    float dcx = (((rand() % 10) / 10.0) - 0.5) / 3.0;
    for (int ty = h - 1; ty > 20; ty--) {
        int bw = trunk + std::max((ty - h + 10) / 3, 0);
        for (int xx = -bw; xx <= bw; xx++) {
            tiles[((int)cx + xx - (int)(dcx * (h - 30))) + ty * w] = MaterialInstance(&Materials::GENERIC_PASSABLE, xx >= 2 ? 0x683600 : 0x7C4000);//A
        }
        cx += dcx;
    }//나무 밑동 구성을 위한 코드

    for (int theta = 0; theta < 360; theta += 1) {
        double p = world.noise.GetPerlin(std::cos(theta * 3.1415 / 180.0) * 4 + x, std::sin(theta * 3.1415 / 180.0) * 4 + y, 2652);
        float r = 15 + (float)p * 6;
        for (float d = 0; d < r; d += 0.5) {
            int tx = cx - (int)(dcx * (h - 30)) + d * std::cos(theta * 3.1415 / 180.0);
            int ty = 20 + d * std::sin(theta * 3.1415 / 180.0);
            if (tx >= 0 && ty >= 0 && tx < w && ty < h) {
                tiles[tx + ty * w] = MaterialInstance(&Materials::GENERIC_PASSABLE, r - d < 0.5f ? 0x00aa00 : 0x00ff00);//B
            }
        }
    }//펄린 노이즈를 활용한 무작위 구성의 잎 생성

    int nBranches = rand() % 3;
    bool side = rand() % 2; // false = right, true = left
    for (int i = 0; i < nBranches; i++) {
        int yPos = 20 + (h - 20) / 3 * (i + 1) + rand() % 10;
        float tilt = ((rand() % 10) / 10.0 - 0.5) * 8;
        int len = 10 + rand() % 5;
        for (int xx = 0; xx < len; xx++) {
            int tx = (int)(w / 2 + dcx * (h - yPos)) + (side ? 1 : -1) * (xx + 2) - (int)(dcx * (h - 30));
            int th = 3 * (1 - (xx / (float)len));
            for (int yy = -th; yy <= th; yy++) {
                int ty = yPos + yy + (xx / (float)len * tilt);
                if (tx >= 0 && ty >= 0 && tx < w && ty < h) {
                    tiles[tx + ty * w] = MaterialInstance(&Materials::GENERIC_PASSABLE, yy >= 2 ? 0x683600 : 0x7C4000);//C <<A / B / C>> 대상 MaterialInstance의 색의 컬러코드
                }
            }
        }
        side = !side;
    }//가지 생성


    return Structure(w, h, tiles);
}

Structure Structures::makeTree1(World world, int x, int y) {
    char buff[30];
    snprintf(buff, sizeof(buff), "assets/objects/tree%d.png", rand() % 8 + 1);
    std::string buffAsStdStr = buff;
    return Structure(Textures::loadTexture(buffAsStdStr.c_str()), Materials::GENERIC_PASSABLE);//에셋으로 부터 나무 불러옴
}