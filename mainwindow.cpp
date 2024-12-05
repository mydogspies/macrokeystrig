#include "mainwindow.h"
#include <iostream>
#include "./ui_mainwindow.h"
#include <Windows.h>

/*
Macrokeytrig by https://github.com/mydogspies
version 0.1
Simple utility to trigger scripts using "ch57x-keyboard-tool" from kriomant.
https://github.com/kriomant/ch57x-keyboard-tool
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // temp definition - move to ini file
    QString ** data;
    data = new QString*[5];
    for(int i=0; i<5; i++) data[i] = new QString[2];
    data[0][0] = "affinity photo";
    data[0][1] = "3x1aphoto.yaml";
    data[1][0] = "dxo photolab";
    data[1][1] = "3x1dxolab.yaml";
    data[2][0] = "msfs";
    data[2][1] = "3x1msfs.yaml";

    // button listeners
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    connect(ui->pushButton_5,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// actions on button clicked
void MainWindow::SlotButtonClicked() {

    auto sender = this->sender();

    if ( sender == ui->pushButton ) {
        std::cout << "Setup" << std::endl;

    } else if ( sender == ui->pushButton_2 ) {
        std::cout << "affinity phot" << std::endl;

    } else if ( sender == ui->pushButton_3 ) {
        std::cout << "dxo photolab" << std::endl;

    } else if ( sender == ui->pushButton_4 ) {
        std::cout << "msfs 2020" << std::endl;

    } else if ( sender == ui->pushButton_5 ) {
        std::cout << "FFXIV" << std::endl;

        ExecuteProcess(charToWString("S:\\40_Coding\\01_MIXENV\\macrokeytrigger\\ch57x-keyboard-tool.exe"),
            MainWindow::charToWString("upload S:\\40_Coding\\01_MIXENV\\macrokeytrigger\\3x1ffxiv.yaml")
            ,5);
    }
}

std::wstring MainWindow::charToWString(const char* text)
{
    const size_t size = std::strlen(text);
    std::wstring wstr;
    if (size > 0) {
        wstr.resize(size);
        std::mbstowcs(&wstr[0], text, size);
    }
    return wstr;
}

size_t MainWindow::ExecuteProcess(std::wstring FullPathToExe, std::wstring Parameters, size_t SecondsToWait)
{
    size_t iMyCounter = 0, iReturnVal = 0, iPos = 0;
    DWORD dwExitCode = 0;
    std::wstring sTempStr = L"";

    /* - NOTE - You should check here to see if the exe even exists */

    /* Add a space to the beginning of the Parameters */
    if (Parameters.size() != 0)
    {
        if (Parameters[0] != L' ')
        {
            Parameters.insert(0,L" ");
        }
    }

    /* The first parameter needs to be the exe itself */
    sTempStr = FullPathToExe;
    iPos = sTempStr.find_last_of(L"\\");
    sTempStr.erase(0, iPos +1);
    Parameters = sTempStr.append(Parameters);

    /* CreateProcessW can modify Parameters thus we allocate needed memory */
    wchar_t * pwszParam = new wchar_t[Parameters.size() + 1];
    if (pwszParam == 0)
    {
        return 1;
    }
    const wchar_t* pchrTemp = Parameters.c_str();
    wcscpy_s(pwszParam, Parameters.size() + 1, pchrTemp);

    /* CreateProcess API initialization */
    STARTUPINFOW siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));
    siStartupInfo.cb = sizeof(siStartupInfo);

    if (CreateProcessW(const_cast<LPCWSTR>(FullPathToExe.c_str()),
                            pwszParam, 0, 0, false,
                            CREATE_DEFAULT_ERROR_MODE, 0, 0,
                            &siStartupInfo, &piProcessInfo) != false)
    {
        /* Watch the process. */
        dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, (SecondsToWait * 1000));
    }
    else
    {
        /* CreateProcess failed */
        iReturnVal = GetLastError();
    }

    /* Free memory */
    delete[]pwszParam;
    pwszParam = 0;

    /* Release handles */
    CloseHandle(piProcessInfo.hProcess);
    CloseHandle(piProcessInfo.hThread);

    return iReturnVal;
}