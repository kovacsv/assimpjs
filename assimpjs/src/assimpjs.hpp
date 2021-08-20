#ifndef ASSIMPJS_HPP
#define ASSIMPJS_HPP

#include <vector>
#include <string>

class File
{
public:
	std::string			path;
	std::vector<char>	content;
};

class FileList
{
public:
	FileList ();

	void			AddFile (const std::string& path, const std::vector<char>& content);
	const File*		GetFile (size_t index) const;
	const File*		GetFile (const std::string& path) const;

private:
	std::vector<File>	files;
};

int MeaningOfLife ();

int ImportFile (const FileList& fileList);

#endif
