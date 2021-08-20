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
	File ();
	File (const std::string& path, const std::vector<std::uint8_t>& content);

	bool	IsValid () const;

	std::string					path;
	std::vector<std::uint8_t>	content;
};

class FileList
{
public:
	FileList ();

	void			SetPrimaryFile (const std::string& path, const std::vector<std::uint8_t>& content);
	void			AddSecondaryFile (const std::string& path, const std::vector<std::uint8_t>& content);
	
	const File*		GetPrimaryFile () const;
	const File*		GetFile (const std::string& path) const;

#ifdef EMSCRIPTEN
	void			SetPrimaryFileEmscripten (const std::string& path, const emscripten::val& content);
	void			AddSecondaryFileEmscripten (const std::string& path, const emscripten::val& content);
#endif


private:
	File				primaryFile;
	std::vector<File>	files;
};

std::string ImportFile (const FileList& fileList);

#endif
