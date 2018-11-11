#include "Pause.hpp"
#include "Debug.hpp"
#include "Global.hpp"
#include <VBN/Platform.hpp>
#include "../GameContext.hpp"
#include <VBN/WindowManager.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/Logging.hpp>

#define XBOX_CONTROLLER_TEXTURE_PATH "xbox_controller.png"
#define XBOX_CONTROLLER_TEXTURE_NAME "XBox360Controller"

Debug::Model::Model(std::shared_ptr<Platform> platform) : _platform(platform)
{}

void Debug::Model::elapse(Uint32 const gameTicks,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	std::shared_ptr<GameControllerManager> gameControllerManager =
		_platform->getGameControllerManager();

	GameController * gameController(nullptr);
	SDL_GameController * sdlController(nullptr);

	gameController = gameControllerManager->getControllerFromDeviceID(0);
	if(gameController)
		sdlController = gameController->getSDLGameController();

	if (sdlController)
	{
		_leftJoystick.first =
			SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_LEFTX) / 256;

		_leftJoystick.second =
			SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_LEFTY) / 256;

		_rightJoystick.first =
			SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_RIGHTX) / 256;

		_rightJoystick.second =
			SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_RIGHTY) / 256;

		_triggers.first =
			SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 256;

		_triggers.second =
			SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 256;

		_buttons["A"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_A);
		_buttons["B"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_B);
		_buttons["X"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_X);
		_buttons["Y"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_Y);

		_buttons["UP"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_DPAD_UP);
		_buttons["DOWN"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		_buttons["LEFT"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		_buttons["RIGHT"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

		_buttons["LSTICK"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_LEFTSTICK);
		_buttons["RSTICK"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
		_buttons["LSHOULDER"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_LEFTSTICK);
		_buttons["RSHOULDER"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_RIGHTSTICK);

		_buttons["START"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_START);
		_buttons["BACK"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_BACK);
		_buttons["GUIDE"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_GUIDE);
	}
}

std::pair<Sint16, Sint16> Debug::Model::getLeftJoystick(void)
{
	return _leftJoystick;
}

std::pair<Sint16, Sint16> Debug::Model::getRightJoystick(void)
{
	return _rightJoystick;
}

std::pair<Sint16, Sint16> Debug::Model::getTriggers(void)
{
	return _triggers;
}

bool Debug::Model::getButton(std::string const & button)
{
	return _buttons[button];
}

Debug::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{
	Window * mainWindow(_platform->getWindowManager()->getWindowByName("mainWindow"));

	if (!mainWindow->hasTexture(XBOX_CONTROLLER_TEXTURE_NAME))
	{
		mainWindow->addImageTexture(
			XBOX_CONTROLLER_TEXTURE_NAME,
			XBOX_CONTROLLER_TEXTURE_PATH);

		Texture & controller = mainWindow->getTexture(XBOX_CONTROLLER_TEXTURE_NAME);
		controller.addClip("A", { 878, 55, 78, 78 });
		controller.addClip("B", { 1038, 55, 78, 78 });
		controller.addClip("X", { 798, 55, 78, 78 });
		controller.addClip("Y", { 958, 55, 78, 78 });
		controller.addClip("BACK", { 491, 44, 81, 77 });
		controller.addClip("START", { 712, 44, 84, 77 });
		controller.addClip("GUIDE", { 574, 24, 136, 138 });
		controller.addClip("DPAD", { 154, 1, 184, 186 });
	}
}

void Debug::View::display(void)
{
	Window * mainWindow = _platform->getWindowManager()->getWindowByName("mainWindow");
	SDL_Renderer * renderer(mainWindow->getRenderer());

	SDL_SetRenderDrawColor(renderer, 0, 0, 32, 255);
	SDL_RenderClear(renderer);

	Sint16 leftx(_model->getLeftJoystick().first),
		lefty(_model->getLeftJoystick().second),
		rightx(_model->getRightJoystick().first),
		righty(_model->getRightJoystick().second),
		ltrigger(_model->getTriggers().first),
		rtrigger(_model->getTriggers().second);

	mainWindow->printText("DEBUG", "courier", 12, { 255, 255, 255, 255 }, {10, 10, 100, 22});

	SDL_Rect lxr{ 200, 199, leftx, 3 };
	SDL_Rect lyr{ 199, 200, 3, lefty };
	SDL_SetRenderDrawColor(renderer, 0, 194, 255, 255);
	SDL_RenderFillRect(renderer, &lxr);
	SDL_RenderFillRect(renderer, &lyr);
	SDL_RenderDrawLine(renderer, 195 + leftx, 200 + lefty, 205 + leftx, 200 + lefty);
	SDL_RenderDrawLine(renderer, 200 + leftx, 195 + lefty, 200 + leftx, 205 + lefty);

	SDL_Rect rxr{ 600, 199, rightx, 3 };
	SDL_Rect ryr{ 599, 200, 3, righty };
	SDL_RenderFillRect(renderer, &rxr);
	SDL_RenderFillRect(renderer, &ryr);
	SDL_RenderDrawLine(renderer, 595 + rightx, 200 + righty, 605 + rightx, 200 + righty);
	SDL_RenderDrawLine(renderer, 600 + rightx, 195 + righty, 600 + rightx, 205 + righty);

	SDL_Rect leftr{ 200, 400, 3, ltrigger };
	SDL_Rect rightr{ 600, 400, 3, rtrigger };
	SDL_RenderFillRect(renderer, &leftr);
	SDL_RenderFillRect(renderer, &rightr);

	int
		buttonsXOffset(500), buttonsYOffset(150),
		dpadXOffset(250), dpadYOffset(150);

	Texture & controller = mainWindow->getTexture(XBOX_CONTROLLER_TEXTURE_NAME);
	SDL_Rect
		aDest{ buttonsXOffset + 300, buttonsYOffset + 200, 78, 78 },
		bDest{ buttonsXOffset + 378, buttonsYOffset + 122, 78, 78 },
		xDest{ buttonsXOffset + 222, buttonsYOffset + 122, 78, 78 },
		yDest{ buttonsXOffset + 300, buttonsYOffset + 44, 78, 78 };

	SDL_Rect dpadDest{ dpadXOffset, dpadYOffset, 184, 186 };

	SDL_Rect upRect{ dpadXOffset + 49, dpadYOffset, 86, 66 };

	if(_model->getButton("A"))
		SDL_RenderCopy(renderer,
			mainWindow->getTexture(XBOX_CONTROLLER_TEXTURE_NAME).getSDLTexture(),
			controller.getClip("A"), &aDest);

	if (_model->getButton("B"))
		SDL_RenderCopy(renderer,
			mainWindow->getTexture(XBOX_CONTROLLER_TEXTURE_NAME).getSDLTexture(),
			controller.getClip("B"), &bDest);

	if (_model->getButton("X"))
		SDL_RenderCopy(renderer,
			mainWindow->getTexture(XBOX_CONTROLLER_TEXTURE_NAME).getSDLTexture(),
			controller.getClip("X"), &xDest);

	if (_model->getButton("Y"))
		SDL_RenderCopy(renderer,
			mainWindow->getTexture(XBOX_CONTROLLER_TEXTURE_NAME).getSDLTexture(),
			controller.getClip("Y"), &yDest);

	if (_model->getButton("UP") ||
		_model->getButton("DOWN") ||
		_model->getButton("LEFT") ||
		_model->getButton("RIGHT"))
		SDL_RenderCopy(renderer,
			mainWindow->getTexture(XBOX_CONTROLLER_TEXTURE_NAME).getSDLTexture(),
			controller.getClip("DPAD"), &dpadDest);

	SDL_SetRenderDrawColor(renderer, 200, 20, 20, 255);
	if(_model->getButton("UP"))
		SDL_RenderDrawRect(renderer, &upRect);
}

void Debug::KeyboardEventHandler::handleEvent(SDL_Event const & event,
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

Debug::GameControllerEventHandler::GameControllerEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{}

Debug::GameControllerEventHandler::~GameControllerEventHandler(void)
{}

void Debug::GameControllerEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> update)
{
	GameController * controller(nullptr);
	SDL_Joystick * joystick(nullptr);
	SDL_Haptic * haptic(nullptr);

	controller = _platform
		->getGameControllerManager()
		->getControllerFromDeviceID(0);

	if (controller)
	{
		joystick = controller->getSDLJoystick();
		haptic = controller->getSDLHaptic();
	}

	switch (event.type)
	{
		case SDL_CONTROLLERBUTTONDOWN:
			switch (event.cbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_START:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button START pressed on instance @%d",
						event.cbutton.which);
					update->pushGameContext(
						std::shared_ptr<IGameContext>(new GameContext(
						_platform,
						nullptr,
						std::shared_ptr<IView>(new Pause::View(
							_platform,
							std::shared_ptr<IView>(new Global::View(
								_platform,
								std::shared_ptr<IView>(new View(
									_platform, _model)))))),
						std::shared_ptr<IEventHandler>(
							new Global::EventHandler(
								_platform,
								nullptr,
								nullptr,
								std::shared_ptr<IEventHandler>(new Pause::GameControllerEventHandler),
								nullptr,
								nullptr)))));
				break;
				case SDL_CONTROLLER_BUTTON_BACK:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button BACK pressed on instance @%d",
						event.cbutton.which);
					switch (SDL_JoystickCurrentPowerLevel(joystick))
					{
						case SDL_JOYSTICK_POWER_WIRED:
							DEBUG(SDL_LOG_CATEGORY_APPLICATION, "Battery status : Wired");
						break;
						case SDL_JOYSTICK_POWER_MAX:
							DEBUG(SDL_LOG_CATEGORY_APPLICATION, "Battery status : Max");
						break;
						case SDL_JOYSTICK_POWER_FULL:
							DEBUG(SDL_LOG_CATEGORY_APPLICATION, "Battery status : Full");
						break;
						case SDL_JOYSTICK_POWER_MEDIUM:
							DEBUG(SDL_LOG_CATEGORY_APPLICATION, "Battery status : Medium");
						break;
						case SDL_JOYSTICK_POWER_LOW:
							DEBUG(SDL_LOG_CATEGORY_APPLICATION, "Battery status : Low");
						break;
						case SDL_JOYSTICK_POWER_EMPTY:
							DEBUG(SDL_LOG_CATEGORY_APPLICATION, "Battery status : Empty");
						break;
						case SDL_JOYSTICK_POWER_UNKNOWN:
							DEBUG(SDL_LOG_CATEGORY_APPLICATION, "Battery status : Unknown");
						break;
					}
				break;
				case SDL_CONTROLLER_BUTTON_A:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button A pressed on instance @%d",
						event.cbutton.which);
					if (controller)
						controller->rumble(1.f, 2500);
				break;
				case SDL_CONTROLLER_BUTTON_B:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button B pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_X:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button X pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_Y:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button Y pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button UP pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button DOWN pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button LEFT pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button RIGHT pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_GUIDE:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button GUIDE pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button LEFTSHOULDER pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button RIGHTSHOULDER pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_LEFTSTICK:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button LEFTSTICK pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button RIGHTSTICK pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_MAX:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button MAX pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_INVALID:
					DEBUG(SDL_LOG_CATEGORY_APPLICATION,
						"Button INVALID pressed on instance @%d",
						event.cbutton.which);
				break;
			}
		break;
		case SDL_CONTROLLERBUTTONUP:
		break;
	}
}