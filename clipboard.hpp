#ifndef _CLIPBOARD_HPP_
#define _CLIPBOARD_HPP_

#include <exception>
#include <cstring>
#include <string>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

namespace hac
{
	/// <summary>
	/// 剪切板异常类
	/// </summary>
	class clipboard_exception : public std::exception
	{
	protected:
		/// <summary>
		/// 为什么
		/// </summary>
		const char* why;

	public:
		/// <summary>
		/// 初始化 clipboard_exception
		/// </summary>
		/// <param name=""></param>
		clipboard_exception(void)
			: why("clipboard exception")
		{
		}

		/// <summary>
		/// 初始化类
		/// </summary>
		/// <param name="what">发生了什么</param>
		clipboard_exception(const char* what)
			: why(what)
		{
		}

		/// <summary>
		/// 初始化类
		/// </summary>
		/// <param name="what">发生了什么</param>
		clipboard_exception(const std::string& what)
			: why(what.c_str())
		{
		}

		/// <summary>
		/// 为什么
		/// </summary>
		/// <returns>为什么</returns>
		const char* what(void) const noexcept
		{
			return this->why;
		}
	};

	/// <summary>
	/// 剪切板类（Windows）
	/// </summary>
	class clipboard
	{
	protected:
		/// <summary>
		/// clipboard 所属的句柄
		/// </summary>
		HWND clipboard_handle;
		/// <summary>
		/// 是否已打开
		/// </summary>
		bool _is_open;

	public:

		/// <summary>
		/// 初始化 clipboard
		/// </summary>
		clipboard(void)
			: clipboard_handle(nullptr), _is_open(false)
		{
		}

		/// <summary>
		/// 初始化 clipboard
		/// </summary>
		/// <param name="handle">clipboard 所属的句柄</param>
		clipboard(HWND handle)
			: clipboard_handle(handle), _is_open(false)
		{
		}

		~clipboard(void)
		{
			if (this->_is_open)
				close();
		}

		/// <summary>
		/// 打开剪切板
		/// </summary>
		/// <example>
        /// <code>
		/// int main(void)
		/// {
		///		try
		///		{
		///			hac::clipboard clip;
		///			clip.open();
		///			std::string content;
		///			std::cin >> content;
		///			clip.set(content);
		///			clip.close();
		///		}
		///		catch (hac::clipboard::exception ex)
		///		{
		///			std::cerr << (ex.what());
		///		}
		///		return 0;
		/// }
        /// </code>
        /// </example>
		void open(void)
		{
			if (this->_is_open)
				return;
#ifdef _WIN32
			if (!OpenClipboard(NULL))
				throw clipboard_exception("Clipboard is already occupied.剪切板已被占用。");
#else
			throw clipboard_exception("Unsupported platform.不支持的平台。");
#endif // _WIN32
			this->_is_open = true;
		}

		/// <summary>
		/// 无异常的open函数
		/// </summary>
		/// <returns>如果成功，则为true；否则为false</returns>
		bool open_noex(void) noexcept
		{
			if (this->_is_open)
				return true;
#ifdef _WIN32
			if (!OpenClipboard(NULL))
				return false;
#else
			return false;
#endif // _WIN32
			this->_is_open = true;
			return true;
		}

		/// <summary>
		/// 已打开
		/// </summary>
		/// <returns>如果剪切板已被当前句柄打开，则为true；否则为false</returns>
		bool is_open(void)
		{
			return this->_is_open;
		}

		/// <summary>
		/// 未打开
		/// </summary>
		/// <returns>如果剪切板未被当前句柄打开，则为true；否则为false</returns>
		bool is_close(void)
		{
			return !(this->_is_open);
		}

		/// <summary>
		/// 清空剪切板，若未打开则自动打开
		/// </summary>
		/// <seealso cref="hac::clipboard::open(void)"/>
		/// <exception cref="hac.clipboard.exception">清空失败</exception>
		void empty(void)
		{
#ifdef _WIN32
			bool not_open = false;
			if (!(this->_is_open))
			{
				open();
				not_open = true;
			}
			if (!EmptyClipboard())
				throw clipboard_exception("Empty failed.清空失败。");
			if (not_open)
				close();
#else
			throw clipboard_exception("Unsupported platform.不支持的平台。");
#endif // _WIN32
		}

		/// <summary>
		/// 无异常的empty函数
		/// </summary>
		/// <returns>如果成功，则为true；否则为false</returns>
		/// <seealso cref="hac::clipboard::open(void)"/>
		bool empty_noex(void) noexcept
		{
#ifdef _WIN32
			bool not_open = false;
			if (!(this->_is_open))
			{
				if (!open_noex())
					return false;
				not_open = true;
			}
			if (!EmptyClipboard())
				return false;
			if (not_open)
				close();
#else
			return false;
#endif // _WIN32
			return true;
		}

		/// <summary>
		/// 以CF_TEXT格式设置剪切板，若剪切板未打开则自动打开并关闭剪切板
		/// </summary>
		/// <param name="content">设置内容</param>
		/// <example>
		/// <code>
		/// int main(void)
		/// {
		///		std::string input;
		///		std::cin >> input;
		///		hac::clipboard clip;
		///		clip.set(input);
		/// 
		///		return 0;
		/// }
		/// </code>
		/// </example>
		void set(const std::string& content)
		{
			set(content, CF_TEXT);
		}

		/// <summary>
		/// 无异常的 set
		/// </summary>
		/// <param name="content">设置内容</param>
		/// </example>
		/// <returns>如果成功设置，则为true；否则为false</returns>
		bool set_noex(const std::string& content) noexcept
		{
			set_noex(content, CF_TEXT);
		}

		/// <summary>
		/// 无异常的 set
		/// </summary>
		/// <param name="content">设置内容</param>
		/// <param name="error_message">错误信息</param>
		/// </example>
		/// <returns>如果成功设置，则为true；否则为false</returns>
		bool set_noex(const std::string& content, char* err) noexcept
		{
			set_noex(content, err, CF_TEXT);
		}

		/// <summary>
		/// 以FORMAT格式设置剪切板，若剪切板未打开则自动打开并关闭剪切板
		/// </summary>
		/// <param name="content">设置内容</param>
		void set(const std::string& content, int format)
		{
#ifdef _WIN32
			bool not_open = false;
			if (!(this->_is_open))
			{
				open();
				not_open = true;
			}
			empty();
			const char* c_content = content.c_str();
			std::string::size_type size = content.size();
			HGLOBAL content_memory = GlobalAlloc(GMEM_MOVEABLE, (size + 1) * sizeof(char));
			if (content_memory == NULL)
			{
				if (not_open)
					close();
				char* message = error_message("Set failed.设置失败：", 256);
				char error_message[256];
				strcpy_s(error_message, message);
				free_error_message(message);
				throw clipboard_exception(error_message);
			}
			LPTSTR lock = (LPTSTR)GlobalLock(content_memory);
			if (lock == nullptr)
				throw clipboard_exception("Set failed.设置失败。");
			std::memcpy(lock, c_content, size);
			lock[size] = 0;
			GlobalUnlock(content_memory);

			SetClipboardData(format, content_memory);
			if (not_open)
				close();
#else
			throw clipboard_exception("Unsupported platform.不支持的平台。");
#endif // _WIN32
		}

		/// <summary>
		/// 无异常的 set
		/// </summary>
		/// <param name="content">设置内容</param>
		/// </example>
		/// <returns>如果成功设置，则为true；否则为false</returns>
		bool set_noex(const std::string& content, int format) noexcept
		{
#ifdef _WIN32
			bool not_open = false;
			if (!(this->_is_open))
			{
				if (!open_noex())
					return false;
				not_open = true;
			}
			if (!empty_noex())
				return false;
			const char* c_content = content.c_str();
			std::string::size_type size = content.size();
			HGLOBAL content_memory = GlobalAlloc(GMEM_MOVEABLE, (size + 1) * sizeof(char));
			if (content_memory == NULL)
			{
				if (not_open)
					close_noex();
				return false;
			}
			LPTSTR lock = (LPTSTR)GlobalLock(content_memory);
			std::memcpy(lock, c_content, size);
			lock[size] = 0;
			GlobalUnlock(content_memory);

			SetClipboardData(format, content_memory);
			if (not_open)
				close_noex();
			return true;
#else
			return false;
#endif // _WIN32
		}

		/// <summary>
		/// 无异常的 set
		/// </summary>
		/// <param name="content">设置内容</param>
		/// <param name="error_message">错误信息</param>
		/// </example>
		/// <returns>如果成功设置，则为true；否则为false</returns>
		bool set_noex(const std::string& content, char* err, int format) noexcept
		{
#ifdef _WIN32
			bool not_open = false;
			if (!(this->_is_open))
			{
				if (!open_noex())
					return false;
				not_open = true;
			}
			if (!empty_noex())
				return false;
			const char* c_content = content.c_str();
			std::string::size_type size = content.size();
			HGLOBAL content_memory = GlobalAlloc(GMEM_MOVEABLE, (size + 1) * sizeof(char));
			if (content_memory == NULL)
			{
				if (not_open)
					close_noex();
				char* message = error_message("Set failed.设置失败：", 256);
				strcpy_s(err, 256, message);
				free_error_message(message);
			}
			if (content_memory == nullptr)
				return false;
			LPTSTR lock = (LPTSTR)GlobalLock(content_memory);
			std::memcpy(lock, c_content, size);
			lock[size] = 0;
			GlobalUnlock(content_memory);

			SetClipboardData(format, content_memory);
			if (not_open)
				close_noex();
			return true;
#else
			return false;
#endif // _WIN32
		}

		/// <summary>
		/// 获取剪切板内容
		/// </summary>
		/// <returns>内容</returns>
		std::string get(void)
		{
#ifdef _WIN32
			bool not_open = false;
			if (!(this->_is_open))
			{
				open();
				not_open = true;
			}
			HGLOBAL mem;
			mem = GetClipboardData(CF_TEXT);
			if (mem == nullptr)
				mem = GetClipboardData(CF_UNICODETEXT);
			if (mem == nullptr)
			{
				if (not_open)
					close();
				throw clipboard_exception("Get failed.获取失败。");
			}
			LPTSTR str = (LPTSTR)GlobalLock(mem);
			if (str == nullptr)
			{
				if (not_open)
					close();
				throw clipboard_exception("Get failed.获取失败。");
			}
			std::string result(str);
			GlobalUnlock(mem);
			if (not_open)
				close();
			return result;
#else
			throw clipboard_exception("Unsupported platform.不支持的平台。");
#endif // _WIN32
		}

		/// <summary>
		/// 无异常的 get
		/// </summary>
		/// <param name="str">内容</param>
		/// <returns>如果成功读取，则为true；否则为false</returns>
		bool get_noex(std::string& result) noexcept
		{
#ifdef _WIN32
			bool not_open = false;
			if (!(this->_is_open))
			{
				open();
				not_open = true;
			}
			HGLOBAL mem;
			mem = GetClipboardData(CF_TEXT);
			if (mem == nullptr)
				mem = GetClipboardData(CF_UNICODETEXT);
			if (mem == nullptr)
			{
				if (not_open)
					close();
				return false;
			}
			LPTSTR str = (LPTSTR)GlobalLock(mem);
			if (str == nullptr)
			{
				if (not_open)
					close();
				return false;
			}
			result = str;
			GlobalUnlock(mem);
			if (not_open)
				close();
			return true;
#else
			return false;
#endif // _WIN32
		}

		/// <summary>
		/// 关闭剪切板
		/// </summary>
		void close(void)
		{
#ifdef _WIN32
			if (!(this->_is_open))
				return;
			if (!CloseClipboard())
				throw clipboard_exception("Close failed.关闭失败。");
			this->_is_open = false;
#else
			throw clipboard_exception("Unsupported platform.不支持的平台。");
#endif // _WIN32
		}

		/// <summary>
		/// 无异常的 close
		/// </summary>
		/// <returns>如果成功关闭，则为true；否则为false</returns>
		bool close_noex(void) noexcept
		{
#ifdef _WIN32
			if (!(this->_is_open))
				return true;
			if (!CloseClipboard())
				return false;
			this->_is_open = false;
#else
			return false;
#endif // _WIN32
			return true;
		}

	protected:
		/// <summary>
		/// 错误信息
		/// </summary>
		/// <returns>错误信息</returns>
		char* error_message(void)
		{
			char buf[128];
			void* message_buf;
			unsigned long error_code = GetLastError();

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(char*)&message_buf, 0, NULL);
			strcpy_s(buf, (const char*)message_buf);
			LocalFree(message_buf);
			return buf;
		}

		/// <summary>
		/// 错误信息
		/// </summary>
		/// <param name="prefix">前缀</param>
		/// <returns>错误信息</returns>
		char* error_message(const char* prefix, int buf_siz)
		{
			char* buf = new char[buf_siz];
			void* message_buf;
			unsigned long error_code = GetLastError();

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(char*)&message_buf, 0, NULL);
			sprintf_s(buf, buf_siz, "%s%s", prefix, (char*)message_buf);
			LocalFree(message_buf);
			return buf;
		}

		/// <summary>
		/// 释放 error_message 函数返回的错误信息的内存
		/// </summary>
		/// <param name="message">错误信息</param>
		void free_error_message(char* message)
		{
			delete[] message;
		}
	};
}

#endif // _CLIPBOARD_HPP_