Доброго времени суток!

В данном проекте реализована Virtual File System для хранения большого количества виртуальных файлов в одном физическом файле на диске.
Работа с файловой системой реализована по принципу "один экземпляр VirtualFileSystem = один физический файл на диске".
Один физический файл может хранить определенное количество виртуальных файлов, данное количество задано constexpr MAX_FILES_COUNT, constexpr MAX_REAL_FILE_LENGTH же отвечает за максимальный размер файла на диске.
Виртуальная файловая система позволяет открывать файлы в writeonly или readonly режиме, создавать, перезаписывать и читать контент существующего виртуального файла.
Данная функциональность реализована в потокобезопасном режиме, т.е. работа с одним экземпляром VirtualFileSystem возможна из нескольких потоков.

Good day!

This project implements a Virtual File System for storing a large number of virtual files in one physical file on disk.
Working with the file system is implemented according to the principle "one instance of VirtualFileSystem = one physical file on disk".
One physical file can store a certain number of virtual files, this number is set by the MAX_FILES_COUNT constexpr, the MAX_REAL_FILE_LENGTH constexpr is responsible for the maximum file size on disk.
The virtual file system allows you to open files in writeonly or readonly mode, create, overwrite and read the content of an existing virtual file.
This functionality is implemented in thread-safe mode, i.e. working with a single instance of VirtualFileSystem is possible from multiple threads.
