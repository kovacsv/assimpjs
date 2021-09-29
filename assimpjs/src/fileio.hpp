#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include "filelist.hpp"

class FileIOStreamAdapter : public Assimp::IOStream
{
public:
	FileIOStreamAdapter (const File& file);
	virtual ~FileIOStreamAdapter ();

	virtual size_t		Read (void* pvBuffer, size_t pSize, size_t pCount) override;
	virtual size_t		Write (const void* pvBuffer, size_t pSize, size_t pCount) override;

	virtual aiReturn	Seek (size_t pOffset, aiOrigin pOrigin) override;
	virtual size_t		Tell () const override;

	virtual size_t		FileSize () const override;

	virtual void		Flush () override;

private:
	const File&			file;
	size_t				position;
};

class FileListIOSystemAdapter : public Assimp::IOSystem
{
public:
	FileListIOSystemAdapter (const FileList& fileList);
	virtual ~FileListIOSystemAdapter ();

	virtual bool				Exists (const char* pFile) const override;
	virtual Assimp::IOStream*	Open (const char* pFile, const char* pMode) override;
	virtual void				Close (Assimp::IOStream* pFile) override;

	virtual char				getOsSeparator () const override;

private:
	const FileList&				fileList;
};

class StringWriterIOStream : public Assimp::IOStream
{
public:
	StringWriterIOStream (std::string& resultString);
	virtual ~StringWriterIOStream ();

	virtual size_t		Read (void* pvBuffer, size_t pSize, size_t pCount) override;
	virtual size_t		Write (const void* pvBuffer, size_t pSize, size_t pCount) override;

	virtual aiReturn	Seek (size_t pOffset, aiOrigin pOrigin) override;
	virtual size_t		Tell () const override;

	virtual size_t		FileSize () const override;
	virtual void		Flush () override;

private:
	std::string&		resultString;
};

class StringWriterIOSystem : public Assimp::IOSystem
{
public:
	StringWriterIOSystem (std::string& resultString);
	virtual ~StringWriterIOSystem ();

	virtual bool				Exists (const char* pFile) const override;
	virtual Assimp::IOStream*	Open (const char* pFile, const char* pMode) override;
	virtual void				Close (Assimp::IOStream* pFile) override;

	virtual char				getOsSeparator () const override;

private:
	std::string&				resultString;
};

#endif
