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

#include "asmith/files/directory.hpp"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace asmith {
	std::string standardise_directory_path(const char* aPath) {
		std::string tmp = aPath;
		if(tmp.back() != FILE_SEPERATOR) tmp += FILE_SEPERATOR;
		return tmp;
	}

	// directory

	std::shared_ptr<directory> directory::get_temporary_directory() {
#ifdef _WIN32
		static char BUFFER[MAX_PATH];
		static bool ONCE = true;
		if(ONCE) {
			ONCE = false;
			if(GetTempPathA(MAX_PATH, BUFFER) > MAX_PATH) throw("asmith::directory::get_temporary_directory : Failed to locate temporary directory");
		}
		return get_reference(BUFFER);
#endif
		throw("asmith::directory::get_temporary_directory : Failed to locate temporary directory");
	}

	std::shared_ptr<directory> directory::get_current_directory() {
#ifdef _WIN32
		static char BUFFER[MAX_PATH];
		static bool ONCE = true;
		if(ONCE) {
			ONCE = false;
			if(GetModuleFileNameA(NULL, BUFFER, MAX_PATH) > MAX_PATH) throw("asmith::directory::get_current_directory : Failed to locate current directory");
			char* i = BUFFER;
			char* j = i;
			while(*i != '\0') {
				if(*i == FILE_SEPERATOR) j = i + 1;
				++i;
			}
			*j = '\0';
		}
		return get_reference(BUFFER);
#endif
		throw("asmith::directory::get_current_directory : Failed to locate current directory");
	}

	std::shared_ptr<directory> directory::get_reference(const char* aPath) {
		return std::dynamic_pointer_cast<directory>(filesystem_object::get_object_reference(aPath, true));
	}

	directory::directory() :
		filesystem_object()
	{}

	directory::directory(const char* aPath) :
		filesystem_object(standardise_directory_path(aPath).c_str())
	{
		mFlags = get_flags();
	}

	directory::~directory() {
		enum { DESTROY_FLAG = FILE_EXISTS | FILE_TEMPORARY};
		if((mFlags & DESTROY_FLAG) == DESTROY_FLAG) {
			destroy();
		}
	}

	uint32_t directory::get_flags() const {
		uint32_t flags = 0;
		if(is_temporary()) flags |= FILE_TEMPORARY;
#ifdef _WIN32
		const DWORD wflags = GetFileAttributesA(mPath.c_str());
		if(wflags == INVALID_FILE_ATTRIBUTES) return flags;
		if(!(wflags & FILE_ATTRIBUTE_DIRECTORY)) throw std::runtime_error("asmith::directory::get_flags : Object is a file not a directory");
		flags |= wflags & FILE_ATTRIBUTE_READONLY ? FILE_READ : (FILE_WRITE | FILE_READ);
		flags |= wflags & FILE_ATTRIBUTE_HIDDEN ? FILE_HIDDEN : 0;
		flags |= FILE_EXISTS;
#endif
		return flags;
	}

	std::shared_ptr<file> directory::get_file(const char* aPath) const {
		const std::string path = mPath + aPath;
		return file::get_reference(path.c_str());
	}

	std::shared_ptr<directory> directory::get_directory(const char* aPath) const {
		const std::string path = mPath + aPath;
		return directory::get_reference(path.c_str());
	}

	std::vector<std::shared_ptr<filesystem_object>> directory::get_children() const {
		std::vector<std::shared_ptr<filesystem_object>> children;
		if(! exists()) throw std::runtime_error("asmith::directory::get_children : Directory does not exist");
#ifdef _WIN32
		WIN32_FIND_DATAA ffd;
		const size_t size = mPath.size();
		char path[MAX_PATH];
		memcpy(path, mPath.c_str(), size);
		path[size] = '*';
		path[size + 1] = '\0';
		HANDLE handle = FindFirstFileA(path, &ffd);
		uint32_t c = 0;
		if(handle != INVALID_HANDLE_VALUE) {
			do {
				if(c > 1) children.push_back(get_reference(ffd.cFileName));
				++c;
			}while(FindNextFileA(handle, &ffd) != 0);
		}
		FindClose(handle);
#endif
		return children;
	}

	void directory::hide() {
		//! \todo Implement
		throw std::runtime_error("asmith::directory::hide : Failed to hide directory");
		
	}

	void directory::show() {
		//! \todo Implement
		throw std::runtime_error("asmith::directory::show : Failed to show directory");
		
	}

	void directory::create(const uint32_t aFlags) {
		if(exists()) throw std::runtime_error("asmith::directory::destroy : Directory already exists");
		std::lock_guard<std::mutex> lock(mLock);
#ifdef _WIN32
		if(! CreateDirectoryA(mPath.c_str(), NULL)) throw std::runtime_error("asmith::directory::create : Failed to create directory : " + std::to_string(GetLastError()));
		mFlags = aFlags | FILE_EXISTS;
		return;
#endif
		throw std::runtime_error("asmith::directory::create : Failed to create directory");
	}

	void directory::destroy() {
		if(! exists()) throw std::runtime_error("asmith::directory::destroy : Directory does not exist");
		std::lock_guard<std::mutex> lock(mLock);

		std::vector<std::shared_ptr<filesystem_object>> children = get_children();
		for(std::shared_ptr<filesystem_object>& i : children) i->destroy();

#ifdef _WIN32
		if(! RemoveDirectoryA(mPath.c_str())) throw std::runtime_error("asmith::directory::destroy : Failed to destroy directory : " + std::to_string(GetLastError()));
		mFlags = 0;
		return;
#endif
		throw std::runtime_error("asmith::directory::destroy : Failed to destroy directory");
		
	}

	std::shared_ptr<filesystem_object> directory::move(const char* aPath) {
		//! \todo Implement
		throw std::runtime_error("asmith::directory::move : Failed to move directory");
	}

	std::shared_ptr<filesystem_object> directory::copy(const char* aPath) {
		//! \todo Implement
		throw std::runtime_error("asmith::directory::copy : Failed to copy directory");
	}

	bool directory::is_directory() const throw() {
		return true;
	}

	bool directory::is_file() const throw() {
		return false;
	}

}