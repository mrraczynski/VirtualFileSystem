#pragma once
#include "IVFS.h"
#include <mutex>
#include <shared_mutex>
#include <fstream>
#include <iostream>

namespace TestTask 
{
	constexpr size_t MAX_REAL_FILE_LENGTH = 1000;
	constexpr size_t MAX_REAL_FILES_COUNT = 1000;
	constexpr size_t MAX_FILES_COUNT = 16;

	class VirtualFileSystem : public IVFS
	{
	public:
		VirtualFileSystem(const char* folder);
		~VirtualFileSystem();

		File* Open(const char* name) override; // Открыть файл в readonly режиме. Если нет такого файла или же он открыт во writeonly режиме - вернуть nullptr
		File* Create(const char* name) override; // Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути. Вернуть nullptr, если этот файл уже открыт в readonly режиме.
		size_t Read(File* f, char* buff, size_t len) override; // Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
		size_t Write(File* f, char* buff, size_t len) override; // Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
		void Close(File* f) override; // Закрыть файл
	private:
		static int curRealFile;
		static int futRealFile;

		std::fstream myFileOut;
		File* files[MAX_FILES_COUNT] {nullptr};
		char realFiles[MAX_REAL_FILES_COUNT][260];
		std::shared_mutex mtx;

		File* GetFile(const char* name, bool getFromArray = false);
		void ReplaceSlashes(char* name);
		long ChangeFilesStartPos(File* curFile, long offset);
		size_t GetMaxPosition();
	};

}
