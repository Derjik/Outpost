#ifndef I_EVENT_HANDLER_HPP_INCLUDED
#define I_EVENT_HANDLER_HPP_INCLUDED

#include <memory>
#include <SDL2/SDL_events.h>

class HandlerResponse;

class IEventHandler
{
	public:
		virtual void handleEvent(SDL_Event const & event,
			std::shared_ptr<HandlerResponse> response) = 0;
};

#endif // I_EVENT_HANDLER_HPP_INCLUDED
