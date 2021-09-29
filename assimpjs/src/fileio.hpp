#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include "filelist.hpp"

class ImportIOStream : public Assimp::IOStream
{
public:
	ImportIOStream (const File& file);
	virtual ~ImportIOStream ();

	virtual size_t		Read (void* pvBuffer, size_t pSize, size_t pCount) override;
	virtual size_t		Write (const void* pvBuffer, size_t pSize, size_t pCount) override;

	virtual aiReturn	Seek (size_t pOffset, aiOrigin pOrigin) override;
	virtual size_t		Tell () const override;

	virtual size_t		FileSize () const override;

	virtual void		Flush () override;

private:
	const File&		file;
	size_t			position;
};

class ImportIOSystem : public Assimp::IOSystem
{
public:
	ImportIOSystem (const FileList& fileList);
	virtual ~ImportIOSystem ();

	virtual bool				Exists (const char* pFile) const;

	virtual Assimp::IOStream*	Open (const char* pFile, const char* pMode) override;
	virtual void				Close (Assimp::IOStream* pFile) override;

	virtual char				getOsSeparator () const override;

private:
	const FileList& fileList;
};


class ExportIOStream : public Assimp::IOStream
{
public:
	ExportIOStream (std::string& resultJson);

	virtual ~ExportIOStream ();

	virtual size_t		Read (void* pvBuffer, size_t pSize, size_t pCount) override;
	virtual size_t		Write (const void* pvBuffer, size_t pSize, size_t pCount) override;

	virtual aiReturn	Seek (size_t pOffset, aiOrigin pOrigin) override;
	virtual size_t		Tell () const override;

	virtual size_t		FileSize () const override;
	virtual void		Flush () override;

private:
	std::string& resultJson;
};

class ExportIOSystem : public Assimp::IOSystem
{
public:
	ExportIOSystem (std::string& resultJson);
	virtual ~ExportIOSystem ();

	virtual bool				Exists (const char* pFile) const override;
	virtual Assimp::IOStream*	Open (const char* pFile, const char* pMode) override;
	virtual void				Close (Assimp::IOStream* pFile) override;

	virtual char				getOsSeparator () const override;

private:
	std::string& resultJson;
};

#endif
