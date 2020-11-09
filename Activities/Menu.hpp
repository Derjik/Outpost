#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include "../GameContext.hpp"
#include <VBN/IModel.hpp>
#include <VBN/IView.hpp>
#include <VBN/IEventHandler.hpp>
#include <array>

#define NB_MENU_ENTRIES 5

class WindowManager;

namespace Menu
{
	class Factory
	{
		public:
			static std::shared_ptr<GameContext> createMenu(
				std::shared_ptr<Platform> platform);
	};

	class Model : public IModel
	{
		public:
			enum Item
			{
				APP_1,
				APP_2,
				APP_3,
				APP_4,
				EXIT
			};

		private:
			std::array<Menu::Model::Item, NB_MENU_ENTRIES> _menuEntries;
			unsigned int _currentSelection;
			SDL_Color _backgroundColor;
			SDL_Color _textColor;
			SDL_Color _selectionColor;
			bool _ascend;

		public:
			Model(void);
			Item getCurrentSelection(void);
			void setCurrentSelection(unsigned int const);
			void cycleUp(void);
			void cycleDown(void);
			void cycleTo(Item const app);
			SDL_Color getBackgroundColor(void);
			SDL_Color getTextColor(void);
			SDL_Color getSelectionColor(void);
			void elapse(Uint32 const, std::shared_ptr<EngineUpdate>);
	};

	class View : public IView
	{
		private:
			std::shared_ptr<Model> _model;
			std::shared_ptr<Platform> _platform;

		public:
			View(std::shared_ptr<Platform> platform,
				std::shared_ptr<Model> data);
			void display(void);
	};

	class Controller : public IEventHandler
	{
		private:
			std::shared_ptr<Platform> _platform;
			std::shared_ptr<Model> _model;

			void performAction(std::shared_ptr<EngineUpdate> engineUpdate);
			void quickExit(std::shared_ptr<EngineUpdate> engineUpdate);

		public:
			Controller(std::shared_ptr<Platform> platform,
						std::shared_ptr<Model> model);
			void handleEvent(SDL_Event const & event,
				std::shared_ptr<EngineUpdate> engineUpdate);
	};
};

#endif // MENU_HPP_INCLUDED
