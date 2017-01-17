#include "as/files/file.hpp"

#define ASMITH_OS_ERROR 0
#define ASMITH_OS_WINDOWS 1
#define ASMITH_OS_LINUX 2
#define ASMITH_OS_MAC 3
#define ASMITH_OS_BSD 4

#if defined(_WIN32) || defined(_WIN64)
	#define ASMITH_OS ASMITH_OS_WINDOWS

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#if defined(__APPLE__) || defined(__MACH__)
	//#define ASMITH_OS ASMITH_OS_MAC
#endif

#if defined(__linux__)
	//#define ASMITH_OS ASMITH_OS_LINUX
#endif

#if defined(__FreeBSD__)
	//#define ASMITH_OS ASMITH_OS_BSD
#endif

#ifndef ASMITH_OS
	#error as::file : This OS is not currently supported
#endif

namespace as {
	// file
	
	file::file() throw() :
		mPath(),
		mFlags(0)
	{}
	
	file::file(const char* aPath) throw()  :
		mPath(),
		mFlags(0)
	{
		strcpy(mPath, aPath);
		refresh();
	}
	
	file::~file() throw() {

	}
	
	void file::refresh() throw() {
		//! \todo Implement
	}
	
	bool file::create(uint8_t aFlags) throw() {
		if(exists()) return false;
		if(aFlags & FILE) {
#if ASMITH_OS == ASMITH_OS_WINDOWS
			if(CreateFileA(
				mPath, 
				(aFlags & READABLE ? GENERIC_READ : 0) | (aFlags & WRITABLE ? GENERIC_WRITE : 0),
				0, 
				NULL, 
				CREATE_ALWAYS,
				(aFlags & HIDDEN ? FILE_ATTRIBUTE_HIDDEN : FILE_ATTRIBUTE_NORMAL),
				NULL
			) == INVALID_HANDLE_VALUE) return false;
			refresh();
#endif
		} else if (aFlags & DIRECTORY) {
#if ASMITH_OS == ASMITH_OS_WINDOWS
			if(! CreateDirectoryA(
				mPath,
				NULL
			)) return false;
			refresh();
#endif
		}
		return exists();
	}
	
	bool file::destroy() throw() {
		if(! exists()) return false;
		if(mFlags & FILE) {
#if ASMITH_OS == ASMITH_OS_WINDOWS
			if(! DeleteFileA(mPath)) return false;
			refresh();
#endif
		}else if(mFlags & DIRECTORY) {
#if ASMITH_OS == ASMITH_OS_WINDOWS
			// Delete children
			auto children = get_children();
			for(file& i : children) if(! i.destroy()) return false;

			// Delete directory
			if(! RemoveDirectoryA(mPath)) return false;
			refresh();
#endif
		}
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
		if (mFlags & FILE) {
#if ASMITH_OS == ASMITH_OS_WINDOWS
			CopyFileA(mPath, aPath, FALSE);
#endif
		}else if(mFlags & DIRECTORY) {
#if ASMITH_OS == ASMITH_OS_WINDOWS
			CopyFileA(mPath, aPath, FALSE);
#endif
		}
		return file(aPath);
	}
	
	bool file::move(const char* aPath) throw() {
		if(! exists()) return false;
		//! \todo Implement
		return false;
	}
	
	bool file::exists() const throw() {
		return mFlags & EXISTS;
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
	
	file::operator bool() const throw() {
		return exists();
	}
	
}