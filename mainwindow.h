#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <pefile.h>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    string QOpenFile();
    void ShowDosHeader(int numBase);
    void InitialPEinfo();
    void DeletePointer(void *p);
    void checkFileOpen();
    void ShowSectionsInfo(int numBase);
//    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_Hex_clicked();

    void on_pushButton_Dec_clicked();

private:
    PEfile *pPE=NULL;
    Ui::MainWindow *ui=NULL;
//    QTableWidget::contextMenuEvent(QContextMenuEvent *event);
    IMAGE_DOS_HEADER DosHeader;
    IMAGE_NT_HEADERS NtHeaders;
    IMAGE_SECTION_HEADER SectionHeader;
};

#endif // MAINWINDOW_H
