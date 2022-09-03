#pragma once

class Texture
{
public:

	Texture(SDL_Renderer *_ren, bool &_success, SDL_Rect _rect);
	Texture(SDL_Renderer *_ren, Sint32 _x, Sint32 _y, TTF_Font *_font);
	Texture(SDL_Renderer *_ren, bool &_success, SDL_Rect _rect, TTF_Font *_font, std::string _text);
	~Texture();

	bool isButtonClicked(const SDL_Point *_pos, Uint8 &_button) const;

	void renderTexture();
	void changeTexture(Uint32 *_buffer, Uint32 _size);
	void changeText(std::string _text);

private:
	SDL_Renderer *ren;
	SDL_Texture *tex;
	TTF_Font *font;
	SDL_Rect rect;
	SDL_Rect *buttons;
	Uint8 buttonCount;
};