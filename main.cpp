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
 * o Modularize menus
 * o Add Joystick API
 */

int main(int argc, char ** argv)
{
	int returnCode(0);

	/* SDL sub-logger settings */
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	/* SDL Modules initialization */
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_FLAC
			|MIX_INIT_MID
			|MIX_INIT_MOD
			|MIX_INIT_MP3
			|MIX_INIT_OGG
			|MIX_INIT_OPUS);

	/* Audio Resources */
	std::string audioAssets("assets/audio/");
	std::map<std::string, std::string>
		samples{ {"sample.wav", "drum"} },
		musics{ {"music.flac", "ftl"} };

	/* Text displaying resources */
	std::string ttfAssets("assets/fonts/");
	std::set<std::string> fontNames{ "noto", "courier", "arial" };

	try
	{
		/* Acquire info on available hardware */
		Introspection::perform();

		/*
		 * Initialize the current Platform :
		 * - WindowManager : handles Window and Renderer instanciation
		 * - GameControllerManager : manages GameController objects (if any)
		 * - Mixer : handles sound effects
		 */
		std::shared_ptr<Platform> platform(new Platform(
			new WindowManager,
			new GameControllerManager,
			new Mixer(0, audioAssets, samples, musics)));

		/* Instantiate the Main Window and its internal TrueTypeFontManager */
		platform->getWindowManager()->addWindow(
			"mainWindow",
			"SDL Main Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1600, 900,
			Window::RatioType::FIXED_RATIO_STRETCH,
			SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE,
			SDL_RENDERER_ACCELERATED,
			std::make_shared<TrueTypeFontManager>(ttfAssets, fontNames));

		/* Send Hardware Introspection results to logging facility */
		Introspection::log();

		/* Instantiate Main Menu */
		std::shared_ptr<GameContext> menu(Menu::Factory::createMenu(platform));
		/* Instantiate Game Engine with Main Menu as initial context */
		std::shared_ptr<Engine> engine(new Engine(menu));

		/* Start Engine Main Loop */
		engine->run(1.f);
	}
	catch (Exception const & exc)
	{
		EXCEPT(exc);
		returnCode = -1;
	}

	/* SDL modules cleanup */
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return returnCode;
}
