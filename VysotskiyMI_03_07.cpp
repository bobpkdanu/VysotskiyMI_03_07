// VysotskiyMI_03_07.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <stdexcept>
using namespace std;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc > 1) {
        int id = atoi(argv[1]); 

        HANDLE hSem = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, "MyComputeSemaphore");

        cout << "Процесс " << id << " ожидает" << endl;
        WaitForSingleObject(hSem, INFINITE);

        cout << "Процесс " << id << " начал вычисления" << endl;
        unsigned long long sum = 0;
        for (unsigned long long i = 1; i <= 100000000; i++) {
            sum += i;
        }
        cout << "Процесс " << id << " закончил, сумма = " << sum << endl;

        ReleaseSemaphore(hSem, 1, NULL); 
        CloseHandle(hSem);
        return 0;
    }

    HANDLE hSem = CreateSemaphoreA(NULL, 2, 2, "MyComputeSemaphore");

    //Узнаем путь к нашему exe файлу
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    PROCESS_INFORMATION pi[4];
    HANDLE handles[4];

    // Запускаем 4 дочерних процесса
    for (int i = 0; i < 4; i++) {
        STARTUPINFOA si = { sizeof(si) };
        char cmd[MAX_PATH + 10];
        sprintf_s(cmd, "\"%s\" %d", exePath, i + 1); 

        CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi[i]);
        handles[i] = pi[i].hProcess;
    }

    // ожидаем завершение 4 дочерних объектов
    WaitForMultipleObjects(4, handles, TRUE, INFINITE);

    // Закрываем все дескрипторы
    for (int i = 0; i < 4; i++) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }
    CloseHandle(hSem);

    cout << "все вычисления завершены, система остановлена" << endl;
    return 0;
}
