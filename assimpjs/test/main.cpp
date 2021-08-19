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
	File file {
		"cube_four_instances.3ds",
		std::vector<char> (fileSize)
	};

	stream->Read (&file.content[0], 1, fileSize);
	int mol = ImportFile ({ file });
	return mol;
}
