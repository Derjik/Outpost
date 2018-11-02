#ifndef GAME_CONTROLLER_EVENT_HANDLER
#define GAME_CONTROLLER_EVENT_HANDLER

#include "IEventHandler.hpp"
#include <memory>

class Platform;

class GameControllerEventHandler : public IEventHandler
{
	private:
		std::shared_ptr<Platform> _platform;
		std::shared_ptr<IEventHandler> _subHandler;

	public:
		GameControllerEventHandler(std::shared_ptr<Platform> platform,
			std::shared_ptr<IEventHandler> subHandler);
		void handleEvent(SDL_Event const & event,
			std::shared_ptr<EngineUpdate> engineUpdate);
};

#endif // GAME_CONTROLLER_EVENT_HANDLER
