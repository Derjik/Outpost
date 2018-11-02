#ifndef WINDOW_EVENT_HANDLER_HPP_INCLUDED
#define WINDOW_EVENT_HANDLER_HPP_INCLUDED

#include "IEventHandler.hpp"
#include <memory>

class Platform;

class WindowEventHandler : public IEventHandler
{
	private:
		std::shared_ptr<Platform> _platform;
		std::shared_ptr<IEventHandler> _subHandler;

	public:
		WindowEventHandler(std::shared_ptr<Platform> platform,
			std::shared_ptr<IEventHandler> subHandler);
		void handleEvent(SDL_Event const & event,
			std::shared_ptr<EngineUpdate> engineUpdate);
};

#endif // WINDOW_EVENT_HANDLER_HPP_INCLUDED
