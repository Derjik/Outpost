#include "Menu.hpp"
#include "Debug.hpp"
#include "GlobalHandler.hpp"
#include "KeyboardEventHandler.hpp"
#include "GameControllerEventHandler.hpp"
#include <VBN/WindowManager.hpp>
#include <VBN/EngineUpdate.hpp>

Menu::KeyboardEventHandler::KeyboardEventHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<Menu::Model> model) :
	_platform(platform),
	_model(model)
{}

void Menu::KeyboardEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> response)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_UP:
					switch(_model->getCurrentSelection())
					{
						case Model::START:
							_model->setCurrentSelection(Model::DEBUG);
						break;
						case Model::EXIT:
							_model->setCurrentSelection(Model::START);
						break;
						case Model::DEBUG:
							_model->setCurrentSelection(Model::EXIT);
						break;
					}
				break;
				case SDLK_DOWN:
					switch(_model->getCurrentSelection())
					{
						case Model::START:
							_model->setCurrentSelection(Model::EXIT);
						break;
						case Model::EXIT:
							_model->setCurrentSelection(Model::DEBUG);
						break;
						case Model::DEBUG:
							_model->setCurrentSelection(Model::START);
						break;
					}
				break;
				case SDLK_RETURN:
					perform(response);
				break;
				case SDLK_ESCAPE:
					_model->setCurrentSelection(Model::EXIT);
					perform(response);
				break;
			}
		break;
	}
}

void Menu::KeyboardEventHandler::perform(std::shared_ptr<EngineUpdate> response)
{
	std::shared_ptr<Debug::Model> model(nullptr);

	switch(_model->getCurrentSelection())
	{
		case Model::DEBUG:
			model = std::shared_ptr<Debug::Model>(new Debug::Model(_platform));

			response->pushGameContext(
				std::shared_ptr<IGameContext>(new GameContext(
					_platform,
					model,
					std::shared_ptr<IView>(
						new Debug::View(_platform, model)),
					std::shared_ptr<IEventHandler>(
						new GlobalHandler(
							_platform,
							nullptr,
							std::shared_ptr<IEventHandler>(
								new Debug::KeyboardEventHandler),
							std::shared_ptr<IEventHandler>(
								new Debug::GameControllerEventHandler(_platform, model)),
							nullptr,
							nullptr))))
				);
		break;
		case Model::START:
			/*
			response->pushGameContext(std::shared_ptr<IGameContext>(
				new GCGame(_windowManager, _gameControllerManager)));
			*/
		break;
		case Model::EXIT:
			response->popGameContext();
		break;
	}
}

Menu::View::View(std::shared_ptr<Model> model,
	std::shared_ptr<WindowManager> windowManager) :
	_model(model),
	_windowManager(windowManager)
{}

void Menu::View::display(void)
{
	Window & mainWindow = _windowManager->getByName("mainWindow");
	SDL_Renderer * renderer(mainWindow.getRenderer());

	/* Clear draw area with blue */
	SDL_SetRenderDrawColor(renderer, 0, 0, 32, 255);
	SDL_RenderFillRect(renderer, nullptr);

	/* Print menu name */
	mainWindow.printText("Menu", "courier",
		20, {200, 200, 200, 255}, {0, 0, 200, 32});

	SDL_Rect menuItems[3];
	menuItems[0] = { 220, 100, 200, 32 };
	menuItems[1] = { 220, 140, 200, 32 };
	menuItems[2] = { 220, 180, 200, 32 };

	SDL_Color menuColor{ 200, 200, 200, 255 };

	/* Print menu items */
	mainWindow.printText("Debug", "courier",
		20, menuColor, menuItems[0]);
	mainWindow.printText("Start", "courier",
		20, menuColor, menuItems[1]);
	mainWindow.printText("Exit", "courier",
		20, menuColor, menuItems[2]);

	/* Highlight selection */
	SDL_SetRenderDrawColor(renderer,
		_model->getSelectionColor().r,
		_model->getSelectionColor().g,
		_model->getSelectionColor().b,
		_model->getSelectionColor().a);

	SDL_Rect r;
	switch(_model->getCurrentSelection())
	{
		case Model::DEBUG:
			r = menuItems[0];
		break;
		case Model::START:
			r = menuItems[1];
		break;
		case Model::EXIT:
			r = menuItems[2];
		break;
	}
	SDL_RenderDrawRect(renderer, &r);
}

Menu::Model::Model(void) :
	_currentSelection(DEBUG),
	_selectionColor{0, 0, 150, 255},
	_ascend(true)
{}

Menu::Model::Item Menu::Model::getCurrentSelection(void)
{
	return _currentSelection;
}

void Menu::Model::setCurrentSelection(Menu::Model::Item selection)
{
	_currentSelection = selection;
}

void Menu::Model::elapse(Uint32 const gameTicks,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	if (_ascend && _selectionColor.b < 200)
	{
		_selectionColor.b += 4;
		if (_selectionColor.b >= 200)
			_ascend = false;
	}
	else if (!_ascend && _selectionColor.b > 50)
	{
		_selectionColor.b -= 4;
		if (_selectionColor.b <= 50)
			_ascend = true;
	}
}

SDL_Color Menu::Model::getSelectionColor(void)
{
	return _selectionColor;
}