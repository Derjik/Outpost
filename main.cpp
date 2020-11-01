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
#include <VBN/Mixer.hpp>

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
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_FLAC
			|MIX_INIT_MID
			|MIX_INIT_MOD
			|MIX_INIT_MP3
			|MIX_INIT_OGG
			|MIX_INIT_OPUS);

	try
	{
		Introspection::perform();

		std::shared_ptr<Platform> platform(new Platform(
			new WindowManager,
			new GameControllerManager,
			new Mixer(0)));

		platform->getMixer()->loadEffect("sample.wav", "drum");
		platform->getMixer()->loadMusic("music.flac", "ftl");

		platform->getWindowManager()->addWindow(
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

		Introspection::log();

		// M
		std::shared_ptr<Menu::Model> menuModel(
			new Menu::Model);

		// V
		std::shared_ptr<Menu::View> menuView(
			new Menu::View(platform, menuModel));

		// C
		std::shared_ptr<Menu::Controller> menuController(
			new Menu::Controller(platform, menuModel));

		// M + V + C
		std::shared_ptr<GameContext> menu(new GameContext(
				menuModel,
				std::shared_ptr<IView>(new Global::View(
						platform,
						menuView)),
				std::shared_ptr<IEventHandler>(new Global::EventHandler(
						platform,
						nullptr,
						std::shared_ptr<IEventHandler>(menuController),
						std::shared_ptr<IEventHandler>(menuController),
						nullptr,
						nullptr))
		));

		std::shared_ptr<Engine> engine(new Engine(menu));
		engine->run(1.f);
	}
	catch (Exception const & exc)
	{
		EXCEPT(exc);
		returnCode = -1;
	}

	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return returnCode;
}
