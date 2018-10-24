#include "GameContext.hpp"
#include "Platform.hpp"
#include "IEventHandler.hpp"
#include "IView.hpp"
#include "IModel.hpp"

#include <SDL2/SDL_log.h>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>

GameContext::GameContext(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IModel> model,
	std::shared_ptr<IView> view,
	std::shared_ptr<IEventHandler> keyboardEventHandler,
	std::shared_ptr<IEventHandler> gameControllerEventHandler,
	std::shared_ptr<IEventHandler> mouseEventHandler,
	std::shared_ptr<IEventHandler> windowEventHandler) :
	_platform(platform),
	_model(model),
	_view(view),
	_keyboardEventHandler(keyboardEventHandler),
	_gameControllerEventHandler(gameControllerEventHandler),
	_mouseEventHandler(mouseEventHandler),
	_windowEventHandler(windowEventHandler)
{}

void GameContext::handleEvent(SDL_Event const & event,
				std::shared_ptr<EngineUpdate> response)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			handleKeyboardEvent(event, response);
		break;

		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			handleGameControllerEvent(event, response);
		break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			handleMouseEvent(event, response);
		break;

		case SDL_WINDOWEVENT:
			handleWindowEvent(event, response);
		break;
	}
}

void GameContext::handleGameControllerEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> response)
{
	switch(event.type)
	{
		/* Add a game controller device */
		case SDL_CONTROLLERDEVICEADDED:
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT,
				"[EVENT] Device #%d added", event.cdevice.which);

			_platform->getGameControllerManager()->openFromDeviceIndex(event.cdevice.which);
		break;
		/* Remove a game controller device */
		case SDL_CONTROLLERDEVICEREMOVED:
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT,
				"[EVENT] Instance @%d removed", event.cdevice.which);

			_platform->getGameControllerManager()->closeInstance(event.cdevice.which);
		break;

		default:
			if (_gameControllerEventHandler)
				_gameControllerEventHandler->handleEvent(event, response);
		break;
	}
}

void GameContext::handleKeyboardEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> response)
{
	Uint32 keyEvType = event.key.type;
	Window & main(_platform->getWindowManager()->getByName("mainWindow"));

	switch(event.key.keysym.sym)
	{
		case SDLK_F11:
			if(keyEvType == SDL_KEYDOWN)
				main.toggleFullscreen();
		break;
	}

	if(_keyboardEventHandler)
		_keyboardEventHandler->handleEvent(event, response);
}

void GameContext::handleMouseEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> response)
{
	if (_mouseEventHandler)
		_mouseEventHandler->handleEvent(event, response);
}

void GameContext::handleWindowEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> response)
{
	std::string windowName("");
	SDL_Window * address(nullptr);

	switch (event.window.event)
	{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			address = SDL_GetWindowFromID(event.window.windowID);
			windowName = _platform->getWindowManager()->getWindowNameByAddress(address);

			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
				"Window '%s' (ID: %d) changed size to %dx%d",
				windowName.c_str(), _platform->getWindowManager()->getByAddress(address).getId(),
				event.window.data1, event.window.data2);
			_platform->getWindowManager()->getByAddress(address).handleResize();
		break;
		default:
			if (_windowEventHandler)
				_windowEventHandler->handleEvent(event, response);
		break;
	}
}

void GameContext::display(void)
{
	Window & mainWindow = _platform->getWindowManager()->getByName("mainWindow");
	SDL_Renderer * renderer(mainWindow.getRenderer());

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (_view)
		_view->display();

	SDL_RenderPresent(renderer);
}

void GameContext::elapse(Uint32 const gameTicks)
{
	if(_model)
		_model->elapse(gameTicks);
}
