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

#ifndef ASMITH_FILES_DIRECTORY_HPP
#define ASMITH_FILES_DIRECTORY_HPP

#include "filesystem_object.hpp"

namespace asmith {
	class directory : public filesystem_entity {
	protected:
		directory();
		directory(const char* aPath);
		
		// Inherited from filesystem_entity
		uint32_t get_flags() const override;
	public:
		static std::shared_ptr<directory> get_reference(const char*);
		static std::shared_ptr<directory> create_temporary(const char*);
		~directory();
		
		std::vector<std::shared_ptr<filesystem_entity>> get_children() const ;
		
		// Inherited from filesystem_entity
		
		void hide() override;
		void show() override;
		void create(const uint32_t aFlags) override;
		void destroy() override;
		void move(const char* aPath) override;
		void copy(const char* aPath) override;
		bool is_directory() const override;
		bool is_file() const override;
	};
}
#endif
