#ifndef I_MODEL_HPP_INCLUDED
#define I_MODEL_HPP_INCLUDED

#include <SDL2/SDL_types.h>

class IModel
{
	public:
		virtual void elapse(Uint32 const gameTicks) = 0;
};

#endif // I_MODEL_HPP_INCLUDED
