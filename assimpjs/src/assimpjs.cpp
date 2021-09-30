#include "assimpjs.hpp"

#include "fileio.hpp"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdio.h>
#include <iostream>

static const aiScene* ImportFileListByMainFile (Assimp::Importer& importer, const File* file)
{
	try {
		const aiScene* scene = importer.ReadFile (file->path,
			aiProcess_Triangulate |
			aiProcess_GenUVCoords |
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

static std::string CreateResultJson (const aiScene* scene)
{
	if (scene == nullptr) {
		return CreateErrorJson ("model_import_failed");
	}

	Assimp::Exporter exporter;

	std::string resultJson;
	StringWriterIOSystem* exportIOSystem = new StringWriterIOSystem (resultJson);
	exporter.SetIOHandler (exportIOSystem);

	Assimp::ExportProperties exportProperties;
	exportProperties.SetPropertyBool ("JSON_SKIP_WHITESPACES", true);
	exporter.Export (scene, "assjson", "result.json", 0u, &exportProperties);

	if (resultJson.empty ()) {
		return CreateErrorJson ("model_json_conversion_failed");
	}
	return resultJson;
}

std::string ImportFile (const File& file, const FileLoader& loader)
{
	Assimp::Importer importer;
	importer.SetIOHandler (new DelayLoadedIOSystemAdapter (file, loader));
	const aiScene* scene = ImportFileListByMainFile (importer, &file);
	return CreateResultJson (scene);
}

std::string ImportFileList (const FileList& fileList)
{
	if (fileList.FileCount () == 0) {
		return CreateErrorJson ("no_file_specified");
	}

	Assimp::Importer importer;
	importer.SetIOHandler (new FileListIOSystemAdapter (fileList));

	const aiScene* scene = nullptr;
	for (size_t fileIndex = 0; fileIndex < fileList.FileCount (); fileIndex++) {
		const File* file = fileList.GetFile (fileIndex);
		scene = ImportFileListByMainFile (importer, file);
		if (scene != nullptr) {
			break;
		}
	}

	return CreateResultJson (scene);
}

#ifdef EMSCRIPTEN

std::string ImportFileEmscripten (
	const std::string& name,
	const emscripten::val& content,
	const emscripten::val& existsFunc,
	const emscripten::val& loadFunc)
{
	class FileLoaderEmscripten : public FileLoader
	{
	public:
		FileLoaderEmscripten (const emscripten::val& existsFunc, const emscripten::val& loadFunc) :
			existsFunc (existsFunc),
			loadFunc (loadFunc)
		{
		}

		virtual bool Exists (const char* pFile) const override
		{
			if (existsFunc.isUndefined () || existsFunc.isNull ()) {
				return false;
			}
			std::string fileName = GetFileName (pFile);
			emscripten::val exists = existsFunc (fileName);
			return exists.as<bool> ();
		}

		virtual Buffer Load (const char* pFile) const override
		{
			if (loadFunc.isUndefined () || loadFunc.isNull ()) {
				return {};
			}
			std::string fileName = GetFileName (pFile);
			emscripten::val fileBuffer = loadFunc (fileName);
			return emscripten::vecFromJSArray<std::uint8_t> (fileBuffer);
		}

	private:
		const emscripten::val& existsFunc;
		const emscripten::val& loadFunc;
	};

	Buffer buffer = emscripten::vecFromJSArray<std::uint8_t> (content);
	File file (name, buffer);
	FileLoaderEmscripten loader (existsFunc, loadFunc);
	return ImportFile (file, loader);
}

EMSCRIPTEN_BINDINGS (assimpjs)
{
	emscripten::class_<FileList> ("FileList")
		.constructor<> ()
		.function ("AddFile", &FileList::AddFileEmscripten)
	;

	emscripten::function<std::string, const std::string&, const emscripten::val&, const emscripten::val&, const emscripten::val&> ("ImportFile", &ImportFileEmscripten);
	emscripten::function<std::string, const FileList&> ("ImportFileList", &ImportFileList);
}

#endif
