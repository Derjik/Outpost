#ifndef KEYBOARD_EVENT_HANDLER_HPP_INCLUDED
#define KEYBOARD_EVENT_HANDLER_HPP_INCLUDED

#include "IEventHandler.hpp"
#include <memory>

class Platform;

class KeyboardEventHandler : public IEventHandler
{
	private:
		std::shared_ptr<Platform> _platform;
		std::shared_ptr<IEventHandler> _subHandler;

	public:
		KeyboardEventHandler(std::shared_ptr<Platform> platform,
			std::shared_ptr<IEventHandler> subHandler);
		void handleEvent(SDL_Event const & event,
			std::shared_ptr<EngineUpdate> engineUpdate);
};

#endif // KEYBOARD_EVENT_HANDLER_HPP_INCLUDED

