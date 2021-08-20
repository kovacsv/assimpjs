#ifndef ASSIMPJS_HPP
#define ASSIMPJS_HPP

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#endif

#include <vector>
#include <string>

class File
{
public:
	std::string					path;
	std::vector<std::uint8_t>	content;
};

class FileList
{
public:
	FileList ();

	void			AddFile (const std::string& path, const std::vector<std::uint8_t>& content);
	size_t			FileCount () const;
	const File*		GetFile (size_t index) const;
	const File*		GetFile (const std::string& path) const;

#ifdef EMSCRIPTEN
	void			AddFileEmscripten (const std::string& path, const emscripten::val& content);
#endif


private:
	std::vector<File>	files;
};

std::string ImportFile (const FileList& fileList);

#endif
