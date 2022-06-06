#pragma once
/*в дальнейшем возможна реализация механизма, при котором будет определён максимальный размер одного файла и при заполнении будет происходить переключение на новый
в задании не увидел подобных требований, но если будет необходимость - готов реализовать*/
#define MAX_FILE_LENGTH 1000
#define MAX_FILES_COUNT 16 
#include "IVFS.h"
#include <iostream>

namespace TestTask 
{

	class VirtualFileSystem : IVFS
	{
	public:
		static IVFS* CreateInstance();

		File* Open(const char* name); // Открыть файл в readonly режиме. Если нет такого файла или же он открыт во writeonly режиме - вернуть nullptr
		File* Create(const char* name); // Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути. Вернуть nullptr, если этот файл уже открыт в readonly режиме.
		size_t Read(File* f, char* buff, size_t len); // Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
		size_t Write(File* f, char* buff, size_t len); // Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
		void Close(File* f); // Закрыть файл
	private:
		static File* files[MAX_FILES_COUNT];

		char* GetFilePath(const char* name);
		char* CopyStringToPosition(const char* copyFrom, char* copyTo, int end);
		File* GetFile(const char* name, bool getFromArray = false);
		void ReplaceSlashes(char* name);
	};

}
