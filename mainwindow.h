#include <QMainWindow>

#include "json.hpp"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void SlotButtonClicked();

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static nlohmann::json jsonData();

    std::wstring charToWString(const char *text);

    size_t ExecuteProcess(std::wstring FullPathToExe, std::wstring Parameters, size_t SecondsToWait);

    void ExecScript();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

