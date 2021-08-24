#include <assimp/DefaultIOSystem.h>
#include <assimp/IOStream.hpp>

#include "assimpjs.hpp"

static File GetFile (const std::string& filePath)
{
	Assimp::DefaultIOSystem system;
	Assimp::IOStream* stream = system.Open (filePath.c_str (), "rb");
	if (stream == nullptr) {
		return File ();
	}
	size_t fileSize = stream->FileSize ();
	std::vector<std::uint8_t> content (fileSize);
	stream->Read (&content[0], 1, fileSize);
	return File (filePath, content);
}

static File GetTestFile (const std::string& folderPath, const std::string& fileName)
{
	std::string filePath = folderPath + "\\testfiles\\" + fileName;
	Assimp::DefaultIOSystem system;
	Assimp::IOStream* stream = system.Open (filePath.c_str (), "rb");
	if (stream == nullptr) {
		return File ();
	}
	size_t fileSize = stream->FileSize ();
	std::vector<std::uint8_t> content (fileSize);
	stream->Read (&content[0], 1, fileSize);
	return File (fileName, content);
}

int main (int argc, const char* argv[])
{
	std::string folderPath = argv[0];
	size_t lastSeparator = folderPath.find_last_of ('\\');
	if (lastSeparator != std::string::npos) {
		folderPath = folderPath.substr (0, lastSeparator);
	}

	{
		FileList fileList;

		if (argc == 2) {
			File mainFile = GetFile (argv[1]);
			fileList.AddFile (mainFile.path, mainFile.content);
		} else {
			File mainFile = GetTestFile (folderPath, "cube_with_materials.obj");
			File mtlFile = GetTestFile (folderPath, "cube_with_materials.mtl");

			fileList.AddFile (mainFile.path, mainFile.content);
			fileList.AddFile (mtlFile.path, mtlFile.content);
		}

		ImportModel (fileList);
	}

	{
		FileList fileList;

		File mainFile = GetFile ("C:\\Users\\kovacsv\\GitRepos\\assimp\\test\\models\\STL\\Spider_binary.stl");
		fileList.AddFile (mainFile.path, mainFile.content);

		//File mainFile = GetTestFile (folderPath, "cube_with_materials.obj");
		//File mtlFile = GetTestFile (folderPath, "cube_with_materials.mtl");
		//
		//fileList.AddFile (mainFile.path, mainFile.content);
		//fileList.AddFile (mtlFile.path, mtlFile.content);

		ImportModel (fileList);
	}
	return 0;
}
