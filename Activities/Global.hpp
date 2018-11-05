#ifndef GLOBAL_HPP_INCLUDED
#define GLOBAL_HPP_INCLUDED

#include <VBN/IView.hpp>
#include <VBN/EventDispatcher.hpp>

class Platform;

class Global
{
	public:
		class View : public IView
		{
			private:
				std::shared_ptr<Platform> _platform;
				std::shared_ptr<IView> _subView;

			public:
				View(std::shared_ptr<Platform> platform,
					std::shared_ptr<IView> subView);
				void display(void);
		};

		class EventHandler : public EventDispatcher
		{
			public:
				EventHandler(
					std::shared_ptr<Platform> platform,
					std::shared_ptr<IEventHandler> mouse,
					std::shared_ptr<IEventHandler> keyboard,
					std::shared_ptr<IEventHandler> gameController,
					std::shared_ptr<IEventHandler> joystick,
					std::shared_ptr<IEventHandler> window);
		};

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
};

#endif // GLOBAL_HPP_INCLUDED
