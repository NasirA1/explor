#pragma once

#include <string.h>

#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#else
static_assert(false, "Unsupported platform!")
#endif



template<const size_t BufferSize>
struct nonblocking_stdin_t
{
	nonblocking_stdin_t() : buff_{ 0 } 
	{
		::fcntl(0, F_SETFL, ::fcntl(0, F_GETFL) | O_NONBLOCK);
	}

	std::string get_input()
	{
		::memset(buff_, 0, sizeof(buff_));
    ::read(0, buff_, sizeof(buff_) - 1);
    return buff_;
	}

private:
	char buff_[BufferSize];
};
