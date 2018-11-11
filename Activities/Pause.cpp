#include "Pause.hpp"
#include <VBN/Platform.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/WindowManager.hpp>

Pause::View::View(std::shared_ptr<Platform> platform,
	std::shared_ptr<IView> background) :
	_platform(platform),
	_background(background)
{}

void Pause::View::display(void)
{
	Window * mainWindow = _platform->getWindowManager()->getWindowByName("mainWindow");
	SDL_Renderer * renderer(mainWindow->getRenderer());

	_background->display();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
	SDL_RenderFillRect(renderer, nullptr);

	mainWindow->printText(
		"PAUSE",
		"courier", 40,
		{ 255, 255, 255, 255 },
		{230, 220, 250, 50});

	SDL_RenderPresent(renderer);
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