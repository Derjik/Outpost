#include "GameContext.hpp"
#include "Platform.hpp"
#include "IEventHandler.hpp"
#include "IView.hpp"
#include "IModel.hpp"

#include <VBN/Logging.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>

GameContext::GameContext(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IModel> model,
	std::shared_ptr<IView> view,
	std::shared_ptr<IEventHandler> eventHandler) :
	_platform(platform),
	_model(model),
	_view(view),
	_eventHandler(eventHandler)
{}

void GameContext::handleEvent(SDL_Event const & event,
				std::shared_ptr<EngineUpdate> engineUpdate)
{
	if (_eventHandler)
		_eventHandler->handleEvent(event, engineUpdate);
}

void GameContext::display(void)
{
	Window & mainWindow = _platform->getWindowManager()->getByName("mainWindow");
	SDL_Renderer * renderer(mainWindow.getRenderer());

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (_view)
		_view->display();

	SDL_RenderPresent(renderer);
}

void GameContext::elapse(Uint32 const gameTicks)
{
	if(_model)
		_model->elapse(gameTicks);
}
