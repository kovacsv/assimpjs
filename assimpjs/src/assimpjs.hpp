#ifndef ASSIMPJS_HPP
#define ASSIMPJS_HPP

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#endif

#include "filelist.hpp"

#include <vector>
#include <string>

std::string ImportModel (const FileList& fileList);

#endif
