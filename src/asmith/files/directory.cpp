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
	// directory

	std::shared_ptr<directory> directory::get_reference(const char* aPath) {
		return std::dynamic_pointer_cast<directory>(filesystem_object::get_object_reference(aPath, true));
	}

	std::shared_ptr<directory> directory::create_temporary(const char* aName) {
		std::string path = filesystem_object::get_temporary_directory();
		path += aName;
		std::shared_ptr<directory> ref = std::dynamic_pointer_cast<directory>(get_object_reference(path, true));
		if(! ref->exists()) ref->create(FILE_READ | FILE_WRITE | FILE_TEMPORARY);
		return ref;
	}

	directory::directory() :
		filesystem_object()
	{}

	directory::directory(const char* aPath) :
		filesystem_object(aPath)
	{}

	directory::~directory() {
		
	}

	uint32_t directory::get_flags() const {
		//! \todo Implement
		return 0;
	}

	std::vector<std::shared_ptr<filesystem_object>> directory::get_children() const {
		std::vector<std::shared_ptr<filesystem_object>> children;
		return children;
	}

	void directory::hide() {
		//! \todo Implement
		
	}

	void directory::show() {
		//! \todo Implement
		
	}

	void directory::create(const uint32_t aFlags) {
		//! \todo Implement
		
	}

	void directory::destroy() {
		//! \todo Implement
		
	}

	std::shared_ptr<filesystem_object> directory::move(const char* aPath) {
		//! \todo Implement
		return nullptr;
	}

	std::shared_ptr<filesystem_object> directory::copy(const char* aPath) {
		//! \todo Implement
		return nullptr;
	}

	bool directory::is_directory() const {
		return true;
	}

	bool directory::is_file() const {
		return false;
	}

}