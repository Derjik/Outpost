#include "Menu.hpp"
#include "Global.hpp"
#include "Debug.hpp"
#include <VBN/WindowManager.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/Platform.hpp>
#include <VBN/Mixer.hpp>

/* ------------------ MODEL ------------------ */

Menu::Model::Model(void) :
	_menuEntries{
		Menu::Model::Item::ZONE_1,
		Menu::Model::Item::ZONE_2,
		Menu::Model::Item::ZONE_3,
		Menu::Model::Item::DEBUG,
		Menu::Model::Item::EXIT},
	_currentSelection(0),
	_backgroundColor{0, 0, 0, 255},
	_textColor{192, 192, 192, 255},
	_selectionColor{0, 0, 150, 255},
	_ascend(true)
{}

Menu::Model::Item Menu::Model::getCurrentSelection(void)
{
	return _menuEntries.at(_currentSelection);
}

void Menu::Model::setCurrentSelection(unsigned int const selection)
{
	_currentSelection = selection;
}

void Menu::Model::cycleUp(void)
{
	_currentSelection = (_currentSelection - 1);
	if (_currentSelection >= NB_MENU_ENTRIES)
		_currentSelection = NB_MENU_ENTRIES - 1;
}

void Menu::Model::cycleDown(void)
{
	_currentSelection = (_currentSelection + 1) % NB_MENU_ENTRIES;
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

SDL_Color Menu::Model::getBackgroundColor(void)
{
	return _backgroundColor;
}

SDL_Color Menu::Model::getTextColor(void)
{
	return _textColor;
}

SDL_Color Menu::Model::getSelectionColor(void)
{
	return _selectionColor;
}

/* ------------------ CONTROLLER ------------------ */

Menu::MenuController::MenuController(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_platform(platform),
	_model(model)
{}

void Menu::MenuController::performAction(std::shared_ptr<EngineUpdate> engineUpdate)
{
	std::shared_ptr<Debug::Model> model(nullptr);

	switch(_model->getCurrentSelection())
	{
		case Model::DEBUG:
			model = std::shared_ptr<Debug::Model>(new Debug::Model(_platform));

			engineUpdate->pushGameContext(
				std::shared_ptr<IGameContext>(new GameContext(
					model,
					std::shared_ptr<IView>(new Global::View(
							_platform,
							std::shared_ptr<IView>(
								new Debug::View(_platform, model)))),
					std::shared_ptr<IEventHandler>(
						new Global::EventHandler(
							_platform,
							nullptr,
							std::shared_ptr<IEventHandler>(
								new Debug::KeyboardEventHandler),
							std::shared_ptr<IEventHandler>(
								new Debug::GameControllerEventHandler(
									_platform, model)),
							nullptr,
							nullptr))))
				);
		break;
		case Model::ZONE_1:
			/*
			engineUpdate->pushGameContext(std::shared_ptr<IGameContext>(
				new GCGame(_windowManager, _gameControllerManager)));
			*/
		break;
		case Model::ZONE_2:
			/*
			engineUpdate->pushGameContext(std::shared_ptr<IGameContext>(
				new GCGame(_windowManager, _gameControllerManager)));
			*/
		break;
		case Model::ZONE_3:
			/*
			engineUpdate->pushGameContext(std::shared_ptr<IGameContext>(
				new GCGame(_windowManager, _gameControllerManager)));
			*/
		break;
		case Model::EXIT:
			engineUpdate->popGameContext();
		break;
	}
}

void Menu::MenuController::quickExit(std::shared_ptr<EngineUpdate> engineUpdate)
{
	_model->setCurrentSelection(Model::EXIT);
	performAction(engineUpdate);
}

void Menu::MenuController::handleEvent(SDL_Event const & event,
							std::shared_ptr<EngineUpdate> engineUpdate)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_UP:
					_model->cycleUp();
					_platform->getMixer()->playEffect("drum");
				break;
				case SDLK_DOWN:
					_model->cycleDown();
					_platform->getMixer()->playEffect("drum");
				break;
				case SDLK_RETURN:
					performAction(engineUpdate);
				break;
				case SDLK_ESCAPE:
					quickExit(engineUpdate);
				break;
				case SDLK_m:
					_platform->getMixer()->playMusic("ftl");
				break;
			}
		break;
		case SDL_CONTROLLERBUTTONDOWN:
			switch (event.cbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					_model->cycleDown();
					_platform->getMixer()->playEffect("drum");
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					_model->cycleUp();
					_platform->getMixer()->playEffect("drum");
				break;
				case SDL_CONTROLLER_BUTTON_A:
					performAction(engineUpdate);
				break;
				case SDL_CONTROLLER_BUTTON_BACK:
					quickExit(engineUpdate);
				break;
			}
		break;
	}
}

/* ------------------ VIEW ------------------ */

Menu::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<Model> model) :
	_model(model),
	_platform(platform)
{}

void Menu::View::display(void)
{
	WindowManager * windowManager(nullptr);
	Window * mainWindow(nullptr);
	Renderer * renderer(nullptr);

	windowManager = _platform->getWindowManager();
	if (windowManager)
		mainWindow = windowManager->getWindowByName("mainWindow");
	if (mainWindow)
		renderer = mainWindow->getRenderer();
	if (!renderer)
		return;

	/* Clear draw area with blue */
	renderer->setDrawColor(_model->getBackgroundColor());
	renderer->fill();

	/* Print menu name */
	renderer->printText(
		"Main Menu",
		"courier",
		20,
		_model->getTextColor(),
		{0, 0, 200, 64});

	SDL_Rect menuItems[NB_MENU_ENTRIES];
	menuItems[0] = { 220, 100, 300, 32 };
	menuItems[1] = { 220, 140, 300, 32 };
	menuItems[2] = { 220, 180, 300, 32 };
	menuItems[3] = { 220, 220, 300, 32 };
	menuItems[4] = { 220, 260, 300, 32 };

	mainWindow->getRenderer()->printText(
		"Some complex multi-line text",
		"courier",
		20,
		_model->getTextColor(),
		{40,100,150,150});

	/* Print menu items */
	mainWindow->getRenderer()->printText(
		"ZONE_1",
		"courier",
		20,
		_model->getTextColor(),
		menuItems[0]);
	mainWindow->getRenderer()->printText(
		"ZONE 2",
		"courier",
		20,
		_model->getTextColor(),
		menuItems[1]);
	mainWindow->getRenderer()->printText(
		"ZONE 3 3",
		"courier",
		20,
		_model->getTextColor(),
		menuItems[2]);
	mainWindow->getRenderer()->printText(
		"ZONE A",
		"courier",
		20,
		_model->getTextColor(),
		menuItems[3]);
	mainWindow->getRenderer()->printText(
		"EXIT",
		"courier",
		20,
		_model->getTextColor(),
		menuItems[4]);

	/* Highlight selection */
	renderer->setDrawColor(
		_model->getSelectionColor().r,
		_model->getSelectionColor().g,
		_model->getSelectionColor().b,
		_model->getSelectionColor().a);

	SDL_Rect r(menuItems[_model->getCurrentSelection()]);
	renderer->drawRect(r);
}
