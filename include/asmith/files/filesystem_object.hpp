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

#ifndef ASMITH_FILES_FILESYSTEM_OBJECT_HPP
#define ASMITH_FILES_FILESYSTEM_OBJECT_HPP

#include <string>
#include <mutex>
#include <memory>

namespace asmith {
	enum : char {
		FILE_SEPERATOR = '\\'
	};

	enum {
		FILE_EXISTS		= 1 << 0,
		FILE_HIDDEN		= 1 << 1,
		FILE_READ		= 1 << 2,
		FILE_WRITE		= 1 << 3,
		FILE_TEMPORARY	= 1 << 4
	};
		
	class filesystem_object : public std::enable_shared_from_this<filesystem_object> {
	private:
		filesystem_object(filesystem_object&&) = delete;
		filesystem_object(const filesystem_object&) = delete;
		filesystem_object& operator=(filesystem_object&&) = delete;
		filesystem_object& operator=(const filesystem_object&) = delete;
	protected:
		const std::string mPath;
		mutable std::mutex mLock;
		uint32_t mFlags;
	protected:
		static std::shared_ptr<filesystem_object> get_object_reference(const std::string&, const bool);
		
		filesystem_object();
		filesystem_object(const char* aPath);
		
		virtual uint32_t get_flags() const = 0;
	public:
		static size_t max_path_length() throw();

		virtual ~filesystem_object();
		
		operator bool() const throw();
		
		const char* get_name() const throw();
		const char* get_path() const throw();
		std::shared_ptr<filesystem_object> get_parent() const;
		
		bool exists() const throw();
		bool is_hidden() const throw();
		bool is_temporary() const throw();
		bool is_readable() const throw();
		bool is_writeable() const throw();
		bool is_read_only() const throw();
		bool is_write_only() const throw();
		
		virtual void hide() = 0;
		virtual void show() = 0;
		virtual void create(const uint32_t aFlags) = 0;
		virtual void destroy() = 0;
		virtual std::shared_ptr<filesystem_object> move(const char* aPath) = 0;
		virtual std::shared_ptr<filesystem_object> copy(const char* aPath) = 0;
		virtual bool is_file() const throw() = 0;
		virtual bool is_directory() const throw() = 0;
	};
}
#endif
