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

	std::shared_ptr<file> file::create_temporary(const char* aName) {
		std::string path = filesystem_object::get_temporary_directory();
		path += aName;
		std::shared_ptr<file> ref = std::dynamic_pointer_cast<file>(get_object_reference(path, false));
		if(! ref->exists()) ref->create(FILE_READ | FILE_WRITE | FILE_TEMPORARY);
		return ref;
	}

	file::file() :
		filesystem_object()
	{}

	file::file(const char* aPath) :
		filesystem_object(aPath)
	{}

	file::~file() {
		
	}

	uint32_t file::get_flags() const {
		return 0;
	}

	const char* file::get_extension() const {
		return nullptr;
	}

	size_t file::size() const {
		if(! exists()) return 0;
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
		
	}

	void file::show() {
		
	}

	void file::create(const uint32_t aFlags) {
		
	}

	void file::destroy() {
		
	}

	std::shared_ptr<filesystem_object> file::move(const char* aPath) {
		return nullptr;
	}

	std::shared_ptr<filesystem_object> file::copy(const char* aPath) {
		return nullptr;
	}

	bool file::is_file() const {
		return true;
	}

	bool file::is_directory() const {
		return false;
	}

}