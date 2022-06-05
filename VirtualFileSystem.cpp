#include <iostream>
#include "VFS.h"

using namespace std;

int main()
{
    TestTask::IVFS* vfs = TestTask::VirtualFileSystem::CreateInstance();
    TestTask::File* file = nullptr;
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
        cin >> command;
        switch (command)
        {
        case 1:
            cout << "Enter file name:\n";
            cin >> fileName;
            file = vfs->Create(fileName);
            if (file)
            {
                cout << file->name << " is opened for writeonly\n";
            }
            break;
        case 2:
            cout << "Enter file name:\n";
            cin >> fileName;
            file = vfs->Open(fileName);
            if (file)
            {
                cout << file->name << " is opened for readonly\n";
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
            }
            if (res == -1)
            {
                cout << "Writing to the file failed\n";
            }
            else
            {
                cout << "Writing to the file succeed. Total bytes written:\n";
                cout << res << '\n';
            }
            break;
        case 4:
            cout << "Enter information length:\n";
            cin >> len;
            recievedInfo = new char[len]{};
            if (file != nullptr)
            {
                res = vfs->Read(file, recievedInfo, len);
            }
            if (res == 0)
            {
                cout << "Reading to the file failed\n";
            }
            else
            {
                cout << "Reading to the file succeed. Total bytes read:\n";
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
            }
            cout << "File closed\n";
            break;
        }
    } while (true);
}