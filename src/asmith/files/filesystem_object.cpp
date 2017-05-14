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

#include "asmith/files/filesystem_object.hpp"
#include <map>
#include "asmith/files/file.hpp"
#include "asmith/files/directory.hpp"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace asmith {
	std::map<std::string,std::shared_ptr<filesystem_object>> FILE_MAP;
	std::mutex FILE_MAP_LOCK;

	// filesystem_object

	std::shared_ptr<filesystem_object> filesystem_object::get_object_reference(const std::string& aPath, const bool aDirectory) {
		FILE_MAP_LOCK.lock();
		auto i = FILE_MAP.find(aPath);
		if(i == FILE_MAP.end()) i = FILE_MAP.emplace(aPath, std::shared_ptr<filesystem_object>(
			aDirectory ? static_cast<filesystem_object*>(new directory(aPath.c_str())) : 
			static_cast<filesystem_object*>(new file(aPath.c_str()))
		)).first;
		FILE_MAP_LOCK.unlock();
		return i->second;
	}
	
	
	filesystem_object::filesystem_object() :
		mPath(),
		mLock(),
		mFlags(0)
	{}
	
	filesystem_object::filesystem_object(const char* aPath) :
		mPath(aPath),
		mLock(),
		mFlags(0)
	{}
	
	filesystem_object::~filesystem_object() {
		
	}
	
	filesystem_object::operator bool() const throw() {
		return exists();
	}
	
	const char* filesystem_object::get_name() const throw() {
		size_t pos = mPath.find_last_of(FILE_SEPERATOR);
		if(pos == std::string::npos) return mPath.c_str();
		//! \todo Handle directory names
		return mPath.c_str() + pos + 1;
	}
	
	const char* filesystem_object::get_path() const throw() {
		return mPath.c_str();
	}

	std::shared_ptr<filesystem_object> filesystem_object::get_parent() const {
		size_t pos = mPath.find_last_of(FILE_SEPERATOR);
		if (pos == std::string::npos) throw("asmith::filesystem_object::get_parent : Directory is root");
		//! \todo Handle directory names
		return directory::get_reference(mPath.substr(0, pos).c_str());
	}
	
	bool filesystem_object::exists() const throw() {
		return mFlags & FILE_EXISTS;
	}
	
	bool filesystem_object::is_hidden() const throw() {
		return mFlags & FILE_HIDDEN;;
	}
	
	bool filesystem_object::is_temporary() const throw() {
		return mFlags & FILE_TEMPORARY;
	}
	
	bool filesystem_object::is_readable() const throw() {
		return mFlags & FILE_READ;
	}
	
	bool filesystem_object::is_writeable() const throw() {
		return mFlags & FILE_WRITE;
	}
	
	bool filesystem_object::is_read_only() const throw() {
		return is_readable() && ! is_writeable();
	}
	
	bool filesystem_object::is_write_only() const throw() {
		return is_writeable() && ! is_readable();;
	}
}