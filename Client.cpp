#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <winsock2.h>
#include <string>
#include <fstream>
#include <ctime>
using namespace std;

SOCKET Connect;
string sent = " sent: ";
string received = " received: ";

void log(string message, string action)
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);                         // текущая дата, выраженная в секундах
    timeinfo = localtime(&rawtime);        // текущая дата, представленная в нормальной форме

    ofstream out;
    out.open("D:\\Археос\\logfile.txt", ios::app);

    if (out.is_open())
    {
        out << asctime(timeinfo) << "Client" << action << message << endl;
    }
    out.close();
}

void GetInfo()
{
    int size_of_info;
    recv(Connect, (char*)&size_of_info, sizeof(size_of_info), NULL);
    char* info = new char[size_of_info + 1];
    info[size_of_info] = '\0';
    recv(Connect, info, size_of_info, NULL);
    string info_reinitialize = info;
    log(info, received);
    cout << info << endl;
    delete[] info;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    WSAData wsadata;
    WORD DLLVersion = MAKEWORD(2, 1);

    if (WSAStartup(DLLVersion, &wsadata) != 0)
    {
        cout << "WSAStartup couldn't upload winsock version" << endl;
        log("WSAStartup couldn't upload winsock version", " error: ");
        exit(1);
    }

    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1032);
    addr.sin_family = AF_INET;

    Connect = socket(AF_INET, SOCK_STREAM, NULL);

    if (connect(Connect, (SOCKADDR*)&addr, sizeof(addr)) != 0)
    {
        cout << "Connection failed" << endl;
        log("Connection failed", " error: ");
        return 1;
    }
    else
    {
        cout << "Client connected" << endl;
        log("Client connected", " upload successful: ");

        cout << "Wait for command input:" << endl;
        log("Wait for command input:", " state: ");
        while (true)
        {
            string command;
            cin >> command;
            int size_of_command = size(command);
            send(Connect, (char*)&size_of_command, sizeof(size_of_command), NULL);
            send(Connect, command.c_str(), size_of_command, NULL);
            log(command, sent);
            if (command == "Translate")
            {
                string word;
                cin >> word;
                int size_of_word = size(word);
                send(Connect, (char*)&size_of_word, sizeof(size_of_word), NULL);
                send(Connect, word.c_str(), size_of_word, NULL);
                log(word, sent);
            }
            GetInfo();
        }
    }

    return 0;
}