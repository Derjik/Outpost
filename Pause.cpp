#include "Pause.hpp"
#include <VBN/Engine.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/EngineUpdate.hpp>

//GCPause::GCPause(std::shared_ptr<WindowManager> windowManager,
//	std::shared_ptr<GameControllerManager> gameControllerManager) :
//	GameContext(windowManager, gameControllerManager)
//{}

//void GCPause::drawContext(void)
//{
//	Window & mainWindow = _windowManager->getByName("mainWindow");
//	SDL_Renderer * renderer(mainWindow.getRenderer());

//	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//	SDL_RenderClear(renderer);

//	mainWindow.printText("PAUSE", "courier", 40, { 255, 255, 255, 255 }, {230, 220, 250, 50});

//	SDL_RenderPresent(renderer);
//}

//void GCPause::handleKeyboardEvent(SDL_Event const & event,
//	std::shared_ptr<EngineUpdate> response)
//{
//	switch(event.type)
//	{
//		case SDL_KEYDOWN:
//			switch(event.key.keysym.sym)
//			{
//				case SDLK_ESCAPE:
//					response->popGameContext();
//				break;
//			}
//		break;
//	}
//}
