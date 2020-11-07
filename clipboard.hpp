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
	/// ���а��쳣��
	/// </summary>
	class clipboard_exception : public std::exception
	{
	protected:
		/// <summary>
		/// Ϊʲô
		/// </summary>
		const char* why;

	public:
		/// <summary>
		/// ��ʼ�� clipboard_exception
		/// </summary>
		/// <param name=""></param>
		clipboard_exception(void)
			: why("clipboard exception")
		{
		}

		/// <summary>
		/// ��ʼ����
		/// </summary>
		/// <param name="what">������ʲô</param>
		clipboard_exception(const char* what)
			: why(what)
		{
		}

		/// <summary>
		/// ��ʼ����
		/// </summary>
		/// <param name="what">������ʲô</param>
		clipboard_exception(const std::string& what)
			: why(what.c_str())
		{
		}

		/// <summary>
		/// Ϊʲô
		/// </summary>
		/// <returns>Ϊʲô</returns>
		const char* what(void) const noexcept
		{
			return this->why;
		}
	};

	/// <summary>
	/// ���а��ࣨWindows��
	/// </summary>
	class clipboard
	{
	protected:
		/// <summary>
		/// clipboard �����ľ��
		/// </summary>
		HWND clipboard_handle;
		/// <summary>
		/// �Ƿ��Ѵ�
		/// </summary>
		bool _is_open;

	public:

		/// <summary>
		/// ��ʼ�� clipboard
		/// </summary>
		clipboard(void)
			: clipboard_handle(nullptr), _is_open(false)
		{
		}

		/// <summary>
		/// ��ʼ�� clipboard
		/// </summary>
		/// <param name="handle">clipboard �����ľ��</param>
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
		/// �򿪼��а�
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
				throw clipboard_exception("Clipboard is already occupied.���а��ѱ�ռ�á�");
#else
			throw clipboard_exception("Unsupported platform.��֧�ֵ�ƽ̨��");
#endif // _WIN32
			this->_is_open = true;
		}

		/// <summary>
		/// ���쳣��open����
		/// </summary>
		/// <returns>����ɹ�����Ϊtrue������Ϊfalse</returns>
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
		/// �Ѵ�
		/// </summary>
		/// <returns>������а��ѱ���ǰ����򿪣���Ϊtrue������Ϊfalse</returns>
		bool is_open(void)
		{
			return this->_is_open;
		}

		/// <summary>
		/// δ��
		/// </summary>
		/// <returns>������а�δ����ǰ����򿪣���Ϊtrue������Ϊfalse</returns>
		bool is_close(void)
		{
			return !(this->_is_open);
		}

		/// <summary>
		/// ��ռ��а壬��δ�����Զ���
		/// </summary>
		/// <seealso cref="hac::clipboard::open(void)"/>
		/// <exception cref="hac.clipboard.exception">���ʧ��</exception>
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
				throw clipboard_exception("Empty failed.���ʧ�ܡ�");
			if (not_open)
				close();
#else
			throw clipboard_exception("Unsupported platform.��֧�ֵ�ƽ̨��");
#endif // _WIN32
		}

		/// <summary>
		/// ���쳣��empty����
		/// </summary>
		/// <returns>����ɹ�����Ϊtrue������Ϊfalse</returns>
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
		/// ��CF_TEXT��ʽ���ü��а壬�����а�δ�����Զ��򿪲��رռ��а�
		/// </summary>
		/// <param name="content">��������</param>
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
		/// ���쳣�� set
		/// </summary>
		/// <param name="content">��������</param>
		/// </example>
		/// <returns>����ɹ����ã���Ϊtrue������Ϊfalse</returns>
		bool set_noex(const std::string& content) noexcept
		{
			set_noex(content, CF_TEXT);
		}

		/// <summary>
		/// ���쳣�� set
		/// </summary>
		/// <param name="content">��������</param>
		/// <param name="error_message">������Ϣ</param>
		/// </example>
		/// <returns>����ɹ����ã���Ϊtrue������Ϊfalse</returns>
		bool set_noex(const std::string& content, char* err) noexcept
		{
			set_noex(content, err, CF_TEXT);
		}

		/// <summary>
		/// ��FORMAT��ʽ���ü��а壬�����а�δ�����Զ��򿪲��رռ��а�
		/// </summary>
		/// <param name="content">��������</param>
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
				char* message = error_message("Set failed.����ʧ�ܣ�", 256);
				char error_message[256];
				strcpy_s(error_message, message);
				free_error_message(message);
				throw clipboard_exception(error_message);
			}
			LPTSTR lock = (LPTSTR)GlobalLock(content_memory);
			if (lock == nullptr)
				throw clipboard_exception("Set failed.����ʧ�ܡ�");
			std::memcpy(lock, c_content, size);
			lock[size] = 0;
			GlobalUnlock(content_memory);

			SetClipboardData(format, content_memory);
			if (not_open)
				close();
#else
			throw clipboard_exception("Unsupported platform.��֧�ֵ�ƽ̨��");
#endif // _WIN32
		}

		/// <summary>
		/// ���쳣�� set
		/// </summary>
		/// <param name="content">��������</param>
		/// </example>
		/// <returns>����ɹ����ã���Ϊtrue������Ϊfalse</returns>
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
		/// ���쳣�� set
		/// </summary>
		/// <param name="content">��������</param>
		/// <param name="error_message">������Ϣ</param>
		/// </example>
		/// <returns>����ɹ����ã���Ϊtrue������Ϊfalse</returns>
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
				char* message = error_message("Set failed.����ʧ�ܣ�", 256);
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
		/// ��ȡ���а�����
		/// </summary>
		/// <returns>����</returns>
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
				throw clipboard_exception("Get failed.��ȡʧ�ܡ�");
			}
			LPTSTR str = (LPTSTR)GlobalLock(mem);
			if (str == nullptr)
			{
				if (not_open)
					close();
				throw clipboard_exception("Get failed.��ȡʧ�ܡ�");
			}
			std::string result(str);
			GlobalUnlock(mem);
			if (not_open)
				close();
			return result;
#else
			throw clipboard_exception("Unsupported platform.��֧�ֵ�ƽ̨��");
#endif // _WIN32
		}

		/// <summary>
		/// ���쳣�� get
		/// </summary>
		/// <param name="str">����</param>
		/// <returns>����ɹ���ȡ����Ϊtrue������Ϊfalse</returns>
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
		/// �رռ��а�
		/// </summary>
		void close(void)
		{
#ifdef _WIN32
			if (!(this->_is_open))
				return;
			if (!CloseClipboard())
				throw clipboard_exception("Close failed.�ر�ʧ�ܡ�");
			this->_is_open = false;
#else
			throw clipboard_exception("Unsupported platform.��֧�ֵ�ƽ̨��");
#endif // _WIN32
		}

		/// <summary>
		/// ���쳣�� close
		/// </summary>
		/// <returns>����ɹ��رգ���Ϊtrue������Ϊfalse</returns>
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
		/// ������Ϣ
		/// </summary>
		/// <returns>������Ϣ</returns>
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
		/// ������Ϣ
		/// </summary>
		/// <param name="prefix">ǰ׺</param>
		/// <returns>������Ϣ</returns>
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
		/// �ͷ� error_message �������صĴ�����Ϣ���ڴ�
		/// </summary>
		/// <param name="message">������Ϣ</param>
		void free_error_message(char* message)
		{
			delete[] message;
		}
	};
}

#endif // _CLIPBOARD_HPP_