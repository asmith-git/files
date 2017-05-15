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

#ifndef ASMITH_FILES_FILE_HPP
#define ASMITH_FILES_FILE_HPP

#include "filesystem_object.hpp"

namespace asmith {
	class file : public filesystem_object {
	protected:
		friend filesystem_object;

		file();
		file(const char* aPath);
		
		// Inherited from filesystem_object
		uint32_t get_flags() const override;
	public:
		static std::shared_ptr<file> get_reference(const char*);
		
		~file();
		
		const char* get_extension() const throw();
		size_t size() const;
		
		// Inherited from filesystem_object
		
		void hide() override;
		void show() override;
		void create(const uint32_t aFlags) override;
		void destroy() override;
		std::shared_ptr<filesystem_object> move(const char* aPath) override;
		std::shared_ptr<filesystem_object> copy(const char* aPath) override;
		bool is_file() const throw() override;
		bool is_directory() const throw() override;
	};

#ifdef ASMITH_SERIAL_VALUE_HPP
	namespace serial {
		template<>
		struct serialiser<file> {
			typedef const std::shared_ptr<file>& input_t;
			typedef std::shared_ptr<file> output_t;
			static inline value serialise(input_t aValue) throw() { return value(aValue->get_path()); }
			static inline output_t deserialise(const value& aValue) throw() { return file::get_reference(aValue.get_string().c_ptr()); }
		};
	}
#endif
}
#endif
