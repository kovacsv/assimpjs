#include "assimpjs.hpp"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdexcept>
#include <stdio.h>
#include <iostream>

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

void FileList::AddFile (const std::string& path, const std::vector<std::uint8_t>& content)
{
	files.push_back ({ path, content });
}

size_t FileList::FileCount () const
{
	return files.size ();
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

#ifdef EMSCRIPTEN
void FileList::AddFileEmscripten (const std::string& path, const emscripten::val& content)
{
	std::vector<std::uint8_t> contentArr = emscripten::vecFromJSArray<std::uint8_t> (content);
	AddFile (path, contentArr);
}
#endif

class ImportIOStream: public Assimp::IOStream
{
public:
	ImportIOStream (const File& file) :
		file (file),
		position (0)
	{
	}

	virtual ~ImportIOStream ()
	{
	}
	
	virtual size_t Read (void* pvBuffer, size_t pSize, size_t pCount) override
	{
		size_t memSize = pSize * pCount;
		size_t readableMemSize = std::min (FileSize () - position, memSize);
		if (readableMemSize == 0) {
			return 0;
		}
		memcpy (pvBuffer, &file.content[position], readableMemSize);
		position += readableMemSize;
		return readableMemSize;
	}

	virtual size_t Write (const void* pvBuffer, size_t pSize, size_t pCount) override
	{
		throw std::logic_error ("not implemented");
	}
	
	virtual aiReturn Seek (size_t pOffset, aiOrigin pOrigin) override
	{
		switch (pOrigin) {
			case aiOrigin_SET:
				position = pOffset;
				break;
			case aiOrigin_CUR:
				position += pOffset;
				break;
			case aiOrigin_END:
				position = file.content.size () - pOffset;
				break;
			default:
				break;
		}
		return aiReturn::aiReturn_SUCCESS;
	}

	virtual size_t Tell () const override
	{
		return position;
	}

	virtual size_t FileSize () const override
	{
		return file.content.size ();
	}

	virtual void Flush () override
	{

	}

private:
	const File&		file;
	size_t			position;
};

class ImportIOSystem : public Assimp::IOSystem
{
public:
	ImportIOSystem (const FileList& fileList) :
		fileList (fileList)
	{
	}

	virtual ~ImportIOSystem ()
	{
	
	}

	virtual bool Exists (const char* pFile) const override
	{
		return fileList.GetFile (pFile) != nullptr;
	}

	virtual char getOsSeparator () const override
	{
		return '/';
	}

	virtual Assimp::IOStream* Open (const char* pFile, const char* pMode) override
	{
		const File* foundFile = fileList.GetFile (pFile);
		if (foundFile == nullptr) {
			return nullptr;
		}
		return new ImportIOStream (*foundFile);
	}

	virtual void Close (Assimp::IOStream* pFile) override
	{
		delete pFile;
	}

private:
	const FileList& fileList;
};

class ExportIOStream : public Assimp::IOStream
{
public:
	ExportIOStream (std::vector<std::string>& resultFiles) :
		resultFiles (resultFiles),
		result ()
	{
	}

	virtual ~ExportIOStream ()
	{
		if (!result.empty ()) {
			resultFiles.push_back (result);
		}
	}

	virtual size_t Read (void* pvBuffer, size_t pSize, size_t pCount) override
	{
		throw std::logic_error ("not implemented");
	}

	virtual size_t Write (const void* pvBuffer, size_t pSize, size_t pCount) override
	{
		size_t memSize = pSize * pCount;
		result.append ((char*) pvBuffer, memSize);
		return memSize;
	}

	virtual aiReturn Seek (size_t pOffset, aiOrigin pOrigin) override
	{
		throw std::logic_error ("not implemented");
	}

	virtual size_t Tell () const override
	{
		throw std::logic_error ("not implemented");
	}

	virtual size_t FileSize () const override
	{
		throw std::logic_error ("not implemented");
	}

	virtual void Flush () override
	{

	}

private:
	std::vector<std::string>& resultFiles;
	std::string result;
};

class ExportIOSystem : public Assimp::IOSystem
{
public:
	ExportIOSystem () :
		resultFiles ()
	{
	}

	virtual ~ExportIOSystem ()
	{

	}

	virtual bool Exists (const char* pFile) const override
	{
		return false;
	}

	virtual char getOsSeparator () const override
	{
		return '/';
	}

	virtual Assimp::IOStream* Open (const char* pFile, const char* pMode) override
	{
		return new ExportIOStream (resultFiles);
	}

	virtual void Close (Assimp::IOStream* pFile) override
	{
		delete pFile;
	}

	const std::vector<std::string>& GetResultFiles () const
	{
		return resultFiles;
	}

private:
	std::vector<std::string> resultFiles;
};

std::string ImportFile (const FileList& fileList)
{
	if (fileList.FileCount () == 0) {
		return "error";
	}

	Assimp::Importer importer;
	importer.SetIOHandler (new ImportIOSystem (fileList));
	const aiScene* scene = importer.ReadFile (fileList.GetFile (0)->path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	if (scene == nullptr) {
		return "error";
	}

	Assimp::Exporter exporter;
	ExportIOSystem* exportIOSystem = new ExportIOSystem ();
	exporter.SetIOHandler (exportIOSystem);
	exporter.Export (scene, "assjson", "result.json");

	const std::vector<std::string>& resultFiles = exportIOSystem->GetResultFiles ();
	if (resultFiles.size () != 1) {
		return "error";
	}
	return resultFiles[0];
}

#ifdef EMSCRIPTEN

EMSCRIPTEN_BINDINGS (assimpjs)
{
	emscripten::class_<FileList> ("FileList")
		.constructor<> ()
		.function ("AddFile", &FileList::AddFileEmscripten)
	;

	emscripten::function<std::string, const FileList&> ("ImportFile", &ImportFile);
}

#endif
