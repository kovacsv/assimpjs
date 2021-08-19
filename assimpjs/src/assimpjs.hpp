#ifndef ASSIMPJS_HPP
#define ASSIMPJS_HPP

#include <vector>
#include <string>

struct File
{
	std::string			name;
	std::vector<char>	content;
};

int MeaningOfLife ();

int ImportFile (const std::vector<File>& files);

#endif
