#ifndef EVENT_HANDLER_HPP_INCLUDED
#define EVENT_HANDLER_HPP_INCLUDED

#include "IEventHandler.hpp"
#include <memory>

class EventHandler : public IEventHandler
{
	private:
		std::shared_ptr<IEventHandler> _mouseEventHandler;
		std::shared_ptr<IEventHandler> _keyboardEventHandler;
		std::shared_ptr<IEventHandler> _gameControllerEventHandler;
		std::shared_ptr<IEventHandler> _joystickEventHandler;
		std::shared_ptr<IEventHandler> _windowEventHandler;

	public:
		EventHandler(
			std::shared_ptr<IEventHandler> mouse,
			std::shared_ptr<IEventHandler> keyboard,
			std::shared_ptr<IEventHandler> gameController,
			std::shared_ptr<IEventHandler> joystick,
			std::shared_ptr<IEventHandler> window);

		void handleEvent(SDL_Event const & event,
			std::shared_ptr<EngineUpdate> engineUpdate);
};

#endif // EVENT_HANDLER_HPP_INCLUDED
