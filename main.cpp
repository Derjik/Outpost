#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <VBN/Engine.hpp>
#include <VBN/Introspection.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Logging.hpp>
#include "GlobalHandler.hpp"
#include "Activities/Menu.hpp"
#include "Platform.hpp"

using namespace std;

/*
 * TODO:
 * o Add global and local millisecond-to-gametick ratio settings
 * o Add EngineUpdate to IModel::elapse() signature
 * o Improve text displaying API
 * o Rewrite Introspection API
 * o Modularize menus
 * o Handle item placement
 *     TL T TR
 *     L  C  R
 *     BL B BR
 * o Implement zoom
 */

int main(int argc, char ** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	{
		Introspection::perform();

		std::shared_ptr<Platform> platform(new Platform(
			std::shared_ptr<WindowManager>(new WindowManager),
			std::shared_ptr<GameControllerManager>(new GameControllerManager)));

		platform->getWindowManager()->add(
			"mainWindow",
			"SDL Main Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1366, 768,
			Window::RatioType::FIXED_RATIO_STRETCH,
			SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE,
			SDL_RENDERER_ACCELERATED,
			std::shared_ptr<TrueTypeFontManager>(
				new TrueTypeFontManager({ "courier", "arial" })));

		std::shared_ptr<Menu::Model> menuModel(new Menu::Model);
		std::shared_ptr<GameContext> menu(new GameContext(
			platform,
			menuModel,
			std::shared_ptr<IView>(new Menu::View(menuModel, platform->getWindowManager())),
			std::shared_ptr<IEventHandler>(
				new GlobalHandler(
					platform,
					nullptr,
					std::shared_ptr<IEventHandler>(new Menu::KeyboardEventHandler(platform, menuModel)),
					nullptr,
					nullptr,
					nullptr))
		));

		std::shared_ptr<Engine> engine(new Engine(menu));
		engine->run(0.01);
	}

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}
