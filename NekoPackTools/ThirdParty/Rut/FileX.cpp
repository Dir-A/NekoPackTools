﻿#include "FileX.h"
#include "StringX.h"

#include <Windows.h>
#include <shlobj.h>


namespace Rut
{
	namespace FileX
	{
		bool SaveFileViaPath(const wchar_t* pPath, void* pBuffer, size_t nSize)
		{
			std::wstring path = GetCurrentDirectoryFolder_RETW() + pPath;

			SHCreateDirectoryExW(NULL, PathRemoveFileName_RET(path).c_str(), NULL);

			HANDLE hFile = CreateFileW(pPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) return false;

			bool isWritten = WriteFile(hFile, pBuffer, nSize, NULL, NULL);
			FlushFileBuffers(hFile);
			CloseHandle(hFile);

			return isWritten;
		}

		bool SaveFileViaPath(const char* pPath, void* pBuffer, size_t nSize)
		{
			std::wstring wsPath;
			std::string msPath = pPath;
			StringX::StrToWStr(msPath, wsPath, CP_ACP);

			return SaveFileViaPath(wsPath.c_str(), pBuffer, nSize);
		}


		size_t GetCurrentDirectoryFolder(char* pPath)
		{
			size_t sz = GetCurrentDirectoryA(MAX_PATH, pPath);
			memcpy(pPath + sz, "\\", 1);
			return ++sz;
		}

		size_t GetCurrentDirectoryFolder(wchar_t* pPath)
		{
			size_t sz = GetCurrentDirectoryW(MAX_PATH, pPath);
			memcpy(pPath + sz, L"\\", 2);
			return ++sz;
		}

		size_t GetCurrentDirectoryFolder(std::string& msPath)
		{
			size_t len = 0;
			msPath.resize(MAX_PATH);
			len = GetCurrentDirectoryFolder(const_cast<char*>(msPath.data()));
			msPath.resize(len);
			return len;
		}

		size_t GetCurrentDirectoryFolder(std::wstring& wsPath)
		{
			size_t len = 0;
			wsPath.resize(MAX_PATH);
			len = GetCurrentDirectoryFolder(const_cast<wchar_t*>(wsPath.data()));
			wsPath.resize(len);
			return len;
		}

		std::string GetCurrentDirectoryFolder_RETA()
		{
			std::string path;
			GetCurrentDirectoryFolder(path);
			return path;
		}

		std::wstring GetCurrentDirectoryFolder_RETW()
		{
			std::wstring path;
			GetCurrentDirectoryFolder(path);
			return path;
		}


		void BackSlash(char* pPath)
		{
			for (size_t ite = 0; pPath[ite]; ite++)
			{
				if ((uint8_t)pPath[ite] > 0x7F) { ite++; continue; }

				switch (pPath[ite])
				{
				case '/':
					pPath[ite] = '\\';
					break;
				case '\\':
					pPath[ite] = '/';
					break;
				}
			}
		}

		void BackSlash(wchar_t* pPath)
		{
			for (size_t ite = 0; pPath[ite]; ite++)
			{
				switch (pPath[ite])
				{
				case L'/':
					pPath[ite] = L'\\';
					break;
				case L'\\':
					pPath[ite] = L'/';
					break;
				}
			}
		}

		void BackSlash(const char* pPath, std::string& msPath)
		{
			msPath = pPath;
			BackSlash(const_cast<char*>(msPath.data()));
		}

		void BackSlash(const wchar_t* pPath, std::wstring& wsPath)
		{
			wsPath = pPath;
			BackSlash(const_cast<wchar_t*>(wsPath.data()));
		}

		std::string BackSlash_RET(const char* pPath)
		{
			std::string msPath = pPath;
			BackSlash(const_cast<char*>(msPath.data()));
			return msPath;
		}

		std::wstring BackSlash_RET(const wchar_t* pPath)
		{
			std::wstring wsPath = pPath;
			BackSlash(const_cast<wchar_t*>(wsPath.data()));
			return wsPath;
		}


		bool PathRemoveFileName(std::string& msPath, std::string& msRemoved)
		{
			size_t pos = msPath.rfind("\\");
			if (pos == std::wstring::npos) return false;
			msRemoved = msPath.substr(0, pos + 1);
			return true;
		}

		bool PathRemoveFileName(std::wstring& wsPath, std::wstring& wsRemoved)
		{
			size_t pos = wsPath.rfind(L"\\");
			if (pos == std::wstring::npos) return false;
			wsRemoved = wsPath.substr(0, pos + 1);
			return true;
		}

		std::string PathRemoveFileName_RET(std::string& msPath)
		{
			std::string folder;
			PathRemoveFileName(msPath, folder);
			return folder;
		}

		std::wstring PathRemoveFileName_RET(std::wstring& wsPath)
		{
			std::wstring folder;
			PathRemoveFileName(wsPath, folder);
			return folder;
		}


		size_t PathGetFileName(char* pPath)
		{
			//StrLen
			size_t len = 0;
			while (pPath[++len]);

			for (size_t iteChar = len; iteChar > 0; iteChar--)
			{
				if (pPath[iteChar - 1] != '\\') continue;
				len -= iteChar;
				memcpy(pPath, pPath + iteChar, len + 1);
				return len;
			}

			return 0;
		}

		size_t PathGetFileName(wchar_t* pPath)
		{
			//StrLen
			size_t len = 0;
			while (pPath[++len]);

			for (size_t iteChar = len; iteChar > 0; iteChar--)
			{
				if (pPath[iteChar - 1] != L'\\') continue;
				len -= iteChar;
				memcpy(pPath, pPath + iteChar, (len + 1) * 2);
				return len;
			}

			return 0;
		}

		void PathGetFileName(std::string& msPath, std::string& msFileName)
		{
			msPath.resize(PathGetFileName(const_cast<char*>(msPath.data())));
			msFileName = msPath;
		}

		void PathGetFileName(std::wstring& wsPath, std::wstring& wsFileName)
		{
			wsPath.resize(PathGetFileName(const_cast<wchar_t*>(wsPath.data())));
			wsFileName = wsPath;
		}

		std::string PathGetFileName_RET(std::string& msPath)
		{
			std::string filename;
			PathGetFileName(msPath, filename);
			return filename;
		}

		std::wstring PathGetFileName_RET(std::wstring& wsPath)
		{
			std::wstring filename;
			PathGetFileName(wsPath, filename);
			return filename;
		}
	}
}

