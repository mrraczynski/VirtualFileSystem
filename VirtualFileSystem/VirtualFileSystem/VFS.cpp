#include "VFS.h"
#include <fstream>
#include <filesystem>

using namespace std;

namespace TestTask
{
	File* VirtualFileSystem::files[MAX_FILES_COUNT];

	File* VirtualFileSystem::Create(const char* name)
	{
		char* filePath = nullptr;
		try
		{
			ofstream myfile;
			File* file;
			filePath = GetFilePath(name);
			ReplaceSlashes(filePath);
			filesystem::path p = filePath; //в случае отсутствия у компилятора SDK поддержки C++17, вероятно, можно использовать stat и mkdir		
			if (!filesystem::exists(p))
			{
				filesystem::create_directories(p);
			}
			file = GetFile(name);
			if (file != nullptr)
			{
				if (!file->isReadOnly)
				{
					if ((file->isWriteOnly && *file->writerThreadIdPointer == this_thread::get_id()) || !file->isWriteOnly)
					{
						myfile.open(name, ios::out | ios::binary);
						file->isWriteOnly = true;
						file->writerThreadId = std::this_thread::get_id();
						file->writerThreadIdPointer = &file->writerThreadId;
						myfile.close();
						delete[] filePath;
						return file;
					}
					else
					{
						cout << "The file is being edited by another user\n";
					}
				}
				else
				{
					cout << "The file is in readonly mode\n";
				}
			}
			else
			{
				cout << "Cannot find the file\n";
			}
		}
		catch (exception& e)
		{
			cout << "An error has occurred:\n" << e.what() << endl;
			delete[] filePath;
			return nullptr;
		}
		delete[] filePath;
		return nullptr;
	}

	File* VirtualFileSystem::Open(const char* name)
	{
		try
		{
			ifstream myfile;
			File* file;
			file = GetFile(name, true);
			if (file != nullptr)
			{
				if (!file->isWriteOnly)
				{
					myfile.open(name, ios::in | ios::binary);
					file->isReadOnly = true;
					myfile.close();
					return file;
				}
				else
				{
					cout << "The file is in writeonly mode\n";
				}
			}
			else
			{
				cout << "Cannot find the file\n";
			}
		}
		catch (exception& e)
		{
			cout << "An error has occurred:\n" << e.what() << endl;
			return nullptr;
		}
		return nullptr;
	}

	size_t VirtualFileSystem::Read(File* f, char* buff, size_t len)
	{
		try
		{
			if (f != nullptr)
			{
				if (f->isReadOnly)
				{
					ifstream myfile(f->name, ios::in | ios::binary);
					istream* result = &myfile.read(buff, len);
					long size = result->gcount();
					myfile.close();
					return size;
				}
				else
				{
					cout << "The file is not in readonly mode\n";
				}
			}
			else
			{
				cout << "Cannot find the file\n";
			}
		}
		catch (exception& e)
		{
			cout << "An error has occurred:\n" << e.what() << endl;
			return 0;
		}
		return 0;
	}

	size_t VirtualFileSystem::Write(File* f, char* buff, size_t len)
	{
		try
		{
			if (f != nullptr && f->isWriteOnly && *f->writerThreadIdPointer == this_thread::get_id())
			{
				fstream myfile(f->name, ios::out | ios::binary);
				ostream* result = &myfile.write(buff, len);
				result->seekp(0, result->end);
				long size = result->tellp();
				myfile.close();
				return size;
			}
			else if(f == nullptr)
			{
				cout << "Cannot find the file\n";				
			}
			else
			{
				if (!f->isWriteOnly)
				{
					cout << "The file is not in writeonly mode\n";
				}
				else if (*f->writerThreadIdPointer != this_thread::get_id())
				{
					cout << "The file is being edited by another user\n";
				}
			}
		}
		catch (exception& e)
		{
			cout << "An error has occurred:\n" << e.what() << endl;
			return 0;
		}
		return 0;
	}

	void VirtualFileSystem::Close(File* f)
	{
		try
		{
			ifstream myfile;
			if (f != nullptr)
			{
				myfile.open(f->name, ios::in);
				f->isReadOnly = false;
				f->isWriteOnly = false;
				f->writerThreadIdPointer = nullptr;
				myfile.close();
			}
		}
		catch (exception& e)
		{
			cout << "An error has occurred:\n" << e.what() << endl;
		}
	}

	IVFS* VirtualFileSystem::CreateInstance()
	{
		return static_cast<IVFS*>(new VirtualFileSystem());
	}

	char* VirtualFileSystem::GetFilePath(const char* name)
	{
		char* filePath;
		for (int i = strlen(name); i - 1 >= 0; i--)
		{
			if (name[i] == '\\')
			{
				filePath = new char[i + 1];
				filePath = CopyStringToPosition(name, filePath, i);
				return filePath;
			}
		}		
		return nullptr;
	}

	char* VirtualFileSystem::CopyStringToPosition(const char* copyFrom, char* copyTo, int endPos)
	{
		for (int i = 0; i < endPos; i++)
		{
			copyTo[i] = copyFrom[i];
		}
		copyTo[endPos] = '\0';
		return copyTo;
	}

	File* VirtualFileSystem::GetFile(const char* name, bool getFromArray)
	{
		for (int i = 0; i < MAX_FILES_COUNT; i++)
		{
			if (files[i] != nullptr)
			{
				if (strcmp(files[i]->name, name) == 0)
				{
					return files[i];
				}
			}
			else
			{
				if (!getFromArray)
				{
					File* newFile = new File(name, 0);
					files[i] = newFile;
					return newFile;
				}
				else
				{
					return nullptr;
				}
			}
		}
		return nullptr;
	}

	void VirtualFileSystem::ReplaceSlashes(char* name)
	{
		for (int i = 0; i < strlen(name); i++)
		{
			if (name[i] == '\\')
			{
				name[i] = '/';
			}
		}
	}
}
