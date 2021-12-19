#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <OpenGl/gl.h>
#include <OpenGL/glu.h>
#endif

#include<SDL2/SDL.h>

#include "../input/inputManager.hpp"

class Window {
protected:
	SDL_Window* window;
	SDL_GLContext glContext;
	int flags;

	virtual bool onCreate() { return true; }
	virtual bool onClose() { return true; }
	virtual void onResize() {}
	virtual void onMouseDownL(float _x, float _y) {}
	virtual void onMouseDownR(float _x, float _y) {}
	virtual void onMouseUpL() {}
	virtual void onMouseUpR() {}
	virtual void onMouseMove(int _x, int _y, int centreX, int centreY) {}
	virtual void onMouseMove(int deltaX, int deltaY) {}
	virtual void onMouseDragL(int _x, int _y, int dx, int dy) {}
	virtual void onMouseDragR(int _x, int _y, int dx, int dy) {}
	virtual void onKeyUp(SDL_Scancode key) {}
	virtual void onKeyDown(SDL_Scancode key) {}
	virtual void onChar(char c) {}

	int processEvent(SDL_Event _event);
private:
	void destroy();
	void resize();

	float normMousePosX(int _x);
	float normMousePosY(int _y);
public:
	int width;
	int height;
	int centreX;
	int centreY;
	int aspect;
	int mouseX;
	int mouseY;
	bool fullscreen;
	float mouseSensitivity;
	bool useInput;

	InputManager *inputManager;

	Window() {}
	Window(const char *title, bool fullscreen, int width, int height);
	virtual ~Window();

	void startFullscreen(int w, int h);
	void stopFullscreen();
};

#endif // WINDOW_H