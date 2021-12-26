#pragma once

#ifndef QUAKE_RASTERTEXT_HPP
#define QUAKE_RASTERTEXT_HPP

#include <spdlog/spdlog.h>
#include <sdl2/sdl.h>
#include <sdl2/sdl_ttf.h>

namespace RasterText {
    inline static unsigned int power_two_floor(unsigned int val) {
        unsigned int power = 2, nextVal = power*2;
        while((nextVal *= 2) <= val)
            power*=2;
        return power*2;
    }

    static void renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize) {
        TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (font == nullptr){
            spdlog::error("Could not find font {0} with size {1}", fontFile, fontSize);
            return;
        }
        SDL_Surface *surface = TTF_RenderText_Blended(font, message.c_str(), color);
        if (surface == nullptr){
            TTF_CloseFont(font);
            spdlog::error("Could not create rasterized surface for text");
            return;
        }
        GLuint texId;

        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        int w = RasterText::power_two_floor(surface->w) * 2;
        int h = RasterText::power_two_floor(surface->h) * 2;

        SDL_Surface *s = SDL_CreateRGBSurface(
            0,
            w,
            h,
            32,
            0x00ff0000,
            0x0000ff00,
            0x000000ff,
            0xff000000
        );
        SDL_BlitSurface(surface, nullptr, s, nullptr);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            w,
            h,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            s->pixels
        );

        glBegin(GL_QUADS);
        glTexCoord2d(0, 1);
        glVertex3f(0, 0, 0);
        glTexCoord2d(1, 1);
        glVertex3f(surface->w, 0, 0);
        glTexCoord2d(1, 0);
        glVertex3f(surface->w, surface->h, 0);
        glTexCoord2d(0, 0);
        glVertex3f(0, surface->h, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        TTF_CloseFont(font);
        SDL_FreeSurface(s);
        SDL_FreeSurface(surface);
        glDeleteTextures(1, &texId);
    }
}

#endif //QUAKE_RASTERTEXT_HPP
