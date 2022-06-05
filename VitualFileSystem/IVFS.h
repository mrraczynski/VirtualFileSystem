#pragma once
#include <thread>

namespace TestTask
{
	struct File // �� ������ ����� ��� ������ ������ ���������� ���� ���������
	{	
	public:
		char name[260];
		long int length;

		bool isReadOnly = false;
		bool isWriteOnly = false;
		std::thread::id writerThreadId;
		std::thread::id* writerThreadIdPointer = &writerThreadId;

		File(const char* nm, long int len);
	};

	struct IVFS
	{	
		virtual File* Open(const char* name) = 0; // ������� ���� � readonly ������. ���� ��� ������ ����� ��� �� �� ������ �� writeonly ������ - ������� nullptr
		virtual File* Create(const char* name) = 0; // ������� ��� ������� ���� � writeonly ������. ���� �����, �� ������� ��� ������ �������������, ���������� � ����. ������� nullptr, ���� ���� ���� ��� ������ � readonly ������.
		virtual size_t Read(File* f, char* buff, size_t len) = 0; // ��������� ������ �� �����. ������������ �������� - ������� ������� ���� ������� ���������
		virtual size_t Write(File* f, char* buff, size_t len) = 0; // �������� ������ � ����. ������������ �������� - ������� ������� ���� ������� ��������
		virtual void Close(File* f) = 0; // ������� ����	
	};

}