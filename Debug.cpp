#include "Debug.hpp"
#include "Platform.hpp"
#include <VBN/WindowManager.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/GameControllerManager.hpp>

Debug::View::View(std::shared_ptr<Platform> platform) :
	_platform(platform)
{}

void Debug::View::display(void)
{
	Window & mainWindow = _platform->getWindowManager()->getByName("mainWindow");
	SDL_Renderer * renderer(mainWindow.getRenderer());

	SDL_SetRenderDrawColor(renderer, 0, 0, 32, 255);
	SDL_RenderClear(renderer);

	SDL_GameController* contr(_platform->getGameControllerManager()->getFirstController());
	Sint16 leftx(SDL_GameControllerGetAxis(contr, SDL_CONTROLLER_AXIS_LEFTX)/256),
		lefty(SDL_GameControllerGetAxis(contr, SDL_CONTROLLER_AXIS_LEFTY)/256),
		rightx(SDL_GameControllerGetAxis(contr, SDL_CONTROLLER_AXIS_RIGHTX)/256),
		righty(SDL_GameControllerGetAxis(contr, SDL_CONTROLLER_AXIS_RIGHTY)/256),
		ltrigger(SDL_GameControllerGetAxis(contr, SDL_CONTROLLER_AXIS_TRIGGERLEFT)/256),
		rtrigger(SDL_GameControllerGetAxis(contr, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)/256);

	mainWindow.printText("DEBUG", "courier", 12, { 255, 255, 255, 255 }, {10, 10, 100, 22});

	SDL_Rect lxr{ 200, 199, leftx, 3 };
	SDL_Rect lyr{ 199, 200, 3, lefty };
	SDL_SetRenderDrawColor(renderer, 0, 194, 255, 255);
	SDL_RenderFillRect(renderer, &lxr);
	SDL_RenderFillRect(renderer, &lyr);
	SDL_RenderDrawLine(renderer, 195 + leftx, 200 + lefty, 205 + leftx, 200 + lefty);
	SDL_RenderDrawLine(renderer, 200 + leftx, 195 + lefty, 200 + leftx, 205 + lefty);

	SDL_Rect rxr{ 600, 199, rightx, 3 };
	SDL_Rect ryr{ 599, 200, 3, righty };
	SDL_RenderFillRect(renderer, &rxr);
	SDL_RenderFillRect(renderer, &ryr);
	SDL_RenderDrawLine(renderer, 595 + rightx, 200 + righty, 605 + rightx, 200 + righty);
	SDL_RenderDrawLine(renderer, 600 + rightx, 195 + righty, 600 + rightx, 205 + righty);

	SDL_Rect leftr{ 200, 400, 3, ltrigger };
	SDL_Rect rightr{ 600, 400, 3, rtrigger };
	SDL_RenderFillRect(renderer, &leftr);
	SDL_RenderFillRect(renderer, &rightr);
}

void Debug::KeyboardEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> response)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					response->popGameContext();
				break;
			}
		break;
	}
}

Debug::GameControllerEventHandler::GameControllerEventHandler(
	std::shared_ptr<Platform> platform) :
	_platform(platform)
{}

void Debug::GameControllerEventHandler::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> response)
{
	SDL_Joystick * joystick(nullptr);
	SDL_Haptic * haptic(nullptr);

	switch (event.type)
	{
		case SDL_CONTROLLERBUTTONDOWN:
			switch (event.cbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_START:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button START pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_BACK:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button BACK pressed on instance @%d",
						event.cbutton.which);
					joystick = SDL_JoystickFromInstanceID(event.cbutton.which);
					switch (SDL_JoystickCurrentPowerLevel(joystick))
					{
						case SDL_JOYSTICK_POWER_WIRED:
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Wired");
						break;
						case SDL_JOYSTICK_POWER_MAX:
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Max");
						break;
						case SDL_JOYSTICK_POWER_FULL:
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Full");
						break;
						case SDL_JOYSTICK_POWER_MEDIUM:
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Medium");
						break;
						case SDL_JOYSTICK_POWER_LOW:
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Low");
						break;
						case SDL_JOYSTICK_POWER_EMPTY:
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Empty");
						break;
						case SDL_JOYSTICK_POWER_UNKNOWN:
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Unknown");
						break;
					}
				break;
				case SDL_CONTROLLER_BUTTON_A:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button A pressed on instance @%d",
						event.cbutton.which);

					haptic = _platform
							->getGameControllerManager()
							->getHapticFromInstance(event.cbutton.which);

					if (haptic && SDL_HapticRumbleSupported(haptic))
					{
						if (SDL_HapticRumblePlay(haptic, 1, 2500))
							SDL_LogError(SDL_LOG_CATEGORY_ERROR,
								"Error while playing rumble: %s",
								SDL_GetError());
						else
							SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "RUMBLE");
					}
				break;
				case SDL_CONTROLLER_BUTTON_B:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button B pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_X:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button X pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_Y:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button Y pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button UP pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button DOWN pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button LEFT pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button RIGHT pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_GUIDE:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button GUIDE pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button LEFTSHOULDER pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button RIGHTSHOULDER pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_LEFTSTICK:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button LEFTSTICK pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button RIGHTSTICK pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_MAX:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button MAX pressed on instance @%d",
						event.cbutton.which);
				break;
				case SDL_CONTROLLER_BUTTON_INVALID:
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
						"Button INVALID pressed on instance @%d",
						event.cbutton.which);
				break;
			}
		break;
		case SDL_CONTROLLERBUTTONUP:
		break;
	}
}