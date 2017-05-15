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

#ifndef ASMITH_FILES_FILE_WRAPPER_HPP
#define ASMITH_FILES_FILE_WRAPPER_HPP

#include "filesystem_object.hpp"

namespace asmith {

	class file_wrapper {
	private:
		std::shared_ptr<filesystem_object> mObject;
	public:
		static inline size_t max_path_length() throw() { return filesystem_object::max_path_length(); }

		file_wrapper() : mObject() {}
		file_wrapper(std::shared_ptr<filesystem_object> aObject) : mObject(aObject) {}
		~file_wrapper() {}

		// Delegated to filesystem_object
		inline operator bool() const throw() { return mObject->exists(); }
		inline const char* get_name() const throw() { return mObject->get_name(); }
		inline const char* get_path() const throw() { return mObject->get_path(); }
		inline file_wrapper get_parent() const { return mObject->get_parent(); }
		inline bool exists() const throw() { return mObject->exists(); }
		inline bool is_hidden() const throw() { return mObject->is_hidden(); }
		inline bool is_temporary() const throw() { return mObject->is_temporary(); }
		inline bool is_readable() const throw() { return mObject->is_readable(); }
		inline bool is_writeable() const throw() { return mObject->is_writeable(); }
		inline bool is_read_only() const throw() { return mObject->is_read_only(); }
		inline bool is_write_only() const throw() { return mObject->is_write_only(); }
		inline void hide() { mObject->hide(); }
		inline void show() { mObject->show(); }
		inline void create(const uint32_t aFlags) { mObject->create(aFlags); }
		inline void destroy() { mObject->destroy(); }
		inline file_wrapper move(const char* aPath) { return mObject->move(aPath); }
		inline file_wrapper copy(const char* aPath) { return mObject->copy(aPath); }
		inline bool is_file() const throw() { return mObject->is_file(); }
		inline bool is_directory() const throw() { return mObject->is_directory(); }

		// Delegated to file
		inline const char* get_extension() const throw() { return is_file() ? static_cast<const file*>(mObject.get())->get_extension() : ""; }
		inline size_t size() const { return is_file() ? static_cast<const file*>(mObject.get())->size() : 0; }

		// Delegated to directory
		inline file_wrapper operator[](const char* aPath) const { is_directory() ? static_cast<const directory*>(mObject.get())->get_child(aPath) : file_wrapper(); }
		
		std::vector<file_wrapper> get_children() const {
			std::vector<file_wrapper> tmp;
			if(is_directory()) {
				const std::vector<std::shared_ptr<filesystem_object>> children = static_cast<const directory*>(mObject.get())->get_children();
				for(const std::shared_ptr<filesystem_object>& i : children) tmp.push_back(i);
			}
			return tmp;
		}
	};
}
#endif
