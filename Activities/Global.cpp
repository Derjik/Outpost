#include "Global.hpp"
#include <VBN/Platform.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/Window.hpp>
#include <VBN/Logging.hpp>

#define LOG_WIDTH 1000
#define LOG_HEIGHT 400

Global::Model::Model(void) : _showLogs(true)
{}

std::shared_ptr<Global::Model> Global::Model::getInstance(void)
{
	static std::shared_ptr<Global::Model> instance(new Model);
	return instance;
}

void Global::Model::elapse(Uint32 const gameTicks,
	std::shared_ptr<EngineUpdate> engineUpdate)
{}

void Global::Model::setShowLogs(bool const state)
{
	_showLogs = state;
}

void Global::Model::toggleShowLogs(void)
{
	_showLogs = !_showLogs;
}

bool Global::Model::getShowLogs(void) const
{
	return _showLogs;
}

Global::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<IView> subView) :
	_platform(platform),
	_subView(subView)
{}

void Global::View::display(void)
{
	Window * mainWindow = _platform->getWindowManager()->getWindowByName("mainWindow");
	Renderer * renderer(mainWindow->getRenderer());

	renderer->setDrawColor(0, 0, 0, 255);
	renderer->clear();

	if (_subView)
		_subView->display();

	if (Model::getInstance()->getShowLogs())
	{
		std::pair<int, int> winSize = mainWindow->getSize();
		renderer->printText(Logger::lastLogs(),
			"courier", 12, { 255, 255, 255, 255 },
			{ winSize.first - LOG_WIDTH, winSize.second - LOG_HEIGHT,
			LOG_WIDTH, LOG_HEIGHT});
	}

	renderer->present();
}

Global::EventHandler::EventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> mouse,
	std::shared_ptr<IEventHandler> keyboard,
	std::shared_ptr<IEventHandler> gameController,
	std::shared_ptr<IEventHandler> joystick,
	std::shared_ptr<IEventHandler> window) :
	EventDispatcher(
		std::shared_ptr<IEventHandler>(
			new Global::MouseEventHandler(platform, mouse)),
		std::shared_ptr<IEventHandler>(
			new Global::KeyboardEventHandler(platform, keyboard)),
		std::shared_ptr<IEventHandler>(
			new Global::GameControllerEventHandler(platform, gameController)),
		std::shared_ptr<IEventHandler>(
			new Global::JoystickEventHandler(platform, joystick)),
		std::shared_ptr<IEventHandler>(
			new Global::WindowEventHandler(platform, window)))
{}

Global::KeyboardEventHandler::KeyboardEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> subHandler) :
	_platform(platform),
	_subHandler(subHandler)
{}

void Global::KeyboardEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	Uint32 keyEvType = event.key.type;
	Window * mainWindow(_platform->getWindowManager()->getWindowByName("mainWindow"));

	switch(event.key.keysym.sym)
	{
		case SDLK_F11:
			if(keyEvType == SDL_KEYDOWN)
				mainWindow->toggleFullscreen();
		break;
		case SDLK_F12:
			if(keyEvType == SDL_KEYDOWN)
			Model::getInstance()->toggleShowLogs();
		break;
	}

	if(_subHandler)
		_subHandler->handleEvent(event, engineUpdate);
}

Global::MouseEventHandler::MouseEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> subHandler) :
	_platform(platform),
	_subHandler(subHandler)
{}

void Global::MouseEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	switch (event.type)
	{
//		case SDL_MOUSEMOTION:
//			VERBOSE(SDL_LOG_CATEGORY_INPUT,
//				"Mouse motion (%d, %d)",
//				event.motion.xrel,
//				event.motion.yrel);
//		break;
		case SDL_MOUSEBUTTONDOWN:
			VERBOSE(SDL_LOG_CATEGORY_INPUT,
				"Mouse button %d down @[%d, %d]",
				event.button.button,
				event.button.x,
				event.button.y);
		break;
		case SDL_MOUSEBUTTONUP:
			VERBOSE(SDL_LOG_CATEGORY_INPUT,
				"Mouse button %d up",
				event.button.button);
		break;
		case SDL_MOUSEWHEEL:
			VERBOSE(SDL_LOG_CATEGORY_INPUT,
				"Mouse wheel (%d, %d)",
				event.wheel.x,
				event.wheel.y);
		break;
	}
	if (_subHandler)
		_subHandler->handleEvent(event, engineUpdate);
}

Global::GameControllerEventHandler::GameControllerEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> subHandler) :
	_platform(platform),
	_subHandler(subHandler)
{}

void Global::GameControllerEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	switch(event.type)
	{
		case SDL_CONTROLLERDEVICEADDED:
			INFO(SDL_LOG_CATEGORY_INPUT,
				"Device #%d added",
				event.cdevice.which);

			_platform
				->getGameControllerManager()
				->openFromDeviceIndex(event.cdevice.which);
		break;
		case SDL_CONTROLLERDEVICEREMOVED:
			INFO(SDL_LOG_CATEGORY_INPUT,
				"Instance @%d removed",
				event.cdevice.which);

			_platform
				->getGameControllerManager()
				->closeInstance(event.cdevice.which);
		break;

		case SDL_CONTROLLERDEVICEREMAPPED:
			INFO(SDL_LOG_CATEGORY_INPUT,
				"Instance @%d was remapped",
				event.cdevice.which);
		break;

		default:
			if (_subHandler)
				_subHandler->handleEvent(event, engineUpdate);
		break;
	}
}

Global::JoystickEventHandler::JoystickEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> subHandler) :
	_platform(platform),
	_subHandler(subHandler)
{}

void Global::JoystickEventHandler::handleEvent(
	SDL_Event const & event,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	switch (event.type)
	{
		case SDL_JOYDEVICEADDED:
			INFO(SDL_LOG_CATEGORY_INPUT,
				"Device #%d added",
				event.jdevice.which);
		break;
		case SDL_JOYDEVICEREMOVED:
			INFO(SDL_LOG_CATEGORY_INPUT,
				"Instance @%d added",
				event.jdevice.which);
		break;
		default:
			if (_subHandler)
				_subHandler->handleEvent(event, engineUpdate);
		break;
	}
}

Global::WindowEventHandler::WindowEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> subHandler) :
	_platform(platform),
	_subHandler(subHandler)
{}

void Global::WindowEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	Window * window(nullptr);

	switch (event.window.event)
	{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			window = _platform
				->getWindowManager()
				->getWindowById(event.window.windowID);

			DEBUG(SDL_LOG_CATEGORY_APPLICATION,
				"Window (ID: %d) changed size to %dx%d",
				window->getId(),
				event.window.data1,
				event.window.data2);

			window->handleResize();
		break;
		default:
			if (_subHandler)
				_subHandler->handleEvent(event, engineUpdate);
		break;
	}
}