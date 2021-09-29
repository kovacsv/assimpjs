#include "fileio.hpp"

#include <stdexcept>

static char GetOsSeparator ()
{
#ifndef _WIN32
	return '/';
#else
	return '\\';
#endif
}


ImportIOStream::ImportIOStream (const File& file) :
	file (file),
	position (0)
{
}

ImportIOStream::~ImportIOStream ()
{
}

size_t ImportIOStream::Read (void* pvBuffer, size_t pSize, size_t pCount)
{
	size_t remainingElemCount = (size_t) (std::floor ((FileSize () - position) / pSize));
	size_t readableElemCount = std::min (remainingElemCount, pCount);
	if (readableElemCount == 0) {
		return 0;
	}
	memcpy (pvBuffer, &file.content[position], readableElemCount * pSize);
	position += readableElemCount * pSize;
	return readableElemCount;
}

size_t ImportIOStream::Write (const void* pvBuffer, size_t pSize, size_t pCount)
{
	throw std::logic_error ("not implemented");
}

aiReturn ImportIOStream::Seek (size_t pOffset, aiOrigin pOrigin)
{
	switch (pOrigin) {
	case aiOrigin_SET:
		position = pOffset;
		break;
	case aiOrigin_CUR:
		position += pOffset;
		break;
	case aiOrigin_END:
		position = file.content.size () - pOffset;
		break;
	default:
		break;
	}
	return aiReturn::aiReturn_SUCCESS;
}

size_t ImportIOStream::Tell () const
{
	return position;
}

size_t ImportIOStream::FileSize () const
{
	return file.content.size ();
}

void ImportIOStream::Flush ()
{

}

ImportIOSystem::ImportIOSystem (const FileList& fileList) :
	fileList (fileList)
{
}

ImportIOSystem::~ImportIOSystem ()
{

}

bool ImportIOSystem::Exists (const char* pFile) const
{
	return fileList.GetFile (pFile) != nullptr;
}

Assimp::IOStream* ImportIOSystem::Open (const char* pFile, const char* pMode)
{
	const File* foundFile = fileList.GetFile (pFile);
	if (foundFile == nullptr) {
		return nullptr;
	}
	return new ImportIOStream (*foundFile);
}

void ImportIOSystem::Close (Assimp::IOStream* pFile)
{
	delete pFile;
}

char ImportIOSystem::getOsSeparator () const
{
	return GetOsSeparator ();
}

ExportIOStream::ExportIOStream (std::string& resultJson) :
	resultJson (resultJson)
{
}

ExportIOStream::~ExportIOStream ()
{

}

size_t ExportIOStream::Read (void* pvBuffer, size_t pSize, size_t pCount)
{
	throw std::logic_error ("not implemented");
}

size_t ExportIOStream::Write (const void* pvBuffer, size_t pSize, size_t pCount)
{
	size_t memSize = pSize * pCount;
	resultJson.append ((char*) pvBuffer, memSize);
	return memSize;
}

aiReturn ExportIOStream::Seek (size_t pOffset, aiOrigin pOrigin)
{
	throw std::logic_error ("not implemented");
}

size_t ExportIOStream::Tell () const
{
	throw std::logic_error ("not implemented");
}

size_t ExportIOStream::FileSize () const
{
	throw std::logic_error ("not implemented");
}

void ExportIOStream::Flush ()
{

}

ExportIOSystem::ExportIOSystem (std::string& resultJson) :
	resultJson (resultJson)
{
}

ExportIOSystem::~ExportIOSystem ()
{

}

bool ExportIOSystem::Exists (const char* pFile) const
{
	return false;
}

Assimp::IOStream* ExportIOSystem::Open (const char* pFile, const char* pMode)
{
	if (std::string (pFile) != "result.json") {
		throw std::logic_error ("invalid export file");
	}
	return new ExportIOStream (resultJson);
}

void ExportIOSystem::Close (Assimp::IOStream* pFile)
{
	delete pFile;
}

char ExportIOSystem::getOsSeparator () const
{
	return GetOsSeparator ();
}
