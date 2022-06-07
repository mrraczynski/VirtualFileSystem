#pragma once
/*На данный момент система поддерживает запись файлов с фиксированной максимальной длинной
Вероятно, в дальнейшем можно будет реализовать динамическое перемещение файлов при увеличении их объема*/
#define MAX_FILES_COUNT 16
#define MAX_FILE_LENGTH 5
#define MAX_REAL_FILES_COUNT 4
#define MAX_REAL_FILE_LENGTH 20
#include "IVFS.h"
#include <mutex>
#include <fstream>
#include <iostream>

namespace TestTask 
{
	class VirtualFileSystem : public IVFS
	{
	public:
		VirtualFileSystem(const char* folder);

		File* Open(const char* name); // Открыть файл в readonly режиме. Если нет такого файла или же он открыт во writeonly режиме - вернуть nullptr
		File* Create(const char* name); // Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути. Вернуть nullptr, если этот файл уже открыт в readonly режиме.
		size_t Read(File* f, char* buff, size_t len); // Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
		size_t Write(File* f, char* buff, size_t len); // Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
		void Close(File* f); // Закрыть файл
	private:
		static int curRealFile;
		static int futRealFile;

		std::fstream myFileOut;
		std::ifstream myFileIn;
		File* files[MAX_FILES_COUNT] {nullptr};
		char realFiles[MAX_REAL_FILES_COUNT][260];
		std::mutex mtx;

		char* GetFilePath(const char* name);
		char* CopyStringToPosition(const char* copyFrom, char* copyTo, int end);
		File* GetFile(const char* name, bool getFromArray = false);
		void ReplaceSlashes(char* name);
		int GetRealFileNum(File* file);
		long ChangeFilesStartPos(File* curFile, long offset);
		long GetMaxPosition();
	};

}
