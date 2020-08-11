/// =============================================================================
/// cpp-mmf (https://github.com/carlomilanesi/cpp-mmf)
/// 
/// LICENSE - Mozilla Public License 2.0
///
/// Memory-Mapped File C++ Library
/// This is a library, for the C++98 language and its successive versions, to 
/// handle files as arrays of bytes, exploiting system calls provided by 
/// POSIX-compliant operating systems and by Microsoft Windows.
///
/// Created by Carlo Milanesi
///
/// This Source Code Form is subject to the terms of the Mozilla Public 
/// License, v. 2.0. If a copy of the MPL was not distributed with this 
/// file, You can obtain one at http://mozilla.org/MPL/2.0/.
/// =============================================================================

#pragma once

#include "HandyBase.hpp"
#include "HandyFile.hpp"

namespace HANDY_NS {

	//unsigned int mmf_granularity();


	class MMFileBase
	{
	public:
		MMFileBase();
		~MMFileBase();
		size_t offset()      const { return offset_; }
		size_t mapped_size() const { return mapped_size_; }
		size_t file_size()   const { return file_size_; }
		void unmap();
		void close();
		bool is_open() const
		{
			return file_handle_ !=
			#if defined(_WIN32)
				(void*)
			#endif
			-1;
		}        
		#if defined(_WIN32)
			typedef void * HANDLE;
		#else
			typedef int HANDLE;
		#endif
		HANDLE file_handle() const { return file_handle_; }
	protected:
		size_t query_file_size_();
		char * data_;
		size_t offset_;
		size_t mapped_size_;
		size_t file_size_;
		int granularity_;
		HANDLE file_handle_;
		#if defined(_WIN32)
			HANDLE file_mapping_handle_;
		#endif
	};

	class MMFileReadOnly: public MMFileBase
	{
	public:
		MMFileReadOnly() = default;
		void open(char const* pathname, bool map_all = true);
		char const * data() const { return data_; }
		void map(size_t offset = 0, size_t size = 0);
	};

	class MMFile: public MMFileBase
	{
	public:
		MMFile() = default;
		void open(char const * pathname,
				  FileExistsMode         exists_mode = FileExistsMode::if_exists_fail,
				  FileNotExistMode doesnt_exist_mode = FileNotExistMode::if_doesnt_exist_create);
		char * data() { return data_; }
		void map(size_t offset = 0, size_t size = 0);
		bool flush();
	};
}
