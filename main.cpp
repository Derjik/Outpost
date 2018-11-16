#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <VBN/Engine.hpp>
#include <VBN/Introspection.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Logging.hpp>
#include "Activities/Menu.hpp"
#include "Activities/Global.hpp"
#include <VBN/Platform.hpp>

using namespace std;

/*
 * TODO:
 * o Add global and local millisecond-to-gametick ratio settings
 * o Improve text displaying API
 * o Modularize menus
 * o Handle item placement
 *     TL T TR
 *     L  C  R
 *     BL B BR
 * o Add Joystick API
 */

int main(int argc, char ** argv)
{
	int returnCode(0);

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

	try
	{
		Introspection::perform();

		std::shared_ptr<Platform> platform(new Platform(
			new WindowManager,
			new GameControllerManager));

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
				new TrueTypeFontManager({ "noto", "courier", "arial" })));

		std::shared_ptr<Menu::Model> menuModel(new Menu::Model);
		std::shared_ptr<GameContext> menu(new GameContext(
			menuModel,
			std::shared_ptr<IView>(new Global::View(
					platform,
					std::shared_ptr<IView>(new Menu::View(
						platform, menuModel)))),
			std::shared_ptr<IEventHandler>(new Global::EventHandler(
					platform,
					nullptr,
					std::shared_ptr<IEventHandler>(new Menu::KeyboardEventHandler(
						platform, menuModel)),
					nullptr,
					nullptr,
					nullptr))
		));

		std::shared_ptr<Engine> engine(new Engine(menu));
		engine->run(0.01f);
	}
	catch (Exception const & exc)
	{
		EXCEPT(exc);
		returnCode = -1;
	}

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return returnCode;
}
