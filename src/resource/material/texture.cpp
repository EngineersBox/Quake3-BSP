#include "texture.hpp"

#define RGB_BIT_DEPTH 24
#define RGBA_BIT_DEPTH 32
#define PIXEL_BYTE_SIZE 8

void Material::Texture::load(const char* filename) {
	this->image = IMG_Load(filename);
	if (!this->image) {
		spdlog::error("Could not load image: {0}", SDL_GetError());
		return;
	}

	this->width = this->image->w;
	this->height = this->image->h;
	this->bitDepth = this->image->format->BitsPerPixel;

	int bytesPerPixel = this->bitDepth / PIXEL_BYTE_SIZE;
	this->data = new GLubyte[this->width * this->height * bytesPerPixel];

	int pos = 0;
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			Uint8 r, g, b, a;
			Uint32 pixel = Texture::getPixel(this->image, x, y);
			if (this->bitDepth == RGB_BIT_DEPTH) {
				SDL_GetRGB(pixel, this->image->format, &r, &g, &b);
			} else {
				SDL_GetRGBA(pixel, this->image->format, &r, &g, &b, &a);
			}

			this->data[pos] = r;
			this->data[pos+ 1] = g;
			this->data[pos + 2] = b;
			if (this->bitDepth == RGBA_BIT_DEPTH) {
				this->data[pos + 3] = a;
			}
			pos += bytesPerPixel;
		}
	}
}

void Material::Texture::loadNotFound() {
    this->width = 128;
    this->height = 128;
    this->bitDepth = RGB_BIT_DEPTH;

    int bytesPerPixel = this->bitDepth / PIXEL_BYTE_SIZE;

    this->image = SDL_CreateRGBSurface(
        0,
        this->width,
        this->height,
        this->bitDepth,
        0,
        0,
        0,
        0
    );
    if (this->image == nullptr) {
        spdlog::error("Could not create RGB surface: {0}", SDL_GetError());
        return;
    }
    Uint32 pink = SDL_MapRGB(this->image->format, 255, 0, 255);
    SDL_Rect rect{
        .x = 0,
        .y = 0,
        .w = this->width / 2,
        .h = this->height / 2
    };
    this->data = new GLubyte[this->width * this->height * bytesPerPixel];
    SDL_FillRect(this->image, &rect, pink);
    rect = {
        .x = this->width / 2,
        .y = this->height / 2,
        .w = this->width,
        .h = this->height
    };
    SDL_FillRect(this->image, &rect, pink);
    int pos = 0;
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            Uint8 r, g, b;
            Uint32 pixel = Texture::getPixel(this->image, x, y);
            SDL_GetRGB(pixel, this->image->format, &r, &g, &b);
            this->data[pos] = r;
            this->data[pos + 1] = g;
            this->data[pos + 2] = b;
            pos += bytesPerPixel;
        }
    }
}

void Material::Texture::unload() {
	glDeleteTextures(1, &this->id);
	if (this->image) {
		SDL_FreeSurface(this->image);
	}
    delete this->data;
	this->image = nullptr;
	this->data = nullptr;
}

Uint32 Material::Texture::getPixel(SDL_Surface* surface, int x, int y) {
	int bytesPerPixel = surface->format->BytesPerPixel;
	Uint8* pixel = (Uint8*)surface->pixels + y * surface->pitch + x * bytesPerPixel;
	switch (bytesPerPixel) {
	case 1:
		return *pixel;
	case 2:
		return *(Uint16*)pixel;
	case 3:
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
        #else
            return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
        #endif
	case 4:
		return *(Uint32*)pixel;
	default:
		return 0;
	}
}