#include <VBN/Platform.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/Logging.hpp>
#include "../GameContext.hpp"
#include "Tank.hpp"
#include "Global.hpp"
#include <cmath>

std::shared_ptr<GameContext> Tank::Factory::createGameControllerDebug(
	std::shared_ptr<Platform> platform)
{
	std::shared_ptr<Tank::Model> model(std::make_shared<Tank::Model>(platform));

	return std::make_shared<GameContext>(
		model,
		std::make_shared<Global::View>(
			platform,
			std::make_shared<Tank::View>(platform, model)),
		std::make_shared<Global::EventHandler>(
			platform,
			nullptr,
			std::make_shared<Tank::KeyboardEventHandler>(),
			std::make_shared<Tank::GameControllerEventHandler>(platform, model),
			nullptr,
			nullptr));
}

Tank::Model::Model(std::shared_ptr<Platform> platform) :
	_platform(platform),
	_x(500), _y(200),
	_deltaX(0), _deltaY(0),
	_dir(0)
{}

void Tank::Model::elapse(Uint32 const gameTicks,
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

	int leftJ(0), rightJ(0);

	if (sdlController)
	{
		leftJ = SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_LEFTY) / 256;
		rightJ = SDL_GameControllerGetAxis(sdlController, SDL_CONTROLLER_AXIS_RIGHTY) / 256;
	}
	double accel = -leftJ + -rightJ;

	double deltaAngle = rightJ / 40 - leftJ / 40;
	_dir = fmod(_dir + deltaAngle, 360.f);

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"[ %f, %f] - [ %f, %f ] - [ T : %f ] - [ dT : %f ] - [ v : %f ]",
		_x, _y,
		_deltaX, _deltaY,
		_dir,
		deltaAngle, accel);

	_deltaX = accel/20 * cos(_dir * (M_PI / 180.f));
	_deltaY = accel/20 * sin(_dir * (M_PI / 180.f));
	_x += _deltaX;
	_y += _deltaY;
}

Tank::View::View(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{
	Window * mainWindow(_platform->getWindowManager()->getWindowByName("mainWindow"));
	Renderer * renderer(nullptr);
	if (mainWindow)
		renderer = mainWindow->getRenderer();

	if (renderer && !renderer->getTexture("TANK"))
	{
		renderer->addImageTexture( "TANK", "assets/textures/tank.png");
	}
}

void Tank::View::display(void)
{
	Window * mainWindow = _platform->getWindowManager()->getWindowByName("mainWindow");
	Renderer * renderer = mainWindow->getRenderer();

	renderer->setDrawColor(0, 0, 0, 255);
	renderer->fill();

	renderer->printText("TANK", "courier", 12, { 255, 255, 255, 255 }, {10, 10, 100, 22});

	renderer->copyEx("TANK", "", SDL_Rect{ (int)(_model->_x), (int)(_model->_y), 256, 256 },
		_model->_dir, SDL_Point{ 128, 128 }, SDL_FLIP_NONE);

	renderer->setDrawColor(255, 0, 0, 255);
	renderer->drawLine(
		200,
		200,
		200 + 10*_model->_deltaX,
		200 + 10*_model->_deltaY);
}

void Tank::KeyboardEventHandler::handleEvent(
	SDL_Event const & e,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	switch(e.type)
	{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					engineUpdate->popGameContext();
				break;
			}
		break;
	}
}

Tank::GameControllerEventHandler::GameControllerEventHandler(
	std::shared_ptr<Platform> platform, std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{}

void Tank::GameControllerEventHandler::handleEvent(
	SDL_Event const & e,
	std::shared_ptr<EngineUpdate> engineUpdate)
{

}