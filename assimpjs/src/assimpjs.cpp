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

static std::string ToLowercase (const std::string& str)
{
	std::string res = str;
	for (char& c : res) {
		c = std::tolower (c);
	}
	return res;
}

static std::string GetFileName (const std::string& path)
{
	size_t lastSeparator = path.find_last_of ('/');
	if (lastSeparator == std::wstring::npos) {
		lastSeparator = path.find_last_of ('\\');
	}
	if (lastSeparator == std::wstring::npos) {
		return ToLowercase (path);
	}
	std::string fileName = path.substr (lastSeparator + 1, path.length () - lastSeparator - 1);
	return ToLowercase (fileName);
}

File::File () :
	path (),
	content ()
{
}

File::File (const std::string& path, const std::vector<std::uint8_t>& content) :
	path (path),
	content (content)
{
}

bool File::IsValid () const
{
	return !path.empty () && !content.empty ();
}

FileList::FileList () :
	files ()
{
}

void FileList::AddFile (const std::string& path, const std::vector<std::uint8_t>& content)
{
	files.push_back (File (path, content));
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
	std::string fileName = GetFileName (path);
	for (const File& file : files) {
		std::string currFileName = GetFileName (file.path);
		if (currFileName == fileName) {
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
		size_t remainingElemCount = (size_t) (std::floor ((FileSize () - position) / pSize));
		size_t readableElemCount = std::min (remainingElemCount, pCount);
		if (readableElemCount == 0) {
			return 0;
		}
		memcpy (pvBuffer, &file.content[position], readableElemCount * pSize);
		position += readableElemCount * pSize;
		return readableElemCount;
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
#ifndef _WIN32
		return '/';
#else
		return '\\';
#endif
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
	ExportIOStream (std::string& resultJson) :
		resultJson (resultJson)
	{
	}

	virtual ~ExportIOStream ()
	{

	}

	virtual size_t Read (void* pvBuffer, size_t pSize, size_t pCount) override
	{
		throw std::logic_error ("not implemented");
	}

	virtual size_t Write (const void* pvBuffer, size_t pSize, size_t pCount) override
	{
		size_t memSize = pSize * pCount;
		resultJson.append ((char*) pvBuffer, memSize);
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
	std::string& resultJson;
};

class ExportIOSystem : public Assimp::IOSystem
{
public:
	ExportIOSystem (std::string& resultJson) :
		resultJson (resultJson)
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
#ifndef _WIN32
		return '/';
#else
		return '\\';
#endif
	}

	virtual Assimp::IOStream* Open (const char* pFile, const char* pMode) override
	{
		if (std::string (pFile) != "result.json") {
			throw std::logic_error ("invalid export file");
		}
		return new ExportIOStream (resultJson);
	}

	virtual void Close (Assimp::IOStream* pFile) override
	{
		delete pFile;
	}

private:
	std::string& resultJson;
};

static const aiScene* ImportModelByMainFile (Assimp::Importer& importer, const File* file)
{
	try {
		const aiScene* scene = importer.ReadFile (file->path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);
		return scene;
	} catch (...) {
		return nullptr;
	}
	return nullptr;
}

static std::string CreateErrorJson (const std::string& errorCode)
{
	return "{\"error\":\"" + errorCode + "\"}";
}

std::string ImportModel (const FileList& fileList)
{
	if (fileList.FileCount () == 0) {
		return CreateErrorJson ("no_file_specified");
	}

	Assimp::Importer importer;
	importer.SetIOHandler (new ImportIOSystem (fileList));

	const aiScene* scene = nullptr;
	for (size_t fileIndex = 0; fileIndex < fileList.FileCount (); fileIndex++) {
		const File* file = fileList.GetFile (fileIndex);
		scene = ImportModelByMainFile (importer, file);
		if (scene != nullptr) {
			break;
		}
	}

	if (scene == nullptr) {
		return CreateErrorJson ("model_import_failed");
	}

	Assimp::Exporter exporter;

	std::string resultJson;
	ExportIOSystem* exportIOSystem = new ExportIOSystem (resultJson);
	exporter.SetIOHandler (exportIOSystem);

	Assimp::ExportProperties exportProperties;
	exportProperties.SetPropertyBool ("JSON_SKIP_WHITESPACES", true);
	exporter.Export (scene, "assjson", "result.json", 0u, &exportProperties);

	if (resultJson.empty ()) {
		return CreateErrorJson ("model_json_conversion_failed");
	}
	return resultJson;
}

#ifdef EMSCRIPTEN

EMSCRIPTEN_BINDINGS (assimpjs)
{
	emscripten::class_<FileList> ("FileList")
		.constructor<> ()
		.function ("AddFile", &FileList::AddFileEmscripten)
	;

	emscripten::function<std::string, const FileList&> ("ImportModel", &ImportModel);
}

#endif
