#include "Pause.hpp"
#include "Debug.hpp"
#include "Global.hpp"
#include <VBN/Platform.hpp>
#include "../GameContext.hpp"
#include <VBN/WindowManager.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/Logging.hpp>
#include <sstream>
#include <cmath>

#define XBOX_CONTROLLER_TEXTURE_PATH "xbox_controller.png"
#define XBOX_CONTROLLER_TEXTURE_NAME "XBox360Controller"

/* ----------------------------------------------- */
/* ------------------- FACTORY ------------------- */
/* ----------------------------------------------- */
std::shared_ptr<GameContext> Debug::Factory::createDebug(
	std::shared_ptr<Platform> platform)
{
	std::shared_ptr<Debug::Model> model(
		std::make_shared<Debug::Model>(platform));

	return std::make_shared<GameContext>(
		model,
		std::make_shared<Global::View>(
			platform,
			std::make_shared<Debug::View>(platform, model)),
		std::make_shared<Global::EventHandler>(
				platform,
				nullptr,
				std::make_shared<Debug::KeyboardEventHandler>(),
				std::make_shared<Debug::GameControllerEventHandler>(
					platform, model),
				nullptr,
				nullptr));
}

/* ----------------------------------------------- */
/* -------------------- MODEL -------------------- */
/* ----------------------------------------------- */

Debug::Model::Model(std::shared_ptr<Platform> platform) : _platform(platform)
{}

void Debug::Model::elapse(Uint32 const gameTicks,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	GameControllerManager * gameControllerManager(nullptr);
	GameController * gameController(nullptr);
	SDL_GameController * sdlController(nullptr);

	gameControllerManager = _platform->getGameControllerManager();
	if(gameControllerManager)
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
		_buttons["LSHOULDER"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		_buttons["RSHOULDER"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

		_buttons["START"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_START);
		_buttons["BACK"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_BACK);
		_buttons["GUIDE"] = SDL_GameControllerGetButton(sdlController, SDL_CONTROLLER_BUTTON_GUIDE);

		_leftPole.first = fmin(sqrt(pow(_leftJoystick.first, 2.) + pow(_leftJoystick.second, 2.)), 120.f);
		_leftPole.second = atan2((double)(_leftJoystick.second), (double)(_leftJoystick.first));// *(180.f / M_PI);
		//VERBOSE(SDL_LOG_CATEGORY_APPLICATION, "r : %f - theta : %f", _leftPole.first, _leftPole.second);
	}
}

std::pair<Sint16, Sint16> Debug::Model::getLeftJoystick(void)
{
	return _leftJoystick;
}

std::pair<double, double> Debug::Model::getLeftPole(void)
{
	return _leftPole;
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

std::map<std::string, bool> const & Debug::Model::getButtons(void) const
{
	return _buttons;
}

/* ---------------------------------------------- */
/* -------------------- VIEW -------------------- */
/* ---------------------------------------------- */

Debug::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{
	Window * mainWindow(_platform->getWindowManager()->getWindowByName("mainWindow"));
	Renderer * renderer(nullptr);
	Texture * xboxController(nullptr);
	if (mainWindow)
		renderer = mainWindow->getRenderer();

	if (renderer && !renderer->getTexture(XBOX_CONTROLLER_TEXTURE_NAME))
	{
		renderer->addImageTexture(
			XBOX_CONTROLLER_TEXTURE_NAME,
			XBOX_CONTROLLER_TEXTURE_PATH);

		Texture * controller = renderer->getTexture(XBOX_CONTROLLER_TEXTURE_NAME);
		controller->addClip("A", { 878, 55, 78, 78 });
		controller->addClip("B", { 1038, 55, 78, 78 });
		controller->addClip("X", { 798, 55, 78, 78 });
		controller->addClip("Y", { 958, 55, 78, 78 });
		controller->addClip("BACK", { 491, 44, 81, 77 });
		controller->addClip("START", { 712, 44, 84, 77 });
		controller->addClip("GUIDE", { 574, 24, 136, 138 });
		controller->addClip("DPAD", { 154, 1, 184, 186 });
	}
}

void Debug::View::display(void)
{
	Window * mainWindow = _platform->getWindowManager()->getWindowByName("mainWindow");
	Renderer * renderer = mainWindow->getRenderer();

	renderer->setDrawColor(0, 0, 32, 255);
	renderer->fill();

	renderer->printText("DEBUG", "courier", 12, { 255, 255, 255, 255 }, {10, 10, 100, 22});



	// Acquire Joysticks & Triggers data
	Sint16 leftx(_model->getLeftJoystick().first),
		lefty(_model->getLeftJoystick().second),
		rightx(_model->getRightJoystick().first),
		righty(_model->getRightJoystick().second),
		ltrigger(_model->getTriggers().first),
		rtrigger(_model->getTriggers().second);

	std::stringstream controllerStatus;
	controllerStatus
		<< "Left Joystick : " << leftx << "," << lefty << '\n'
		<< "Right Joystick : " << rightx << "," << righty << '\n'
		<< "Triggers Status : " << ltrigger << "," << rtrigger << '\n'
		<< "A : " << _model->getButton("A") << "\n"
		<< "B : " << _model->getButton("B") << "\n"
		<< "X : " << _model->getButton("X") << "\n"
		<< "Y : " << _model->getButton("Y");

	renderer->printText(controllerStatus.str(),
		"courier", 16, { 255, 255, 255, 255 },
		{ 920, 20, 400, 300 });

	// Draw left & right joystick crosshairs
	renderer->setDrawColor(0, 194, 255, 255);
	// LEFT
	//SDL_Rect lxr{ 200, 199, leftx, 3 };
	//SDL_Rect lyr{ 199, 200, 3, lefty };
	//renderer->fillRect(lxr);
	//renderer->fillRect(lyr);
	//renderer->drawLine(195 + leftx, 200 + lefty, 205 + leftx, 200 + lefty);
	//renderer->drawLine(200 + leftx, 195 + lefty, 200 + leftx, 205 + lefty);
	std::pair<double, double> pole = _model->getLeftPole();
	renderer->drawLine(200, 200, 200 + pole.first * cos(pole.second), 200 + pole.first * sin(pole.second));
	// RIGHT
	SDL_Rect rxr{ 600, 199, rightx, 3 };
	SDL_Rect ryr{ 599, 200, 3, righty };
	renderer->fillRect(rxr);
	renderer->fillRect(ryr);
	renderer->drawLine(595 + rightx, 200 + righty, 605 + rightx, 200 + righty);
	renderer->drawLine(600 + rightx, 195 + righty, 600 + rightx, 205 + righty);

	// Draw left & right trigger levels
	SDL_Rect leftr{ 200, 400, 3, ltrigger };
	SDL_Rect rightr{ 600, 400, 3, rtrigger };
	renderer->fillRect(leftr);
	renderer->fillRect(rightr);

	int
		buttonsXOffset(500), buttonsYOffset(150),
		dpadXOffset(250), dpadYOffset(150);

	Texture * controller = renderer->getTexture(XBOX_CONTROLLER_TEXTURE_NAME);
	SDL_Rect
		aDest{ buttonsXOffset + 300, buttonsYOffset + 200, 78, 78 },
		bDest{ buttonsXOffset + 378, buttonsYOffset + 122, 78, 78 },
		xDest{ buttonsXOffset + 222, buttonsYOffset + 122, 78, 78 },
		yDest{ buttonsXOffset + 300, buttonsYOffset + 44, 78, 78 },
		dpadDest{ dpadXOffset, dpadYOffset, 184, 186 };

	if (_model->getButton("A"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "A", aDest);

	if (_model->getButton("B"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "B", bDest);

	if (_model->getButton("X"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "X", xDest);

	if (_model->getButton("Y"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "Y", yDest);

	if (_model->getButton("UP") ||
		_model->getButton("DOWN") ||
		_model->getButton("LEFT") ||
		_model->getButton("RIGHT"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "DPAD", dpadDest);

	renderer->setDrawColor(200, 20, 20, 255);
	if (_model->getButton("UP"))
		renderer->drawRect({ dpadXOffset + 49, dpadYOffset, 86, 66 });
}

/* ---------------------------------------------------- */
/* -------------------- CONTROLLER -------------------- */
/* ---------------------------------------------------- */

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
						nullptr,
						std::shared_ptr<IView>(new Global::View(
							_platform,
							std::shared_ptr<IView>(new Pause::View(
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