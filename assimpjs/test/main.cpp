#include "assimpjs.hpp"
#include <assimp/DefaultIOSystem.h>
#include <assimp/IOStream.hpp>

int main ()
{
	Assimp::DefaultIOSystem system;
	Assimp::IOStream* stream = system.Open ("C:\\Users\\kovacsv\\GitRepos\\assimpjs\\em_build\\Debug\\cube_four_instances.3ds", "rb");
	if (stream == nullptr) {
		return 1;
	}
	size_t fileSize = stream->FileSize ();
	std::vector<char> content (fileSize);
	stream->Read (&content[0], 1, fileSize);

	FileList fileList;
	fileList.AddFile ("cube_four_instances.3ds", content);
	int mol = ImportFile (fileList);
	return mol;
}
