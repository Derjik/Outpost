#ifndef GAME_CONTEXT_HPP_INCLUDED
#define GAME_CONTEXT_HPP_INCLUDED

#include <VBN/IGameContext.hpp>

class Platform;
class IEventHandler;
class IView;
class IModel;

class GameContext : public IGameContext
{
	private:
		void handleKeyboardEvent(
			SDL_Event const & event,
			std::shared_ptr<EngineUpdate> response);
		void handleGameControllerEvent(
			SDL_Event const & event,
			std::shared_ptr<EngineUpdate> response);
		void handleMouseEvent(
			SDL_Event const & event,
			std::shared_ptr<EngineUpdate> response);
		void handleWindowEvent(
			SDL_Event const & event,
			std::shared_ptr<EngineUpdate> response);

	protected:
		std::shared_ptr<Platform> _platform;

		std::shared_ptr<IView> _view;

		std::shared_ptr<IModel> _model;

		std::shared_ptr<IEventHandler> _keyboardEventHandler;
		std::shared_ptr<IEventHandler> _gameControllerEventHandler;
		std::shared_ptr<IEventHandler> _mouseEventHandler;
		std::shared_ptr<IEventHandler> _windowEventHandler;

	public:
		GameContext(
			std::shared_ptr<Platform> platform,
			std::shared_ptr<IModel> model,
			std::shared_ptr<IView> view,
			std::shared_ptr<IEventHandler> keyboardEventHandler,
			std::shared_ptr<IEventHandler> gameControllerEventHandler,
			std::shared_ptr<IEventHandler> mouseEventHandler,
			std::shared_ptr<IEventHandler> windowEventHandler);

		void handleEvent(
			SDL_Event const & event,
			std::shared_ptr<EngineUpdate> response);
		void display(void);

		void elapse(Uint32 const gameTicks);
};

#endif // GAME_CONTEXT_HPP_INCLUDED
