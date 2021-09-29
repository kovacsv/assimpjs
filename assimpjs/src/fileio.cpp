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

BufferIOStreamAdapter::BufferIOStreamAdapter (const Buffer& buffer) :
	buffer (buffer),
	position (0)
{
}

BufferIOStreamAdapter::~BufferIOStreamAdapter ()
{
}

size_t BufferIOStreamAdapter::Read (void* pvBuffer, size_t pSize, size_t pCount)
{
	size_t remainingElemCount = (size_t) (std::floor ((FileSize () - position) / pSize));
	size_t readableElemCount = std::min (remainingElemCount, pCount);
	if (readableElemCount == 0) {
		return 0;
	}
	memcpy (pvBuffer, &buffer[position], readableElemCount * pSize);
	position += readableElemCount * pSize;
	return readableElemCount;
}

size_t BufferIOStreamAdapter::Write (const void* pvBuffer, size_t pSize, size_t pCount)
{
	throw std::logic_error ("not implemented");
}

aiReturn BufferIOStreamAdapter::Seek (size_t pOffset, aiOrigin pOrigin)
{
	switch (pOrigin) {
		case aiOrigin_SET:
			position = pOffset;
			break;
		case aiOrigin_CUR:
			position += pOffset;
			break;
		case aiOrigin_END:
			position = buffer.size () - pOffset;
			break;
		default:
			break;
	}
	return aiReturn::aiReturn_SUCCESS;
}

size_t BufferIOStreamAdapter::Tell () const
{
	return position;
}

size_t BufferIOStreamAdapter::FileSize () const
{
	return buffer.size ();
}

void BufferIOStreamAdapter::Flush ()
{

}

FileListIOSystemAdapter::FileListIOSystemAdapter (const FileList& fileList) :
	fileList (fileList)
{
}

FileListIOSystemAdapter::~FileListIOSystemAdapter ()
{

}

bool FileListIOSystemAdapter::Exists (const char* pFile) const
{
	return fileList.GetFile (pFile) != nullptr;
}

Assimp::IOStream* FileListIOSystemAdapter::Open (const char* pFile, const char* pMode)
{
	const File* foundFile = fileList.GetFile (pFile);
	if (foundFile == nullptr) {
		return nullptr;
	}
	return new BufferIOStreamAdapter (foundFile->content);
}

void FileListIOSystemAdapter::Close (Assimp::IOStream* pFile)
{
	delete pFile;
}

char FileListIOSystemAdapter::getOsSeparator () const
{
	return GetOsSeparator ();
}

StringWriterIOStream::StringWriterIOStream (std::string& resultString) :
	resultString (resultString)
{
}

StringWriterIOStream::~StringWriterIOStream ()
{

}

size_t StringWriterIOStream::Read (void* pvBuffer, size_t pSize, size_t pCount)
{
	throw std::logic_error ("not implemented");
}

size_t StringWriterIOStream::Write (const void* pvBuffer, size_t pSize, size_t pCount)
{
	size_t memSize = pSize * pCount;
	resultString.append ((char*) pvBuffer, memSize);
	return memSize;
}

aiReturn StringWriterIOStream::Seek (size_t pOffset, aiOrigin pOrigin)
{
	throw std::logic_error ("not implemented");
}

size_t StringWriterIOStream::Tell () const
{
	throw std::logic_error ("not implemented");
}

size_t StringWriterIOStream::FileSize () const
{
	throw std::logic_error ("not implemented");
}

void StringWriterIOStream::Flush ()
{

}

StringWriterIOSystem::StringWriterIOSystem (std::string& resultString) :
	resultString (resultString)
{
}

StringWriterIOSystem::~StringWriterIOSystem ()
{

}

bool StringWriterIOSystem::Exists (const char* pFile) const
{
	return false;
}

Assimp::IOStream* StringWriterIOSystem::Open (const char* pFile, const char* pMode)
{
	if (std::string (pFile) != "result.json") {
		throw std::logic_error ("invalid export file");
	}
	return new StringWriterIOStream (resultString);
}

void StringWriterIOSystem::Close (Assimp::IOStream* pFile)
{
	delete pFile;
}

char StringWriterIOSystem::getOsSeparator () const
{
	return GetOsSeparator ();
}
