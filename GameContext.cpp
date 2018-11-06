#include "GameContext.hpp"
#include <VBN/Platform.hpp>
#include <VBN/IModel.hpp>
#include <VBN/IView.hpp>
#include <VBN/IEventHandler.hpp>

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
	if (_view)
		_view->display();
}

void GameContext::elapse(Uint32 const gameTicks,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	if(_model)
		_model->elapse(gameTicks, engineUpdate);
}
