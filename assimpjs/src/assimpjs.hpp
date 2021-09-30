#ifndef ASSIMPJS_HPP
#define ASSIMPJS_HPP

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#endif

#include "filelist.hpp"
#include "fileio.hpp"

#include <vector>
#include <string>

std::string ImportFile (const File& file, const FileLoader& loader);
std::string ImportFileList (const FileList& fileList);

#endif
