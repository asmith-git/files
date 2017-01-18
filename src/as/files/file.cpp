
// Copyright 2017 Adam Smith
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
#if ASMITH_OS == ASMITH_OS_WINDOWS
		mFlags = 0;
		const DWORD flags = GetFileAttributesA(mPath);
		if(flags == INVALID_FILE_ATTRIBUTES) return;

		mFlags |= flags & FILE_ATTRIBUTE_READONLY ? READABLE : (WRITABLE | READABLE);
		mFlags |= flags & FILE_ATTRIBUTE_HIDDEN ? HIDDEN : 0;
		mFlags |= flags & FILE_ATTRIBUTE_DIRECTORY ? DIRECTORY : FILE;
		mFlags |= EXISTS;
		//! \todo Temporary
#endif
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

	void file::make_temporary() throw() {
		//! \todo Implement
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
		if (mFlags & FILE) {
#if ASMITH_OS == ASMITH_OS_WINDOWS
			if(! MoveFileA(mPath, aPath)) return false;
			refresh();
#endif
		}else if(mFlags & DIRECTORY) {
#if ASMITH_OS == ASMITH_OS_WINDOWS
			if(! MoveFileA(mPath, aPath)) return false;
			refresh();
#endif
		}
		return exists();
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

	bool file::is_temporary() const throw() {
		return mFlags & TEMPORARY;
	}
	
	size_t file::size() const throw() {
		if(!(mFlags & (EXISTS | FILE))) return 0;
		size_t size = 0;
#if ASMITH_OS == ASMITH_OS_WINDOWS
		// Open file handle
		const HANDLE handle = CreateFileA(
			mPath,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if(handle != INVALID_HANDLE_VALUE) {
			// Get size
			DWORD s = 0;
			GetFileSize(handle, &s);
			size = s;

			// Close file handle
			CloseHandle(handle);
		}
#endif
		return size;
	}
	
	std::vector<file> file::get_children() const throw() {
		if (!(mFlags & (EXISTS | DIRECTORY))) return std::vector<file>();
		std::vector<file> children;
#if ASMITH_OS == ASMITH_OS_WINDOWS
		WIN32_FIND_DATAA ffd;
		CHAR szDir[MAX_PATH];
		HANDLE handle = INVALID_HANDLE_VALUE;

		//StringCchCopyA(szDir, MAX_PATH, mPath);
		//StringCchCatA(szDir, MAX_PATH, TEXT("\\*"));

		handle = FindFirstFileA(szDir, &ffd);
		if(handle != INVALID_HANDLE_VALUE) {
			do {
				children.push_back(file(ffd.cFileName));
			} while (FindNextFileA(handle, &ffd) != 0);

		}
		FindClose(handle);
#endif
		return children;
	}
	
	const char* file::get_name() const throw() {
		return mPath;
	}
	
	file::operator bool() const throw() {
		return exists();
	}
	
}