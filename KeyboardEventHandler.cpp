#include "KeyboardEventHandler.hpp"
#include "Platform.hpp"
#include <VBN/WindowManager.hpp>

KeyboardEventHandler::KeyboardEventHandler(std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> subHandler) :
	_platform(platform),
	_subHandler(subHandler)
{}

void KeyboardEventHandler::handleEvent(SDL_Event const & event,
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