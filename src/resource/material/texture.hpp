#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#endif
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGl/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>
#include <array>
#include <iterator>
#include <string_view>
#include "../../utils/arrayUtils.hpp"

namespace Material {
    static constexpr const std::array<std::string_view, 4> SUPPORTED_FILE_EXTENSIONS = ArrayUtils::makeLiteralArray(
            ".tga",
            ".jpeg",
            ".jpg",
            ".png"
    );

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
        void loadNotFound();
        void unload();
    };
}

#endif // TEXTURE_H