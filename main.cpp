#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <VBN/Engine.hpp>
#include <VBN/Introspection.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/TrueTypeFontManager.hpp>
#include "Menu.hpp"
#include "Platform.hpp"

using namespace std;

/*
 * TODO:
 * v Add a GlobalHandler (with default: handleX)
 * o Modularize menus
 * o Handle item placement
 *     TL T TR
 *     L  C  R
 *     BL B BR
 * o Implement zoom
 * o Implement layers (shade and stack contexts when drawing)
 * o (Maybe) improve text displaying (handle newline ?)
 */

int main(int argc, char ** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	{
		Introspection::perform();

		std::shared_ptr<TrueTypeFontManager> ttfManager(
			new TrueTypeFontManager({ "courier", "arial" }));

		std::shared_ptr<WindowManager> windowManager(
			new WindowManager());

		std::shared_ptr<GameControllerManager> gameControllerManager(
			new GameControllerManager());

		std::shared_ptr<Platform> platform(new Platform(
			windowManager, gameControllerManager));

		windowManager->add("mainWindow", "SDL Main Window",
							SDL_WINDOWPOS_CENTERED,
							SDL_WINDOWPOS_CENTERED,
							1366, 768,
							RatioType::FIXED_RATIO_STRETCH,
							SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE,
							SDL_RENDERER_ACCELERATED,
							ttfManager);

		std::shared_ptr<Menu::Model> menuModel(new Menu::Model);
		std::shared_ptr<GameContext> menu(new GameContext(
			platform,
			menuModel,
			std::shared_ptr<IView>(new Menu::View(menuModel, windowManager)),
			std::shared_ptr<IEventHandler>(new Menu::KeyboardEventHandler(platform, menuModel)),
			nullptr,
			nullptr,
			nullptr));

		std::shared_ptr<Engine> engine(new Engine(menu));
		engine->run();
	}

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}
