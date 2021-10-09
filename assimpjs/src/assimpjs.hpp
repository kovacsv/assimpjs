#ifndef ASSIMPJS_HPP
#define ASSIMPJS_HPP

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#endif

#include "filelist.hpp"
#include "fileio.hpp"
#include "result.hpp"

#include <vector>
#include <string>

Result ConvertFile (const File& file, const FileLoader& loader);
Result ConvertFileList (const FileList& fileList);

#endif
