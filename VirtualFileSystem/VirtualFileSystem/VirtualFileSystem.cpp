#include "VFS.h"

using namespace std;

void main()
{
    try
    {
        char folderName[260];
        cout << "Please, enter the folder name:\n";
        cin >> folderName;
        TestTask::VirtualFileSystem* vfs = new TestTask::VirtualFileSystem(folderName);
        TestTask::File* file = nullptr;
        TestTask::File* buffFile = nullptr;
        char fileName[260];
        char info[1024];
        char* recievedInfo;
        int command;
        long res = -1, len = 0;
        do
        {
            cout << "Please, select command:\n";
            cout << "1. Create/open writeonly\n";
            cout << "2. Open readonly\n";
            cout << "3. Write\n";
            cout << "4. Read\n";
            cout << "5. Close\n";
            cout << "6. Delete file system\n";
            cin >> command;
            switch (command)
            {
            case 1:
                cout << "Enter file name:\n";
                cin >> fileName;
                buffFile = vfs->Create(fileName);
                if (buffFile != nullptr)
                {
                    file = buffFile;
                    cout << fileName << " is open for writeonly\n";
                }
                else
                {
                    cout << fileName << " is not open\n";
                }
                break;
            case 2:
                cout << "Enter file name:\n";
                cin >> fileName;
                buffFile = vfs->Open(fileName);
                if (buffFile != nullptr)
                {
                    file = buffFile;
                    cout << fileName << " is open for readonly\n";
                }
                else
                {
                    cout << fileName << " is not open\n";
                }
                break;
            case 3:
                cout << "Enter information length:\n";
                cin >> len;
                cout << "Enter information:\n";
                cin.get();
                gets_s(info, 1024);
                if (file != nullptr)
                {
                    res = vfs->Write(file, info, len);
                    cout << file->name << ": Writing to the file succeed. Total bytes written:\n";
                    cout << res << '\n';
                }
                break;
            case 4:
                cout << "Enter information length:\n";
                cin >> len;
                recievedInfo = new char[len] {};
                if (file != nullptr)
                {
                    res = vfs->Read(file, recievedInfo, len);
                    cout << file->name << ": Reading from the file succeed. Total bytes read:\n";
                    cout << res << '\n';
                    cout << "Information:\n";
                    cout << recievedInfo << '\n';
                }
                delete[] recievedInfo;
                break;
            case 5:
                if (file != nullptr)
                {
                    vfs->Close(file);
                    cout << file->name << ": File closed\n";
                }
                else
                {
                    cout << "There is no open file.\n";
                }
                break;
            case 6:
                delete vfs;
                break;
            }
        } while (true);
    }
    catch (exception& e)
    {        
        return;
    }
}