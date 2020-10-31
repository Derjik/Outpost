#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

#include <map>
#include <VBN/IModel.hpp>
#include <VBN/IView.hpp>
#include <VBN/IEventHandler.hpp>

namespace Debug
{
	class Model : public IModel
	{
		private:
			std::shared_ptr<Platform> _platform;

			std::pair<Sint16, Sint16> _leftJoystick;
			std::pair<Sint16, Sint16> _rightJoystick;
			std::pair<Sint16, Sint16> _triggers;
			std::map<std::string, bool> _buttons;

		public:
			Model(std::shared_ptr<Platform> platform);
			void elapse(Uint32 const gameTicks,
				std::shared_ptr<EngineUpdate> engineUpdate);

			std::pair<Sint16, Sint16> getLeftJoystick(void);
			std::pair<Sint16, Sint16> getRightJoystick(void);
			std::pair<Sint16, Sint16> getTriggers(void);
			bool getButton(std::string const & button);
			std::map<std::string, bool> const & getButtons(void) const;
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
			~GameControllerEventHandler(void);
			void handleEvent(SDL_Event const & event,
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
};

#endif // DEBUG_HPP_INCLUDED
