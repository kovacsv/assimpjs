#include "assimpjs.hpp"

#include "fileio.hpp"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdio.h>
#include <iostream>

static const aiScene* ImportModelByMainFile (Assimp::Importer& importer, const File* file)
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

std::string ImportModel (const FileList& fileList)
{
	if (fileList.FileCount () == 0) {
		return CreateErrorJson ("no_file_specified");
	}

	Assimp::Importer importer;
	importer.SetIOHandler (new FileListIOSystemAdapter (fileList));

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
