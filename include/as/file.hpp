#ifndef ASMITH_FILE_HPP
#define ASMITH_FILE_HPP

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

#include <cstdint>
#include <vector>

namespace as {
	class file {
	public:
		enum : uint8_t {
			FILE			= 1 << 0,
			DIRECTORY		= 1 << 1,
			READABLE		= 1 << 2,
			WRITABLE		= 1 << 3,
			HIDDEN			= 1 << 4,
			READ_WRITEABLE	= READABLE | WRITABLE,
		};
		
		file() throw();
		file(const char*) throw();
		~file() throw();
		
		bool create() throw();
		bool destroy() throw();
		bool hide() throw();
		bool show() throw();
		file copy(const char*) const throw();
		bool move(const char*) throw();
		
		bool exists() const throw();
		bool is_file() const throw();
		bool is_directory() const throw();
		bool is_hidden() const throw();
		bool is_readable() const throw();
		bool is_writable() const throw();
		bool is_read_only() const throw();
		bool is_write_only() const throw();
		
		size_t size() const throw();
		std::vector<file> get_children() const throw();
		const char* get_name() const throw();
		
		operator bool() const throw();
	};
}

#endif