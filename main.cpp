#include "clipboard.hpp"
#include <iostream>
#include <string>
#include <cinttypes>
#pragma comment(lib, "User32.lib")

constexpr std::int32_t CONTENT = 1;
constexpr std::int32_t EDIT = 2;

int main(int argc, char** argv)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
#endif // _WIN32
	hac::clipboard clip;
	try
	{
		if (argc == CONTENT)
		{
			std::cout << clip.get() << std::endl;
		}
		else if (std::string(argv[1]) == std::string("edit"))
		{
			if (argc == EDIT)
			{
				bool is_start = true;
				while (true)
				{
					std::string str;
					std::getline(std::cin, str);
					if (str.find('\0') != str.npos)
					{
						return 0;
					}
					if (is_start)
					{
						clip.set(str);
						is_start = false;
					}
					else
					{
						clip.set(clip.get() + '\n' + str);
					}
				}
			}
			else
			{
				clip.set(argv[2]);
			}
		}
		else
		{
			throw "Unknown arguments.";
		}
	}
	catch (hac::clipboard_exception ex)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif // _WIN32
		std::cerr << "ERROR: " << ex.what();
	}
	catch (std::exception ex)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif // _WIN32
		std::cerr << "ERROR: " << ex.what();
	}
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), info.wAttributes);
#endif // _WIN32
}