#include "assimpjs.hpp"
#include <assimp/Importer.hpp>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int MeaningOfLife ()
{
	return 42;
}

class JSIOStream: public Assimp::IOStream
{
public:
	JSIOStream (const File& file) :
		file (file)
	{
	}

	~JSIOStream ()
	{
	}
	
	size_t Read (void* pvBuffer, size_t pSize, size_t pCount)
	{
		// TODO
		return 0;
	}

	size_t Write (const void* pvBuffer, size_t pSize, size_t pCount)
	{
		// TODO: exception
		return 0;
	}
	
	aiReturn Seek (size_t pOffset, aiOrigin pOrigin)
	{
		// TODO: exception
		return aiReturn::aiReturn_SUCCESS;
	}

	size_t Tell () const
	{
		return 0;
	}

	size_t FileSize () const
	{
		return file.content.size ();
	}

	void Flush ()
	{

	}

private:
	const File& file;
};

class JSIOSystem : public Assimp::IOSystem
{
public:
	JSIOSystem (const std::vector<File>& files) :
		files (files)
	{
	}

	~JSIOSystem ()
	{
	
	}

	bool Exists (const char* pFile) const
	{
		return true;
	}

	char getOsSeparator () const
	{
		return '/';
	}

	Assimp::IOStream* Open (const char* pFile, const char* pMode)
	{
		// TODO
		return new JSIOStream (files[0]);
	}

	void Close (Assimp::IOStream* pFile)
	{
		delete pFile;
	}

private:
	const std::vector<File>& files;
};

int ImportFile (const std::vector<File>& files)
{
	Assimp::Importer importer;
	importer.SetIOHandler (new JSIOSystem (files));
	const aiScene* scene = importer.ReadFile ("C:\\Users\\kovacsv\\GitRepos\\assimpjs\\em_build\\Debug\\cube_four_instances.3ds",
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	return 42;
}

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS (assimpjs)
{

	emscripten::function<int> ("MeaningOfLife", &MeaningOfLife);

}

#endif
