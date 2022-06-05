#pragma once
#define MAX_FILE_LENGTH 1000
#define MAX_FILES_COUNT 16
#include "IVFS.h"

namespace TestTask 
{

	class VirtualFileSystem : IVFS
	{
	public:
		static IVFS* CreateInstance();

		File* Open(const char* name); // ������� ���� � readonly ������. ���� ��� ������ ����� ��� �� �� ������ �� writeonly ������ - ������� nullptr
		File* Create(const char* name); // ������� ��� ������� ���� � writeonly ������. ���� �����, �� ������� ��� ������ �������������, ���������� � ����. ������� nullptr, ���� ���� ���� ��� ������ � readonly ������.
		size_t Read(File* f, char* buff, size_t len); // ��������� ������ �� �����. ������������ �������� - ������� ������� ���� ������� ���������
		size_t Write(File* f, char* buff, size_t len); // �������� ������ � ����. ������������ �������� - ������� ������� ���� ������� ��������
		void Close(File* f);
	private:
		static File* files[MAX_FILES_COUNT];

		char* GetFilePath(const char* name);
		char* CopyStringToPosition(const char* copyFrom, char* copyTo, int end);
		File* GetFile(const char* name, bool getFromArray = false);
		void ReplaceSlashes(char* name);
	};

}
