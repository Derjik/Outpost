#include "Platform.hpp"

Platform::Platform(std::shared_ptr<WindowManager> windowManager,
	std::shared_ptr<GameControllerManager> gameControllerManager) :
	_windowManager(windowManager),
	_gameControllerManager(gameControllerManager)
{}

std::shared_ptr<WindowManager> Platform::getWindowManager(void)
{
	return _windowManager;
}

std::shared_ptr<GameControllerManager> Platform::getGameControllerManager(void)
{
	return _gameControllerManager;
}