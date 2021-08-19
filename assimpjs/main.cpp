#include <emscripten/bind.h>

int MeaningOfLife ()
{
	return 42;
}

EMSCRIPTEN_BINDINGS (assimpjs) {

emscripten::function<int> ("MeaningOfLife", &MeaningOfLife);

}
