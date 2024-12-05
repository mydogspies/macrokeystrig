#include "mainwindow.h"
#include <iostream>
#include "./ui_mainwindow.h"
#include <Windows.h>
#include <fstream>
#include "json.hpp"

// for convenience
using json = nlohmann::json;

/*
Macrokeytrig by https://github.com/mydogspies
version 0.0.2
Simple utility to trigger scripts using "ch57x-keyboard-tool" from kriomant.
https://github.com/kriomant/ch57x-keyboard-tool
This utility requires the following libs;
https://github.com/nlohmann/json, together with the correct version "json.hpp" in the root folder
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    json data = jsonData();
    // button listeners
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    connect(ui->pushButton_5,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
    // button text
    ui->pushButton_2->setText(QString::fromStdString(data["1"]["app"]));
    ui->pushButton_3->setText(QString::fromStdString(data["2"]["app"]));
    ui->pushButton_4->setText(QString::fromStdString(data["3"]["app"]));
    ui->pushButton_5->setText(QString::fromStdString(data["4"]["app"]));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// get the setup data
json MainWindow::jsonData() {
    std::ifstream f("..\\mktsetup.json");
    json data = json::parse(f);
    return data;
}

// actions on button clicked
void MainWindow::SlotButtonClicked() {

    auto sender = this->sender();

    const std::wstring pathToExe = charToWString("..\\ch57x-keyboard-tool.exe");

    if ( sender == ui->pushButton ) {
        std::cout << "Setup" << std::endl;

    } else if ( sender == ui->pushButton_2 ) {
        std::cout << "affinity photo" << std::endl;


    } else if ( sender == ui->pushButton_3 ) {
        std::cout << "dxo photolab" << std::endl;

    } else if ( sender == ui->pushButton_4 ) {
        std::cout << "MSFS 2020" << std::endl;

    } else if ( sender == ui->pushButton_5 ) {
        std::cout << "ffxiv" << std::endl;

        ExecuteProcess(pathToExe,
            MainWindow::charToWString("upload ..\\3x1ffxiv.yaml")
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

// process to load the yaml file into the keyboard programming app
// see https://github.com/kriomant/ch57x-keyboard-tool
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