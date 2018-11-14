#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include "../GameContext.hpp"
#include <VBN/IModel.hpp>
#include <VBN/IView.hpp>
#include <VBN/IEventHandler.hpp>

class WindowManager;

class Menu
{
	public:
		class Model : public IModel
		{
			public:
				enum Item
				{
					START,
					DEBUG,
					EXIT
				};

			private:
				Item _currentSelection;
				SDL_Color _selectionColor;
				bool _ascend;

			public:
				Model(void);
				Item getCurrentSelection(void);
				void setCurrentSelection(Item);
				SDL_Color getSelectionColor(void);
				void elapse(Uint32 const, std::shared_ptr<EngineUpdate>);
		};

		class View : public IView
		{
			private:
				std::shared_ptr<Model> _model;
				std::shared_ptr<Platform> _platform;

			public:
				View(std::shared_ptr<Model> data,
					std::shared_ptr<Platform> platform);
				void display(void);
		};

		class KeyboardEventHandler : public IEventHandler
		{
			private:
				std::shared_ptr<Platform> _platform;
				std::shared_ptr<Model> _model;
				void perform(std::shared_ptr<EngineUpdate> engineUpdate);

			public:
				KeyboardEventHandler(std::shared_ptr<Platform> platform,
									std::shared_ptr<Model> model);
				void handleEvent(SDL_Event const & event,
					std::shared_ptr<EngineUpdate> engineUpdate);
		};
};

#endif // MENU_HPP_INCLUDED
