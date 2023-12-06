#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <shellapi.h>
#include <tlhelp32.h>

using namespace std;

LPCTSTR gEX;
LPCTSTR oID;
HANDLE GetHndl(LPCTSTR pName);
int Start(LPCTSTR pName, LPCTSTR pArgs);
bool IsRunning(LPCTSTR pName);
void Kill(LPCTSTR pName);

int main(int argc, char **argv)
{
	FreeConsole();

    //determine origin directory
    LPCTSTR oDir;
    if (argv[1]!=NULL){
        oDir = argv[1];}
    else oDir = "C:/PROGRA~2/Epic Games/Launcher/Portal/Binaries/Win32/EpicGamesLauncher.exe";
    //check if Epic is running
    if (IsRunning("EpicGamesLauncher.exe")){
        Kill("EpicGamesLauncher.exe");}
    //start Epic as child program
    if (Start(oDir,"-StartClientMinimized") <= 32){
        MessageBox(0, "Failed to launch Epic. \nIf it's not installed in the default location, put the full path to EpicGamesLauncher.exe in the launch parameters.", "Error", MB_OK);
        return 1;}
    while (!IsRunning("EpicGamesLauncher.exe")){
        Sleep(1000);}
    //wait for Epic to go idle
    Sleep(8000);
    //launch game
    oID = "com.epicgames.launcher://apps/fn%3A4fe75bbc5a674f4f9b356b5c90567da5%3AFortnite?action=launch&silent=true";
    Start(oID, NULL);
    for (int i = 1; i<41; i++){
        Sleep (1000);}
    gEX = "FortniteClient-Win64-Shipping.exe";
    //exit if game does not start after 40s
    if (!IsRunning(gEX)){
        MessageBox(0, "Game doesn't appear to be running. Closing launcher.", "Error", MB_OK);
        Kill("EpicGamesLauncher.exe");
        return 1;}
    //wait for game to close
    while (IsRunning(gEX)){
        Sleep(1000);}
    //give some time for cloud sync
    Sleep(8000);
    //Close Epic
    Kill("EpicGamesLauncher.exe");
    return 0;
}

//--------------- F U N C T I O N S ---------------//

HANDLE GetHndl(LPCTSTR pName){
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry) == TRUE){
        while (Process32Next(snapshot, &entry) == TRUE){
            if (_stricmp(entry.szExeFile, pName ) == 0){
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
            CloseHandle(snapshot);
            return hProcess;}}
    }return 0;
}

int Start(LPCTSTR pName, LPCTSTR pArgs){
    HINSTANCE h = ShellExecute(NULL,"open",pName,pArgs,NULL,SW_SHOW);
    int r = (int) h;
    return r;
}

bool IsRunning(LPCTSTR pName){
    if (GetHndl(pName)==0)
        return 0;
    return 1;
}

void Kill(LPCTSTR pName){
    HANDLE hProcess = GetHndl(pName);
    TerminateProcess(hProcess, 0 );
}

