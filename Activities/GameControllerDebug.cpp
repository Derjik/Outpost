#include "Pause.hpp"
#include "GameControllerDebug.hpp"
#include "Global.hpp"
#include <VBN/Platform.hpp>
#include "../GameContext.hpp"
#include <VBN/WindowManager.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/Logging.hpp>
#include <sstream>
#include <cmath>

#define XBOX_CONTROLLER_TEXTURE_PATH "assets/textures/xbox_px.png"
#define XBOX_CONTROLLER_TEXTURE_NAME "XBox360Controller"

/* ----------------------------------------------- */
/* ------------------- FACTORY ------------------- */
/* ----------------------------------------------- */
std::shared_ptr<GameContext> GameControllerDebug::Factory::createGameControllerDebug(
	std::shared_ptr<Platform> platform)
{
	std::shared_ptr<GameControllerDebug::Model> model(
		std::make_shared<GameControllerDebug::Model>(platform));

	return Global::Factory::createGlobal(
		platform,
		model,
		std::make_shared<GameControllerDebug::View>(platform, model),
		nullptr,
		std::make_shared<GameControllerDebug::KeyboardEventHandler>(),
		std::make_shared<GameControllerDebug::GameControllerEventHandler>(
			platform, model),
		nullptr,
		nullptr);
}

/* ----------------------------------------------- */
/* -------------------- MODEL -------------------- */
/* ----------------------------------------------- */

GameControllerDebug::Model::Model(std::shared_ptr<Platform> platform) : _platform(platform)
{}

void GameControllerDebug::Model::elapse(Uint32 const gameTicks,
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

		_leftPole.first = fmin(sqrt(pow(_leftJoystick.first, 2.) + pow(_leftJoystick.second, 2.)), 120.f)/10;
		_leftPole.second = atan2((double)(_leftJoystick.second), (double)(_leftJoystick.first));// *(180.f / M_PI);
		_rightPole.first = fmin(sqrt(pow(_rightJoystick.first, 2.) + pow(_rightJoystick.second, 2.)), 120.f)/10;
		_rightPole.second = atan2((double)(_rightJoystick.second), (double)(_rightJoystick.first));// *(180.f / M_PI);
		VERBOSE(SDL_LOG_CATEGORY_APPLICATION, "r : %f - theta : %f", _leftPole.first, _leftPole.second);
	}
}

std::pair<Sint16, Sint16> GameControllerDebug::Model::getLeftJoystick(void)
{
	return _leftJoystick;
}

std::pair<double, double> GameControllerDebug::Model::getLeftPole(void)
{
	return _leftPole;
}

std::pair<Sint16, Sint16> GameControllerDebug::Model::getRightJoystick(void)
{
	return _rightJoystick;
}

std::pair<double, double> GameControllerDebug::Model::getRightPole(void)
{
	return _rightPole;
}

std::pair<Sint16, Sint16> GameControllerDebug::Model::getTriggers(void)
{
	return _triggers;
}

bool GameControllerDebug::Model::getButton(std::string const & button)
{
	return _buttons[button];
}

std::map<std::string, bool> const & GameControllerDebug::Model::getButtons(void) const
{
	return _buttons;
}

/* ---------------------------------------------- */
/* -------------------- VIEW -------------------- */
/* ---------------------------------------------- */

GameControllerDebug::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{
	Window * mainWindow(_platform->getWindowManager()->getWindowByName("mainWindow"));
	Renderer * renderer(nullptr);
	if (mainWindow)
		renderer = mainWindow->getRenderer();

	if (renderer && !renderer->getTexture(XBOX_CONTROLLER_TEXTURE_NAME))
	{
		renderer->addImageTexture(
			XBOX_CONTROLLER_TEXTURE_NAME,
			XBOX_CONTROLLER_TEXTURE_PATH);

		Texture * controller = renderer->getTexture(XBOX_CONTROLLER_TEXTURE_NAME);
		controller->addClip("A_off", { 0, 0, 32, 32 });
		controller->addClip("A_on", { 32, 0, 32, 32 });
		controller->addClip("B_off", { 64, 0, 32, 32 });
		controller->addClip("B_on", { 96, 0, 32, 32 });
		controller->addClip("X_off", { 128, 0, 32, 32 });
		controller->addClip("X_on", { 160, 0, 32, 32 });
		controller->addClip("Y_off", { 192, 0, 32, 32 });
		controller->addClip("Y_on", { 224, 0, 32, 32 });
		controller->addClip("LEFT_off", { 0, 32, 32, 32 });
		controller->addClip("LEFT_on", { 32, 32, 32, 32 });
		controller->addClip("RIGHT_off", { 64, 32, 32, 32 });
		controller->addClip("RIGHT_on", { 96, 32, 32, 32 });
		controller->addClip("UP_off", { 128, 32, 32, 32 });
		controller->addClip("UP_on", { 160, 32, 32, 32 });
		controller->addClip("DOWN_off", { 192, 32, 32, 32 });
		controller->addClip("DOWN_on", { 224, 32, 32, 32 });
		controller->addClip("BACK_off", { 0, 64, 32, 32 });
		controller->addClip("BACK_on", { 32, 64, 32, 32 });
		controller->addClip("START_off", { 64, 64, 32, 32 });
		controller->addClip("START_on", { 96, 64, 32, 32 });
		controller->addClip("RSH_off", { 128, 64, 32, 32 });
		controller->addClip("RSH_on", { 160, 64, 32, 32 });
		controller->addClip("LSH_off", { 192, 64, 32, 32 });
		controller->addClip("LSH_on", { 224, 64, 32, 32 });
		controller->addClip("LTR_0", { 0, 96, 32, 32 });
		controller->addClip("LTR_1", { 32, 96, 32, 32 });
		controller->addClip("LTR_2", { 64, 96, 32, 32 });
		controller->addClip("LTR_3", { 96, 96, 32, 32 });
		controller->addClip("LTR_4", { 128, 96, 32, 32 });
		controller->addClip("LTR_5", { 160, 96, 32, 32 });
		controller->addClip("RTR_0", { 192, 96, 32, 32 });
		controller->addClip("RTR_1", { 224, 96, 32, 32 });
		controller->addClip("RTR_2", { 0, 128, 32, 32 });
		controller->addClip("RTR_3", { 32, 128, 32, 32 });
		controller->addClip("RTR_4", { 64, 128, 32, 32 });
		controller->addClip("RTR_5", { 96, 128, 32, 32 });
		controller->addClip("GUIDE_off", { 128, 128, 32, 32 });
		controller->addClip("GUIDE_on", { 160, 128, 32, 32 });
		controller->addClip("JOY_off", { 0, 192, 32, 32 });
		controller->addClip("JOY_on", { 32, 192, 32, 32 });
	}
}

void GameControllerDebug::View::display(void)
{
	Window * mainWindow = _platform->getWindowManager()->getWindowByName("mainWindow");
	Renderer * renderer = mainWindow->getRenderer();

	renderer->setDrawColor(0, 0, 0, 255);
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
	std::pair<double, double> leftPole = _model->getLeftPole();
	//renderer->drawLine(232, 600, 232 + leftPole.first * cos(leftPole.second), 600 + leftPole.first * sin(leftPole.second));
	// RIGHT
	std::pair<double, double> rightPole = _model->getRightPole();
	//renderer->drawLine(432, 600, 432 + rightPole.first * cos(rightPole.second), 600 + rightPole.first * sin(rightPole.second));

	// Draw left & right trigger levels
	//SDL_Rect leftr{ 200, 400, 3, ltrigger };
	//SDL_Rect rightr{ 600, 400, 3, rtrigger };
	//renderer->fillRect(leftr);
	//renderer->fillRect(rightr);

	int dpadX(100), dpadY(400);
	SDL_Rect dDest{ dpadX, dpadY + 32, 64, 64 };
	SDL_Rect rDest{ dpadX + 32, dpadY, 64, 64 };
	SDL_Rect lDest{ dpadX - 32, dpadY, 64, 64 };
	SDL_Rect uDest{ dpadX, dpadY - 32, 64, 64 };
	SDL_Rect backDest{ dpadX + 96, dpadY, 64, 64 };

	int buttonsX(dpadX + 8*32), buttonsY(400);
	SDL_Rect aDest{ buttonsX, buttonsY + 32, 64, 64 };
	SDL_Rect bDest{ buttonsX + 32, buttonsY, 64, 64 };
	SDL_Rect xDest{ buttonsX - 32, buttonsY, 64, 64 };
	SDL_Rect yDest{ buttonsX, buttonsY - 32, 64, 64 };
	SDL_Rect startDest{ buttonsX - 96, buttonsY, 64, 64 };

	SDL_Rect guideDest{ dpadX + 128, dpadY + 32, 64, 64 };

	SDL_Rect lshDest{ dpadX, dpadY - 128, 64, 64};
	SDL_Rect rshDest{ buttonsX, buttonsY - 128, 64, 64};

	SDL_Rect ltDest{ dpadX, dpadY - 80, 64, 64};
	SDL_Rect rtDest{ buttonsX, buttonsY - 80, 64, 64};

	SDL_Rect leftJoyDest{ dpadX + 64 + leftPole.first * cos(leftPole.second), dpadY + 96 + leftPole.first * sin(leftPole.second), 64, 64};
	SDL_Rect rightJoyDest{ buttonsX - 64 + rightPole.first * cos(rightPole.second), buttonsY + 96 + rightPole.first * sin(rightPole.second), 64, 64};

	/*
	for (int i(0); i < 12; ++i)
	{
		for (int j(0); j < 8; ++j)
		{
			renderer->drawRect({ dpadX + (i - 2) * 32, dpadY + (j - 5) * 32, 64, 64 });
		}
	}
	*/

	if (_model->getButton("A"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "A_on", aDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "A_off", aDest);

	if (_model->getButton("B"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "B_on", bDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "B_off", bDest);

	if (_model->getButton("X"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "X_on", xDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "X_off", xDest);

	if (_model->getButton("Y"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "Y_on", yDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "Y_off", yDest);

	if (_model->getButton("DOWN"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "DOWN_on", dDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "DOWN_off", dDest);

	if (_model->getButton("RIGHT"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RIGHT_on", rDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RIGHT_off", rDest);

	if (_model->getButton("LEFT"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LEFT_on", lDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LEFT_off", lDest);

	if (_model->getButton("UP"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "UP_on", uDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "UP_off", uDest);

	if (_model->getButton("BACK"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "BACK_on", backDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "BACK_off", backDest);

	if (_model->getButton("START"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "START_on", startDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "START_off", startDest);

	if (_model->getButton("GUIDE"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "GUIDE_on", guideDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "GUIDE_off", guideDest);

	if (_model->getButton("LSHOULDER"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LSH_on", lshDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LSH_off", lshDest);

	if (_model->getButton("RSHOULDER"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RSH_on", rshDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RSH_off", rshDest);

	if (_model->getButton("LSTICK"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "JOY_on", leftJoyDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "JOY_off", leftJoyDest);

	if (_model->getButton("RSTICK"))
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "JOY_on", rightJoyDest);
	else
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "JOY_off", rightJoyDest);

	Sint16 leftT = ltrigger / 22;
	Sint16 rightT = rtrigger / 22;

	if (leftT == 0)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LTR_0", ltDest);
	else if(leftT == 1)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LTR_1", ltDest);
	else if(leftT == 2)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LTR_2", ltDest);
	else if(leftT == 3)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LTR_3", ltDest);
	else if(leftT == 4)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LTR_4", ltDest);
	else if(leftT == 5)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "LTR_5", ltDest);

	if (rightT == 0)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RTR_0", rtDest);
	else if(rightT == 1)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RTR_1", rtDest);
	else if(rightT == 2)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RTR_2", rtDest);
	else if(rightT == 3)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RTR_3", rtDest);
	else if(rightT == 4)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RTR_4", rtDest);
	else if(rightT == 5)
		renderer->copy(XBOX_CONTROLLER_TEXTURE_NAME, "RTR_5", rtDest);
}

/* ---------------------------------------------------- */
/* -------------------- CONTROLLER -------------------- */
/* ---------------------------------------------------- */

void GameControllerDebug::KeyboardEventHandler::handleEvent(SDL_Event const & event,
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

GameControllerDebug::GameControllerEventHandler::GameControllerEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{}

GameControllerDebug::GameControllerEventHandler::~GameControllerEventHandler(void)
{}

void GameControllerDebug::GameControllerEventHandler::handleEvent(SDL_Event const & event,
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
					update->pushGameContext(Pause::Factory::createPause(
						_platform,
						std::make_shared<GameControllerDebug::View>(
							_platform,
							_model)));
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
					/*
					if (controller)
						controller->rumble(1.f, 2500);
						*/
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