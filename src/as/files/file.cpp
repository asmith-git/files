#include "as/files/file.hpp"

namespace as {
	// file
	
	file::file() throw() :
		mPath(),
		mFlags(0)
	{}
	
	file::file(const char* aPath) throw()  :
		mPath(aPath),
		mFlags(0)
	{
		refresh();
	}
	
	file::~file() throw() :
		mPath(),
		mFlags(0)
	{}
	
	void file::refresh() throw() {
		//! \todo Implement
	}
	
	bool file::create(uint8_t aFlags) throw() {
		if(exists()) return false;
		//! \todo Implement
		return false;
	}
	
	bool file::destroy() throw() {
		if(! exists()) return false;
		//! \todo Implement
		return false;
	}
	
	bool file::hide() throw() {
		if(! exists()) return false;
		//! \todo Implement
		return false;
	}
	
	bool file::show() throw() {
		if(! exists()) return false;
		//! \todo Implement
		return false;
	}
	
	file file::copy(const char* aPath) const throw() {
		if(! exists()) return false;
		//! \todo Implement
		return false;
	}
	
	bool file::move(const char* aPath) throw() {
		if(! exists()) return false;
		//! \todo Implement
		return false;
	}
	
	bool file::exists() const throw() {
		return mFlags & EXITS;
	}
	
	bool file::is_file() const throw() {
		return mFlags & FILE;
	}
	
	bool file::is_directory() const throw() {
		return mFlags & DIRECTORY;
	}
	
	bool file::is_hidden() const throw() {
		return mFlags & HIDDEN;
	}
	
	bool file::is_readable() const throw() {
		return mFlags & READABLE;
	}
	
	bool file::is_writable() const throw() {
		return mFlags & WRITABLE;
	}
	
	bool file::is_read_only() const throw() {
		return (mFlags & READABLE) && ! (mFlags & WRITABLE);
	}
	
	bool file::is_write_only() const throw() {
		return (mFlags & WRITABLE) && ! (mFlags & READABLE);
	}
	
	size_t file::size() const throw() {
		//! \todo Implement
		return mFlags & EXISTS ? 0 : 0;
	}
	
	std::vector<file> file::get_children() const throw() {
		//! \todo Implement
		return std::vector<file>();
	}
	
	const char* file::get_name() const throw() {
		return mPath;
	}
	
	operator file::bool() const throw() {
		return exists();
	}
	
}