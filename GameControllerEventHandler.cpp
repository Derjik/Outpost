#include "GameControllerEventHandler.hpp"
#include "Platform.hpp"
#include <VBN/Logging.hpp>
#include <VBN/GameControllerManager.hpp>

GameControllerEventHandler::GameControllerEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> subHandler) :
	_platform(platform)
{}

void GameControllerEventHandler::handleEvent(SDL_Event const & event,
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