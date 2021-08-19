#include <emscripten/bind.h>
#include <assimp/Importer.hpp>

int MeaningOfLife ()
{
	Assimp::Importer importer;
	return 42;
}

EMSCRIPTEN_BINDINGS (assimpjs) {

emscripten::function<int> ("MeaningOfLife", &MeaningOfLife);

}
