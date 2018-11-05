#ifndef PAUSE_HPP_INCLUDED
#define PAUSE_HPP_INCLUDED

#include "../GameContext.hpp"
#include <VBN/IModel.hpp>
#include <VBN/IView.hpp>
#include <VBN/IEventHandler.hpp>

class Pause
{
	public:
		class GameControllerEventHandler : public IEventHandler
		{
			public:
				void handleEvent(SDL_Event const & event,
					std::shared_ptr<EngineUpdate> update);
		};

		class View : public IView
		{
			private:
				std::shared_ptr<Platform> _platform;
				std::shared_ptr<IView> _background;

			public:
				View(std::shared_ptr<Platform> platform,
					std::shared_ptr<IView> background);
				void display(void);
		};
};

#endif // PAUSE_HPP_INCLUDED
