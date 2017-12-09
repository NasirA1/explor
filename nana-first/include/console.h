#pragma once
#include <string.h>
#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#elif _WIN32
#include <Windows.h>
#else
static_assert(false, "Unsupported platform!")
#endif


//Project Properties -> Debugging -> Command Arguments
//"< $(TargetDir)\nana-first-vs2015.log"


template<const size_t BufferSize>
struct nonblocking_stdin_t
{
	nonblocking_stdin_t() : buff_{ 0 } 
	{
#ifdef __linux__
		::fcntl(0, F_SETFL, ::fcntl(0, F_GETFL) | O_NONBLOCK);
#endif
	}

	std::string get_input()
	{
		::memset(buff_, 0, sizeof(buff_));
#ifdef _WIN32
		static HANDLE hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
		DWORD bytesAvailable = 0;
		if (::PeekNamedPipe(hStdin, NULL, 0, NULL, &bytesAvailable, NULL))
		{
			if (bytesAvailable > 0)
				::ReadFile(hStdin, buff_, sizeof(buff_) - 1, NULL, NULL);
		}
#elif __linux
    ::read(0, buff_, sizeof(buff_) - 1);
#endif
		return buff_;
	}

private:
	char buff_[BufferSize];
};
