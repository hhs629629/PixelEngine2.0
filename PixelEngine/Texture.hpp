#pragma once

class Texture
{
public:
	Texture();
	Texture(SDL_Renderer *_ren, bool &_success, SDL_Rect _rect);
	Texture(SDL_Renderer *_ren, Sint32 _x, Sint32 _y, TTF_Font *_font);
	~Texture();

	bool isButtonClicked(const SDL_Point *_pos, Uint8 &_button) const;

	void renderTexture();
	void changeTexture(Uint32 *_buffer, Uint32 _size);

private:
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	TTF_Font *font;
	SDL_Rect rect;
	SDL_Rect *button;
	Uint8 buttonCount;
};