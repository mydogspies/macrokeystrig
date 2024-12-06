#include "mainwindow.h"
#include <iostream>
#include "./ui_mainwindow.h"
#include <Windows.h>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

/*
Macrokeytrig by https://github.com/mydogspies
version 0.0.2
Simple utility to trigger yaml config files for "ch57x-keyboard-tool" from kriomant.
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
    this->setWindowTitle("MacroKeyTrig v.0.0.3");

    // get the button map, then assign actions and states
    std::map<int, QPushButton*> qtButtons = mapBtn();
    for (auto const& [key,val] : qtButtons) {

        // assign actions
        connect(val,&QPushButton::clicked,this,&MainWindow::SlotButtonClicked);
        // assign button text
        if (key > 0) {
            std::string idx = std::to_string(key);
            val->setText(QString::fromStdString(data[idx]["app"]));
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

// reads the setup config data from mktsetup.json
json MainWindow::jsonData() {
    std::ifstream f("..\\mktsetup.json");
    json data = json::parse(f);
    return data;
}

// defines a map of all the gui buttons for later use
std::map<int, QPushButton*> MainWindow::mapBtn() {
    std::map<int, QPushButton*> qtButtons;
    qtButtons[0] = ui->btnSetup;
    qtButtons[1] = ui->btn1;
    qtButtons[2] = ui->btn2;
    qtButtons[3] = ui->btn3;
    qtButtons[4] = ui->btn4;
    qtButtons[5] = ui->btn5;
    qtButtons[6] = ui->btn6;
    qtButtons[7] = ui->btn7;
    qtButtons[8] = ui->btn8;
    qtButtons[9] = ui->btn9;
    qtButtons[10] = ui->btn10;
    return qtButtons;
}

// actions on button clicked
void MainWindow::SlotButtonClicked() {

    auto sender = this->sender();
    json data = jsonData();
    int waitTime = 10;

    // root dir of this app (ch57x-keyboard-tool.exe needs to be in the same dir)
    std::string pathToExe = data["0"]["pathToRoot"];
    pathToExe.append(data["0"]["file"]);

    // define some commands
    std::string cmdUpload = "upload ";
    cmdUpload.append(data["0"]["pathToRoot"]);

    // setup button
    if ( sender == ui->btnSetup ) {
        std::cout << "Setup" << std::endl;

    } else if ( sender == ui->btn1 ) { // button 1

        std::string yamlFile = cmdUpload;
        yamlFile.append(data["1"]["file"]);

        ExecuteProcess(charToWString(pathToExe.c_str()),
            MainWindow::charToWString(yamlFile.c_str())
            ,waitTime);

    } else if ( sender == ui->btn2 ) { // button 2

        std::string yamlFile = cmdUpload;
        yamlFile.append(data["2"]["file"]);

        ExecuteProcess(charToWString(pathToExe.c_str()),
            MainWindow::charToWString(yamlFile.c_str())
            ,waitTime);

    } else if ( sender == ui->btn3 ) { // button 3

        std::string yamlFile = cmdUpload;
        yamlFile.append(data["3"]["file"]);

        ExecuteProcess(charToWString(pathToExe.c_str()),
            MainWindow::charToWString(yamlFile.c_str())
            ,waitTime);

    } else if ( sender == ui->btn4 ) { // button 4

        std::string yamlFile = cmdUpload;
        yamlFile.append(data["4"]["file"]);

        ExecuteProcess(charToWString(pathToExe.c_str()),
            MainWindow::charToWString(yamlFile.c_str())
            ,waitTime);
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
// below code by https://goffconcepts.com/techarticles/createprocess.html
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