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

namespace asmith {
	// filesystem_object

	const char* filesystem_object::get_temporary_directory() throw() {
		return "";
	}

	std::shared_ptr<filesystem_object> filesystem_object::get_object_reference(const char*) {
		return false;
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
	
	filesystem_object::operator bool() const {
		return exists();
	}
	
	const char* filesystem_object::get_name() const {
		return nullptr;
	}
	
	const char* filesystem_object::get_path() const {
		return nullptr;
	}
	
	bool filesystem_object::exists() const {
		return false;
	}
	
	bool filesystem_object::is_hidden() const {
		return false;
	}
	
	bool filesystem_object::is_temporary() const {
		return false;
	}
	
	bool filesystem_object::is_readable() const {
		return false;
	}
	
	bool filesystem_object::is_writeable() const {
		return false;
	}
	
	bool filesystem_object::is_read_only() const {
		return false;
	}
	
	bool filesystem_object::is_write_only() const {
		return false;
	}
}