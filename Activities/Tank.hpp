#ifndef TANK_HPP_INCLUDED
#define TANK_HPP_INCLUDED

#include <VBN/IModel.hpp>
#include <VBN/IView.hpp>
#include <VBN/IEventHandler.hpp>
#include <memory>


namespace Tank
{
	class Factory
	{
		public:
			static std::shared_ptr<GameContext> createGameControllerDebug(
				std::shared_ptr<Platform> platform);
	};

	class Model : public IModel
	{
		public:
			std::shared_ptr<Platform> _platform;

			double _x;
			double _y;

			double _deltaX;
			double _deltaY;

			double _dir;

			Model(std::shared_ptr<Platform> platform);

			void elapse(Uint32 const gameTicks,
				std::shared_ptr<EngineUpdate> engineUpdate);
	};

	class View : public IView
	{
		private:
			std::shared_ptr<Platform> _platform;
			std::shared_ptr<Model> _model;

		public:
			View(std::shared_ptr<Platform> platform,
				std::shared_ptr<Model> model);
			void display(void);
	};

	class KeyboardEventHandler : public IEventHandler
	{
		public:
			void handleEvent(SDL_Event const & event,
				std::shared_ptr<EngineUpdate> engineUpdate);
	};

	class GameControllerEventHandler : public IEventHandler
	{
		private:
			std::shared_ptr<Platform> _platform;
			std::shared_ptr<Model> _model;

		public:
			GameControllerEventHandler(
				std::shared_ptr<Platform> platform,
				std::shared_ptr<Model> model);
			void handleEvent(SDL_Event const & event,
				std::shared_ptr<EngineUpdate> engineUpdate);
	};
}

#endif // TANK_HPP_INCLUDED
