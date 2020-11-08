#include "Pause.hpp"
#include "Global.hpp"
#include <VBN/Platform.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/WindowManager.hpp>

std::shared_ptr<GameContext> Pause::Factory::createPause(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IView> subView)
{
	return std::make_shared<GameContext>(
			nullptr,
			std::make_shared<Global::View>(
				platform,
				std::make_shared<Pause::View>(
					platform,
					subView)),
			std::make_shared<Global::EventHandler>(
					platform,
					nullptr,
					nullptr,
					std::make_shared<Pause::GameControllerEventHandler>(),
					nullptr,
					nullptr)
		);
}

Pause::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<IView> background) :
	_platform(platform),
	_background(background)
{}

void Pause::View::display(void)
{
	Window * mainWindow = _platform->getWindowManager()->getWindowByName("mainWindow");
	Renderer * renderer(mainWindow->getRenderer());

	_background->display();

	renderer->setDrawColor(0, 0, 0, 100);
	renderer->fill();

	mainWindow->getRenderer()->printText(
		"PAUSE",
		"courier", 40,
		{ 255, 255, 255, 255 },
		{230, 220, 250, 50});
}

void Pause::GameControllerEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> update)
{
	switch (event.type)
	{
		case SDL_CONTROLLERBUTTONDOWN:
			switch (event.cbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_START:
					update->popGameContext();
				break;
			}
		break;
	}
}