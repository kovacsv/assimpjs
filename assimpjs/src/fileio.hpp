#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include "filelist.hpp"

class FileLoader
{
public:
	FileLoader ();
	virtual ~FileLoader ();

	virtual bool	Exists (const char* pFile) const = 0;
	virtual Buffer	Load (const char* pFile) const = 0;
};

class BufferIOStreamAdapter : public Assimp::IOStream
{
public:
	BufferIOStreamAdapter (const Buffer* buffer);
	virtual ~BufferIOStreamAdapter ();

	virtual size_t		Read (void* pvBuffer, size_t pSize, size_t pCount) override;
	virtual size_t		Write (const void* pvBuffer, size_t pSize, size_t pCount) override;

	virtual aiReturn	Seek (size_t pOffset, aiOrigin pOrigin) override;
	virtual size_t		Tell () const override;

	virtual size_t		FileSize () const override;
	virtual void		Flush () override;

protected:
	const Buffer*		buffer;
	size_t				position;
};

class OwnerBufferIOStreamAdapter : public BufferIOStreamAdapter
{
public:
	OwnerBufferIOStreamAdapter (const Buffer* buffer);
	virtual ~OwnerBufferIOStreamAdapter ();
};

class DelayLoadedIOSystemAdapter : public Assimp::IOSystem
{
public:
	DelayLoadedIOSystemAdapter (const File& file, const FileLoader& loader);
	virtual ~DelayLoadedIOSystemAdapter ();

	virtual bool				Exists (const char* pFile) const override;
	virtual Assimp::IOStream*	Open (const char* pFile, const char* pMode) override;
	virtual void				Close (Assimp::IOStream* pFile) override;

	virtual char				getOsSeparator () const override;

private:
	const File&					file;
	const FileLoader&			loader;
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
