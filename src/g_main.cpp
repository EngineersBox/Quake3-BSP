#include "g_main.hpp"

#define GL_SILENCE_DEPRECATION

constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;

int main(int, char**) {
	QuakeLogger::initLogger();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error intialising SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	spdlog::info("SDl2 initialised");
    GEngine* engine;
    int exitCode;

    try {
        engine = new GEngine("test", false, WIDTH, HEIGHT);
        spdlog::info("Engine instantiated with {0}x{1} window", WIDTH, HEIGHT);
        exitCode = engine->eventLoop();
        delete engine;
    } catch (char* e) {
        spdlog::error("Error: {0}", e);
        delete engine;
        exitCode = - 1;
    }

	SDL_Quit();

	return exitCode;
}
