#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

#include "GameContext.hpp"
#include "IModel.hpp"
#include "IView.hpp"
#include "IEventHandler.hpp"

class Debug
{
	public:
		class KeyboardEventHandler : public IEventHandler
		{
			public:
				void handleEvent(SDL_Event const & event,
					std::shared_ptr<EngineUpdate> response);
		};

		class GameControllerEventHandler : public IEventHandler
		{
			private:
				std::shared_ptr<Platform> _platform;

			public:
				GameControllerEventHandler(
					std::shared_ptr<Platform> platform);
				void handleEvent(SDL_Event const & event,
					std::shared_ptr<EngineUpdate> response);
		};

		class View : public IView
		{
			private:
				std::shared_ptr<Platform> _platform;

			public:
				View(std::shared_ptr<Platform> platform);
				void display(void);
		};
};

#endif // DEBUG_HPP_INCLUDED
