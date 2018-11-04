#include "Global.hpp"
#include "../Platform.hpp"
#include <VBN/GameControllerManager.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/Window.hpp>
#include <VBN/Logging.hpp>

Global::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<IView> subView) :
	_platform(platform),
	_subView(subView)
{}

void Global::View::display(void)
{
	Window & mainWindow = _platform->getWindowManager()->getByName("mainWindow");
	SDL_Renderer * renderer(mainWindow.getRenderer());

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (_subView)
		_subView->display();

	SDL_RenderPresent(renderer);
}

Global::EventHandler::EventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> mouse,
	std::shared_ptr<IEventHandler> keyboard,
	std::shared_ptr<IEventHandler> gameController,
	std::shared_ptr<IEventHandler> joystick,
	std::shared_ptr<IEventHandler> window) :
	EventDispatcher(mouse,
		std::shared_ptr<IEventHandler>(new Global::KeyboardEventHandler(platform, keyboard)),
		std::shared_ptr<IEventHandler>(new Global::GameControllerEventHandler(platform, gameController)),
		joystick,
		std::shared_ptr<IEventHandler>(new Global::WindowEventHandler(platform, window)))
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
	Window & mainWindow(_platform->getWindowManager()->getByName("mainWindow"));

	switch(event.key.keysym.sym)
	{
		case SDLK_F11:
			if(keyEvType == SDL_KEYDOWN)
				mainWindow.toggleFullscreen();
		break;
	}

	if(_subHandler)
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
				"Device #%d added", event.cdevice.which);

			_platform
				->getGameControllerManager()
				->openFromDeviceIndex(event.cdevice.which);
		break;
		case SDL_CONTROLLERDEVICEREMOVED:
			INFO(SDL_LOG_CATEGORY_INPUT,
				"Instance @%d removed", event.cdevice.which);

			_platform
				->getGameControllerManager()
				->closeInstance(event.cdevice.which);
		break;

		case SDL_CONTROLLERDEVICEREMAPPED:
			INFO(SDL_LOG_CATEGORY_INPUT, "A GameController was remapped");
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
	std::string windowName("");
	SDL_Window * address(nullptr);

	switch (event.window.event)
	{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			address = SDL_GetWindowFromID(event.window.windowID);
			windowName = _platform->getWindowManager()->getWindowNameByAddress(address);

			DEBUG(SDL_LOG_CATEGORY_APPLICATION,
				"Window '%s' (ID: %d) changed size to %dx%d",
				windowName.c_str(), _platform->getWindowManager()->getByAddress(address).getId(),
				event.window.data1, event.window.data2);
			_platform->getWindowManager()->getByAddress(address).handleResize();
		break;
		default:
			if (_subHandler)
				_subHandler->handleEvent(event, engineUpdate);
		break;
	}
}