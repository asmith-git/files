//	Copyright 2017 Adam Smith
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
// 
//	http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.

#include "asmith/files/file.hpp"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace asmith {
	// file

	std::shared_ptr<file> file::get_reference(const char* aPath) {
		return std::dynamic_pointer_cast<file>(filesystem_object::get_object_reference(aPath, false));
	}

	file::file() :
		filesystem_object()
	{}

	file::file(const char* aPath) :
		filesystem_object(aPath)
	{
		mFlags = get_flags();
	}

	file::~file() {
		if(is_temporary() && exists()) {
			destroy();
		}
	}

	uint32_t file::get_flags() const {
		uint32_t flags = 0;
		if(is_temporary()) flags |= FILE_TEMPORARY;
#ifdef _WIN32
		const DWORD wflags = GetFileAttributesA(mPath.c_str());
		if(wflags == INVALID_FILE_ATTRIBUTES) return flags;
		if(wflags & FILE_ATTRIBUTE_DIRECTORY) throw std::runtime_error("asmith::file::get_flags : Object is a directory not a file");
		flags |= wflags & FILE_ATTRIBUTE_READONLY ? FILE_READ : (FILE_WRITE | FILE_READ);
		flags |= wflags & FILE_ATTRIBUTE_HIDDEN ? FILE_HIDDEN : 0;
		flags |= FILE_EXISTS;
#endif
		return flags;
	}

	const char* file::get_extension() const {
		const size_t pos = mPath.find_last_of('.');
		if(pos == std::string::npos) return "";
		return mPath.c_str() + pos + 1;
	}

	size_t file::size() const {
		if(! exists()) throw std::runtime_error("asmith::file::size : File does not exist");
		size_t size = 0;
#ifdef _WIN32
		// Open file handle
		const HANDLE handle = CreateFileA(
			mPath.c_str(),
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

	void file::hide() {
		//! \todo Implement
		throw std::runtime_error("asmith::file::hide : Failed to hide file");
	}

	void file::show() {
		//! \todo Implement
		throw std::runtime_error("asmith::file::show : Failed to show file");
	}

	void file::create(const uint32_t aFlags) {
		if(exists()) throw std::runtime_error("asmith::file::destroy : File already exists");
		std::lock_guard<std::mutex> lock(mLock);
#ifdef _WIN32
		if(CreateFileA(
			mPath.c_str(),
			(aFlags & FILE_READ ? GENERIC_READ : 0) | (aFlags & FILE_WRITE ? GENERIC_WRITE : 0),
			0,
			NULL,
			CREATE_ALWAYS,
			(aFlags & FILE_HIDDEN ? FILE_ATTRIBUTE_HIDDEN : FILE_ATTRIBUTE_NORMAL),
			NULL
		) == INVALID_HANDLE_VALUE) throw std::runtime_error("asmith::file::create : Failed to create file");
		mFlags = aFlags;
#endif
		throw std::runtime_error("asmith::file::create : Failed to create file");
	}

	void file::destroy() {
		if(! exists()) throw std::runtime_error("asmith::file::destroy : File does not exist");
		std::lock_guard<std::mutex> lock(mLock);
#ifdef _WIN32
		if(! DeleteFileA(mPath.c_str())) throw std::runtime_error("asmith::file::destroy : Failed to destroy file");
		mFlags = 0;
		return;
#endif
		throw std::runtime_error("asmith::file::destroy : Failed to destroy file");
	}

	std::shared_ptr<filesystem_object> file::move(const char* aPath) {
		if(! exists()) throw std::runtime_error("asmith::file::move : File does not exist");
		std::lock_guard<std::mutex> lock(mLock);
#ifdef _WIN32
		if(! MoveFileA(mPath.c_str(), aPath)) throw std::runtime_error("asmith::file::move : Failed to move file");
		mFlags = get_flags();
		return get_reference(aPath);
#endif
		throw std::runtime_error("asmith::file::move : Failed to move file");
	}

	std::shared_ptr<filesystem_object> file::copy(const char* aPath) {
		if(! exists()) throw std::runtime_error("asmith::file::copy : File does not exist");
		std::lock_guard<std::mutex> lock(mLock);
#ifdef _WIN32
		if(! CopyFileA(mPath.c_str(), aPath, FALSE)) throw std::runtime_error("asmith::file::copy : Failed to copy file");
		return get_reference(aPath);
#endif
		throw std::runtime_error("asmith::file::copy : Failed to copy file");
	}

	bool file::is_file() const {
		return true;
	}

	bool file::is_directory() const {
		return false;
	}

}