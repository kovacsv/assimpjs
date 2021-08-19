#include "assimpjs.hpp"

#include <assimp/Importer.hpp>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static std::string GetFileName (const std::string& path)
{
	size_t lastSeparator = path.find_last_of ('/');
	if (lastSeparator == std::wstring::npos) {
		return path;
	}
	return path.substr (lastSeparator, path.length () - lastSeparator);
}

FileList::FileList () :
	files ()
{
}

void FileList::AddFile (const std::string& path, std::vector<char> content)
{
	files.push_back ({ path, content });
}

const File* FileList::GetFile (size_t index) const
{
	return &files[index];
}

const File* FileList::GetFile (const std::string& path) const
{
	std::string name = GetFileName (path);
	for (const File& file : files) {
		std::string fileName = GetFileName (file.path);
		if (file.path == path) {
			return &file;
		}
	}
	return nullptr;
}

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
		size_t memSize = pSize * pCount;
		memcpy_s (pvBuffer, memSize, &file.content[0], memSize);
		return memSize;
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
	JSIOSystem (const FileList& fileList) :
		fileList (fileList)
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
		const File* foundFile = fileList.GetFile (pFile);
		if (foundFile == nullptr) {
			return nullptr;
		}
		return new JSIOStream (*foundFile);
	}

	void Close (Assimp::IOStream* pFile)
	{
		delete pFile;
	}

private:
	const FileList& fileList;
};

int ImportFile (const FileList& fileList)
{
	Assimp::Importer importer;
	importer.SetIOHandler (new JSIOSystem (fileList));
	const aiScene* scene = importer.ReadFile (fileList.GetFile (0)->path,
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
