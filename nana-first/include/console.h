#pragma once
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
    buff_[0] = '\0';
#ifdef _WIN32
		static HANDLE hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
		DWORD bytesAvailable = 0;
		if (::PeekNamedPipe(hStdin, NULL, 0, NULL, &bytesAvailable, NULL))
		{
			if (bytesAvailable > 0)
      {
        DWORD bytesRead = 0;
				::ReadFile(hStdin, buff_, sizeof(buff_) - 1, &bytesRead, NULL);
        bytesRead = bytesRead > 0? bytesRead: 0;
        buff_[bytesRead] = '\0';
      }
		}
#elif __linux
    auto bytes_read = ::read(STDIN_FILENO, buff_, sizeof(buff_) - 1);
    bytes_read = bytes_read > 0? bytes_read: 0;
    buff_[bytes_read] = '\0';
#endif
		return buff_;
	}

private:
	char buff_[BufferSize];
};
