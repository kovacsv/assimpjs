#include "assimpjs.hpp"

int MeaningOfLife ()
{
	return 42;
}

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS (assimpjs)
{

	emscripten::function<int> ("MeaningOfLife", &MeaningOfLife);

}

#endif
