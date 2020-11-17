#include <VBN/Platform.hpp>
#include "../GameContext.hpp"
#include "Global.hpp"
#include "TextDebug.hpp"
#include <VBN/WindowManager.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/Logging.hpp>

/* ----------------------------------------------- */
/* ------------------- FACTORY ------------------- */
/* ----------------------------------------------- */
std::shared_ptr<GameContext> TextDebug::Factory::createTextDebug(
	std::shared_ptr<Platform> platform)
{
	std::shared_ptr<TextDebug::Model> model(new TextDebug::Model(platform));

	return Global::Factory::createGlobal(
		platform,
		model,
		std::make_shared<TextDebug::View>(platform, model),
		nullptr,
		std::make_shared<TextDebug::KeyboardEventHandler>(),
		std::make_shared<TextDebug::GameControllerEventHandler>(model),
		nullptr,
		nullptr);
}

/* ----------------------------------------------- */
/* -------------------- MODEL -------------------- */
/* ----------------------------------------------- */

TextDebug::Model::Model(std::shared_ptr<Platform> platform) :
	_platform(platform),
	_fontSize(18),
	_drawSpace({45, 45, 600, 600}),
	aGT(0), bGT(0), xGT(0), yGT(0),
	upGT(0), downGT(0), leftGT(0), rightGT(0)
{}

void TextDebug::Model::elapse(Uint32 const gameTicks,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	GameControllerManager * gameControllerManager(nullptr);
	GameController * gameController(nullptr);
	SDL_GameController * sdlController(nullptr);

	bool a(false), b(false), x(false), y(false),
		down(false), up(false), left(false), right(false);

	Uint32 delay(10), delta(5);

	gameControllerManager = _platform->getGameControllerManager();
	if (gameControllerManager)
		gameController = gameControllerManager->getControllerFromDeviceID(0);
	if (gameController)
		sdlController = gameController->getSDLGameController();
	if (sdlController)
	{
		if (SDL_GameControllerGetButton(sdlController,
			SDL_CONTROLLER_BUTTON_A))
		{
			aGT += gameTicks;
			if (aGT > delay)
			{
				p2down(delta);
				aGT = 0;
			}
		}
		else
			aGT = 0;

		if (SDL_GameControllerGetButton(sdlController,
			SDL_CONTROLLER_BUTTON_B))
		{
			bGT += gameTicks;
			if (bGT > delay)
			{
				p2right(delta);
				bGT = 0;
			}
		}
		else
			bGT = 0;

		if (SDL_GameControllerGetButton(sdlController,
			SDL_CONTROLLER_BUTTON_X))
		{
			xGT += gameTicks;
			if (xGT > delay)
			{
				p2left(delta);
				xGT = 0;
			}
		}
		else
			xGT = 0;

		if (SDL_GameControllerGetButton(sdlController,
			SDL_CONTROLLER_BUTTON_Y))
		{
			yGT += gameTicks;
			if (yGT > delay)
			{
				p2up(delta);
				yGT = 0;
			}
		}
		else
			yGT = 0;

		if (SDL_GameControllerGetButton(sdlController,
			SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			upGT += gameTicks;
			if (upGT > delay)
			{
				p1up(delta);
				upGT = 0;
			}
		}
		else
			upGT = 0;

		if (SDL_GameControllerGetButton(sdlController,
			SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			downGT += gameTicks;
			if (downGT > delay)
			{
				p1down(delta);
				downGT = 0;
			}
		}
		else
			downGT = 0;

		if (SDL_GameControllerGetButton(sdlController,
			SDL_CONTROLLER_BUTTON_DPAD_LEFT))
		{
			leftGT += gameTicks;
			if (leftGT > delay)
			{
				p1left(delta);
				leftGT = 0;
			}
		}
		else
			leftGT = 0;

		if (SDL_GameControllerGetButton(sdlController,
			SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		{
			rightGT += gameTicks;
			if (rightGT > delay)
			{
				p1right(delta);
				rightGT = 0;
			}
		}
		else
			rightGT = 0;
	}
}

unsigned int TextDebug::Model::getFontSize(void)
{
	return _fontSize;
}

SDL_Rect const & TextDebug::Model::getDrawSpace(void)
{
	return _drawSpace;
}

void TextDebug::Model::p1left(int amount)
{
	_drawSpace.x -= amount;
}

void TextDebug::Model::p1right(int amount)
{
	_drawSpace.x += amount;
}

void TextDebug::Model::p1up(int amount)
{
	_drawSpace.y -= amount;
}

void TextDebug::Model::p1down(int amount)
{
	_drawSpace.y += amount;
}

void TextDebug::Model::p2left(int amount)
{
	_drawSpace.w -= amount;
}

void TextDebug::Model::p2right(int amount)
{
	_drawSpace.w += amount;
}

void TextDebug::Model::p2up(int amount)
{
	_drawSpace.h -= amount;
}

void TextDebug::Model::p2down(int amount)
{
	_drawSpace.h += amount;
}

void TextDebug::Model::upFont(int amount)
{
	_fontSize += amount;
}

void TextDebug::Model::downFont(int amount)
{
	_fontSize -= amount;
}

/* ---------------------------------------------- */
/* -------------------- VIEW -------------------- */
/* ---------------------------------------------- */

TextDebug::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{
	Renderer * r(
		_platform->getWindowManager()
		->getWindowByName("mainWindow")
		->getRenderer());

	std::string utfText(u8"Bonjour Gérard");
	r->addUTF8TextTexture(
		"UTF",
		"courier",
		utfText,
		18,
		SDL_Color{ 255, 255, 255, 255 });
}

void TextDebug::View::display(void)
{
	// Acquire Window & Renderer for later use
	Window * mainWindow = _platform->getWindowManager()->getWindowByName("mainWindow");
	Renderer * renderer = mainWindow->getRenderer();
	Texture * utf8text = renderer->getTexture("UTF");

	// Clear screen
	renderer->setDrawColor(0, 0, 32, 255);
	renderer->fill();

	// Print debug text in dynamically-adjusted Drawing Space
	renderer->printText("Lorem ipsum dolor sit amet, consectetur adipiscing "
		"elit, sed do eiusmod tempor incididunt ut labore et dolore magna "
		"aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco "
		"laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure "
		"dolor in reprehenderit in voluptate velit esse cillum dolore eu "
		"fugiat nulla pariatur.\n"
		"Excepteur sint occaecat cupidatat non proident, "
		"sunt in culpa qui officia deserunt mollit anim id est laborum.\n",
		"courier", _model->getFontSize(), { 255, 255, 255, 255 },
		_model->getDrawSpace());

	mainWindow->getRenderer()->copy(
		"UTF", "",
		{ 100, 600, utf8text->getWidth(), utf8text->getHeight()});

}

/* ---------------------------------------------------- */
/* -------------------- CONTROLLER -------------------- */
/* ---------------------------------------------------- */

void TextDebug::KeyboardEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					engineUpdate->popGameContext();
				break;
			}
		break;
	}
}

TextDebug::GameControllerEventHandler::GameControllerEventHandler(
	std::shared_ptr<Model> model) : _model(model)
{}

void TextDebug::GameControllerEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	switch (event.type)
	{
		case SDL_CONTROLLERBUTTONDOWN:
			switch (event.cbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					_model->p1down(10);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					_model->p1up(10);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
					_model->p1left(10);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
					_model->p1right(10);
				break;

				case SDL_CONTROLLER_BUTTON_A:
					_model->p2down(10);
				break;
				case SDL_CONTROLLER_BUTTON_Y:
					_model->p2up(10);
				break;
				case SDL_CONTROLLER_BUTTON_X:
					_model->p2left(10);
				break;
				case SDL_CONTROLLER_BUTTON_B:
					_model->p2right(10);
				break;

				case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
					_model->downFont(1);
				break;
				case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
					_model->upFont(1);
				break;
			}
		break;
	}
}
