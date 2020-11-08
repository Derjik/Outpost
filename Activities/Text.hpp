#ifndef TEXT_HPP_INCLUDED
#define TEXT_HPP_INCLUDED

#include <VBN/IModel.hpp>
#include <VBN/IView.hpp>
#include <VBN/IEventHandler.hpp>

namespace Text
{
	class Factory
	{
		public:
			static std::shared_ptr<GameContext> createText(
				std::shared_ptr<Platform> platform);
	};

	class Model : public IModel
	{
		private:
			std::shared_ptr<Platform> _platform;

			unsigned int _fontSize;
			SDL_Rect _drawSpace;

			Uint32 aGT;
			Uint32 bGT;
			Uint32 xGT;
			Uint32 yGT;
			Uint32 upGT;
			Uint32 downGT;
			Uint32 leftGT;
			Uint32 rightGT;

		public:
			Model(std::shared_ptr<Platform> platform);
			void elapse(Uint32 const gameTicks,
				std::shared_ptr<EngineUpdate> engineUpdate);

			unsigned int getFontSize(void);
			SDL_Rect const & getDrawSpace(void);

			void p1left(int amount);
			void p1right(int amount);
			void p1up(int amount);
			void p1down(int amount);
			void p2left(int amount);
			void p2right(int amount);
			void p2up(int amount);
			void p2down(int amount);

			void upFont(int amount);
			void downFont(int amount);
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
			std::shared_ptr<Model> _model;

		public:
			GameControllerEventHandler(std::shared_ptr<Model> model);
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

#endif // TEXT_HPP_INCLUDED
