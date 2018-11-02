#include "GlobalHandler.hpp"
#include "Platform.hpp"
#include "KeyboardEventHandler.hpp"
#include "GameControllerEventHandler.hpp"
#include "WindowEventHandler.hpp"
#include <VBN/Logging.hpp>

GlobalHandler::GlobalHandler(
	std::shared_ptr<Platform> platform,
	std::shared_ptr<IEventHandler> mouse,
	std::shared_ptr<IEventHandler> keyboard,
	std::shared_ptr<IEventHandler> gameController,
	std::shared_ptr<IEventHandler> joystick,
	std::shared_ptr<IEventHandler> window) :
	EventHandler(mouse,
		std::shared_ptr<IEventHandler>(new KeyboardEventHandler(platform, keyboard)),
		std::shared_ptr<IEventHandler>(new GameControllerEventHandler(platform, gameController)),
		joystick,
		std::shared_ptr<IEventHandler>(new WindowEventHandler(platform, window)))
{}

GlobalHandler::~GlobalHandler(void)
{}