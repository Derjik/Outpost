#ifndef GLOBAL_HANDLER_HPP_INCLUDED
#define GLOBAL_HANDLER_HPP_INCLUDED

#include "EventHandler.hpp"

class Platform;

class GlobalHandler : public EventHandler
{
	public:
		GlobalHandler(
			std::shared_ptr<Platform> platform,
			std::shared_ptr<IEventHandler> mouse,
			std::shared_ptr<IEventHandler> keyboard,
			std::shared_ptr<IEventHandler> gameController,
			std::shared_ptr<IEventHandler> joystick,
			std::shared_ptr<IEventHandler> window);
};

#endif // GLOBAL_HANDLER_HPP_INCLUDED
