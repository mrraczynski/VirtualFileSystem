#include "VFS.h"
#include <filesystem>
#include <string>

using namespace std;

namespace TestTask
{
	int VirtualFileSystem::curRealFile = 0;
	int VirtualFileSystem::futRealFile = 0;

	VirtualFileSystem::VirtualFileSystem(const char* folder)
	{
		try
		{
			for (int i = 0; i < MAX_FILES_COUNT; i++)
			{
				files[i] = nullptr;
			}
			if (curRealFile < MAX_REAL_FILES_COUNT)
			{
				curRealFile = futRealFile;
				ofstream myfile;
				filesystem::path p = folder; //в случае отсутствия у компилятора SDK поддержки C++17, вероятно, можно использовать stat и mkdir		
				if (!filesystem::exists(p))
				{
					filesystem::create_directories(p);
				}

				std::string tmp = std::to_string(curRealFile);
				char const* num_char = tmp.c_str();
				realFiles[curRealFile][0] = '\0';
				strcat_s(realFiles[curRealFile], 260, folder);
				strcat_s(realFiles[curRealFile], 260, "\\File");
				strcat_s(realFiles[curRealFile], 260, num_char);
				strcat_s(realFiles[curRealFile], 260, ".vfs");
				myfile.open(realFiles[curRealFile], ios::out | ios::binary);
				myfile.close();
				futRealFile++;
			}
			else 
			{
				cout << "Cannot create a new file on the disk:\n";
			}
		}
		catch (exception& e)
		{
			cout << "An error has occurred:\n" << e.what() << endl;
		}
	}

	VirtualFileSystem::~VirtualFileSystem()
	{
		for (int i = 0; i < MAX_FILES_COUNT; i++)
		{
			delete files[i];
		}
	}

	File* VirtualFileSystem::Create(const char* name)
	{
		try
		{
			File* file = GetFile(name);
			std::lock_guard<shared_mutex> lock(mtx);
			if (file != nullptr)
			{
				if (!file->isReadOnly)
				{
					if ((file->isWriteOnly && *file->writerThreadIdPointer == this_thread::get_id()) || !file->isWriteOnly)
					{
						file->isWriteOnly = true;
						file->writerThreadId = std::this_thread::get_id();
						file->writerThreadIdPointer = &file->writerThreadId;
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
			return nullptr;
		}
		return nullptr;
	}

	File* VirtualFileSystem::Open(const char* name)
	{
		try
		{
			File* file;
			file = GetFile(name, true);
			if (file != nullptr)
			{
				if (!file->isWriteOnly)
				{
					file->isReadOnly = true;
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
					std::shared_lock<shared_mutex> lock(mtx);
					myFileOut.open(realFiles[curRealFile], ios::in | ios::binary);
					myFileOut.seekg(f->startPoint);
					istream* result = &myFileOut.read(buff, len);
					long size = result->gcount();
					myFileOut.close();
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
				if (GetMaxPosition() + len > MAX_REAL_FILE_LENGTH)
				{
					cout << "Not enough space\n";
					return 0;
				}
				long size;
				std::lock_guard<shared_mutex> lock(mtx);
				if (f->isNewFile)
				{
					myFileOut.open(realFiles[curRealFile], ios::app | ios::binary);
					myFileOut.seekp(GetMaxPosition());
					size = myFileOut.tellp();
					f->startPoint = size;
					strcpy_s(f->realFileName, 260, realFiles[curRealFile]);
					ostream* result = &myFileOut.write(buff, len);
					result->seekp(0, result->end);
					size = result->tellp();
					f->length = size - f->startPoint;
					f->isNewFile = false;
					myFileOut.close();
				}
				else if (len < f->length && !f->isNewFile)
				{
					char contentBuf[MAX_REAL_FILE_LENGTH + 1];
					myFileOut.open(realFiles[curRealFile], ios::in | ios::binary);
					myFileOut.seekg(f->startPoint + f->length);
					myFileOut.read(contentBuf, MAX_REAL_FILE_LENGTH);
					myFileOut.close();
					myFileOut.open(realFiles[curRealFile], ios::in | ios::out | ios::binary);
					myFileOut.seekp(f->startPoint + len);
					int i = ChangeFilesStartPos(f, len);
					contentBuf[i] = '\0';
					myFileOut.write(contentBuf, i);
					myFileOut.seekp(f->startPoint);
					ostream* result = &myFileOut.write(buff, len);
					size = result->tellp();
					f->length = size - f->startPoint;
					myFileOut.close();
				}
				else if (len == f->length && !f->isNewFile)
				{
					myFileOut.open(realFiles[curRealFile], ios::in | ios::out | ios::binary);
					myFileOut.seekp(f->startPoint);
					ostream* result = &myFileOut.write(buff, len);
					myFileOut.close();
				}
				else if(len > f->length && !f->isNewFile)
				{
					char contentBuf[MAX_REAL_FILE_LENGTH + 1];
					myFileOut.open(realFiles[curRealFile], ios::in | ios::binary);
					myFileOut.seekg(f->startPoint + f->length);
					myFileOut.read(contentBuf, MAX_REAL_FILE_LENGTH);
					myFileOut.close();
					myFileOut.open(realFiles[curRealFile], ios::in | ios::out | ios::binary);
					myFileOut.seekp(f->startPoint);
					int i = ChangeFilesStartPos(f, 0);
					contentBuf[i] = '\0';
					myFileOut.write(contentBuf, i);
					myFileOut.seekp(GetMaxPosition());
					f->startPoint = myFileOut.tellp();
					ostream* result = &myFileOut.write(buff, len);
					size = result->tellp();
					f->length = size - f->startPoint;
					myFileOut.close();
				}
				return f->length;
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
					cout << "The file is being edited by another thread\n";
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
			if (f != nullptr)
			{
				f->isReadOnly = false;
				f->isWriteOnly = false;
				f->writerThreadIdPointer = nullptr;
			}
		}
		catch (exception& e)
		{
			cout << "An error has occurred:\n" << e.what() << endl;
		}
	}

	//get existing file or create new one
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
					File* newFile = new File(name);
					files[i] = newFile;
					//SetFilePosition(newFile, i);
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
	//replace slashes for filesys lib
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
	//offseting files positions if we move some file to the end
	long VirtualFileSystem::ChangeFilesStartPos(File* curFile, long offset)
	{
		long totalLen = 0;
		for (int i = 0; i < MAX_FILES_COUNT; i++)
		{
			if (files[i] != nullptr)
			{
				if (strcmp(files[i]->realFileName, curFile->realFileName) == 0 && strcmp(curFile->name, files[i]->name) != 0 && files[i]->startPoint > curFile->startPoint)
				{
					files[i]->startPoint = files[i]->startPoint - curFile->length + offset;
					totalLen = totalLen = files[i]->length;
				}
			}
			else
			{
				return totalLen;
			}
		}
		return totalLen;
	}
	//get max position for writing at the end of file (in some cases we can have redundant symbols so we cannot use ios_base::end)
	long VirtualFileSystem::GetMaxPosition()
	{
		long maxValue = 0;
		for (int i = 0; i < MAX_FILES_COUNT; i++)
		{
			if (files[i] != nullptr)
			{
				if (maxValue < files[i]->startPoint + files[i]->length)
				{
					maxValue = files[i]->startPoint + files[i]->length;
				}
			}
			else
			{
				break;
			}
		}
		return maxValue;
	}

}
