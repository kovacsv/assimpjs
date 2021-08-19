#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int MeaningOfLife ()
{
	Assimp::Importer importer;
	return 42;
}

bool ImportFile (const std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile (filePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	return true;
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
