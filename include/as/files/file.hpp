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
	/*!
		\brief A class that represents either a file or directory and can perform common file management tasks.
		\date 17th January 2017
		\author Adam Smith
	*/
	class file {
	public:
		enum {
			MAX_PATH_LENGTH = 126					//!< The maximum length of a file path.
		};
	private:
		char mPath[MAX_PATH_LENGTH + 1];			//!< The location of the file on disk.
		uint8_t mFlags;								//!< Stores property flags about the object.
	public:
		enum : uint8_t {
			FILE			= 1 << 0,				//!< Identifies object as a file.
			DIRECTORY		= 1 << 1,				//!< Identifies object as a directory.
			READABLE		= 1 << 2,				//!< Identifies file is readable.
			WRITABLE		= 1 << 3,				//!< Identifies file is writable.
			HIDDEN			= 1 << 4,				//!< Identifies object is hidden.
			EXISTS			= 1 << 5,				//!< Identifies object is exists.
			READ_WRITEABLE	= READABLE | WRITABLE,	//!< Identifies file is readable and writable.
		};
		
		
		/*!
			\brief Create a reference to a null file or directory.
		*/
		file() throw();
		
		/*!
			\brief Create a reference to a file or directory.
			\param aPath The location of the file or directory.
		*/
		file(const char*) throw();
		
		/*!
			\brief Close any open handles to the file or directory.
		*/
		~file() throw();
		
		/*!
			\brief Update reference to file or directory with any changes made outside of this object.
		*/
		void refresh() throw();
		
		/*!
			\brief Create the file or directory.
			\detail Reqires one of FILE or DIRECTORY to be a parameter.
			\param aFlags The properties of the file or directory.
			\return True if the file or directory was created.
		*/
		bool create(uint8_t) throw();
		
		/*!
			\brief Delete the file or directory.
			\detail The file or directory must first exist.
			If this is a directory then all children contained within will also be deleted.
			\return True if the file or directory was delete.
		*/
		bool destroy() throw();
		
		/*!
			\brief Set the file or directory to be hidden.
			\detail The file or directory must first exist.
			\return False if the file or directory cannot be hidden.
		*/
		bool hide() throw();
		
		/*!
			\brief Set the file or directory to not be hidden.
			\detail The file or directory must first exist.
			\return False if the file or directory cannot be made visible.
		*/
		bool show() throw();
		
		/*!
			\brief Copy this file or directory.
			\detail The file or directory must first exist.
			\param aPath The path and file name to copy to.
			\return The coppied file, will not exist if the copy failed.
		*/
		file copy(const char*) const throw();
		
		/*!
			\brief Move this file or directory to another location.
			\detail The can also be used to rename the file.
			The file or directory must first exist.
			\param aPath The directory path to move this to.
			\return False if the move could not be performed.
		*/
		bool move(const char*) throw();
		
		/*!
			\brief Check if this is a file or directory exists.
			\return True if this file or directory exists.
		*/
		bool exists() const throw();
		
		/*!
			\brief Check if this is a file.
			\detail The file must first exist.
			\return True if this is a file instead of a directory.
		*/
		bool is_file() const throw();
		
		/*!
			\brief Check if this is a directory.
			\detail The file must first exist.
			\return True if this is a directory instead of a file.
		*/
		bool is_directory() const throw();
		
		/*!
			\brief Check if the file is hidden.
			\detail The file must first exist.
			\return True if the file is hidden.
		*/
		bool is_hidden() const throw();
		
		/*!
			\brief Check if the file can be read from.
			\detail The file must first exist.
			\return True if the file is readable.
		*/
		bool is_readable() const throw();
		
		/*!
			\brief Check if the file can be written to.
			\detail The file must first exist.
			\return True if the file is writable.
		*/
		bool is_writable() const throw();
		
		/*!
			\brief Check if the file is read only.
			\detail The file must first exist.
			\return True if the file is writable and not readable.
		*/
		bool is_read_only() const throw();
		
		/*!
			\brief Check if the file is write only.
			\detail The file must first exist.
			\return True if the file is readable and not writable.
		*/
		bool is_write_only() const throw();
		
		/*!
			\brief Return the size of the file.
			\detail The file must first exist.
			\return The size of the file in bytes, or zero if this is a directory.
		*/
		size_t size() const throw();
		
		/*!
			\brief Return a list of child files.
			\detail The directory must first exist.
			\return Files contained within this directory, or empty if this is a file.
		*/
		std::vector<file> get_children() const throw();
		
		/*!
			\brief Get the full file name including the path and extension.
			\return The file path.
		*/
		const char* get_name() const throw();
		
		/*!
			\brief True if the file exists.
			\see exists
		*/
		operator bool() const throw();
	};
}

#endif