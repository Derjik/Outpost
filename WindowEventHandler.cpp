#include "WindowEventHandler.hpp"
#include "Platform.hpp"
#include <VBN/WindowManager.hpp>
#include <VBN/Logging.hpp>
#include <string>

WindowEventHandler::WindowEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> subHandler) :
	_platform(platform)
{}

void WindowEventHandler::handleEvent(SDL_Event const & event,
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
