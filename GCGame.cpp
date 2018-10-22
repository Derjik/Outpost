#include "GCGame.hpp"
#include "GCPause.hpp"
#include <VBN/Engine.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/HandlerResponse.hpp>

//GCGame::GCGame(std::shared_ptr<WindowManager> windowManager,
//		std::shared_ptr<GameControllerManager> gameControllerManager) :
//	GameContext(windowManager, gameControllerManager)
//{}

//void GCGame::drawContext(void)
//{
//	Window & mainWindow = _windowManager->getByName("mainWindow");
//	SDL_Renderer * renderer(mainWindow.getRenderer());

//	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//	SDL_RenderClear(renderer);
//	SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
//	SDL_RenderFillRect(renderer, nullptr);

//	mainWindow.printText("GCGame", "courier", 20, { 20, 180, 20, 255 }, { 0, 0, 200, 32 });
//	mainWindow.printText("(such game)", "courier", 40, {20, 180, 20, 255}, { 0, 40, 400, 48 });

//	SDL_RenderPresent(renderer);
//}

//void GCGame::handleKeyboardEvent(SDL_Event const & event,
//	std::shared_ptr<HandlerResponse> response)
//{
//	SDL_Renderer* r(nullptr);
//	r=_windowManager->getByName("mainWindow").getRenderer();
//	SDL_Rect viewport{ 150, 150, 1066, 468 };
//	switch(event.type)
//	{
//		case SDL_KEYDOWN:
//			switch(event.key.keysym.sym)
//			{
//				case SDLK_ESCAPE:
//					response->popGameContext();
//				break;
//				case SDLK_p:
//					response->pushGameContext(std::shared_ptr<IGameContext>(
//						new GCPause(_windowManager, _gameControllerManager)));
//				break;
//				case SDLK_2:
//					SDL_RenderSetScale(r, 2, 2);
//				break;
//				case SDLK_1:
//					SDL_RenderSetScale(r, 1, 1);
//				break;
//				case SDLK_v:
//					SDL_RenderSetViewport(r, &viewport);
//				break;
//				case SDLK_b:
//					SDL_RenderSetViewport(r, nullptr);
//				break;
//			}
//		break;
//	}
//}
