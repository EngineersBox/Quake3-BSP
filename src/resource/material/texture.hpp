#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

class Texture {
public:
	int width;
	int height;
	int bitDepth;
	unsigned int id;

	SDL_Surface* image;
	GLubyte* data;

	Texture() {
		this->data = nullptr;
		this->image = nullptr;
	}
	~Texture() { unload(); }

	static Uint32 getPixel(SDL_Surface* surface, int x, int y);
	void load(const char* filename);
	void unload();
};

#endif // TEXTURE_H