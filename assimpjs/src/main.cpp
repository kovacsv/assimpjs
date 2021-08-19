#include <assimp/Importer.hpp>

int MeaningOfLife ()
{
	Assimp::Importer importer;
	return 42;
}

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS (assimpjs) {

emscripten::function<int> ("MeaningOfLife", &MeaningOfLife);

}

#endif

int main ()
{
	return 0;
}
