#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
//有时候用messagebox输出一个字符调试程序，查看结果
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->hide();//隐藏菜单栏
    ui->statusBar->hide();//隐藏状态栏

    DeletePointer(pPE);
}

MainWindow::~MainWindow()
{
    delete ui;
    DeletePointer(pPE);
}

//说明：QFileDialog::getOpenFileName本来最初设计在PEfile类中，但是
//第一个参数传递this指针传递的是PEfile类的指针，需要改为本文件中类的指针
//索性就把该实现方法放到了MainWindow类中
//放到PEfile类中的时候，传递参数this改为0也可以编译无错误，只尝试了依次，没有
//看到窗口弹出//谨记述，以备后续研究人员的一丁点参考价值//
string MainWindow::QOpenFile()
{
    QString QfileName = QFileDialog::getOpenFileName(this,
        tr("open file"),"",tr("exe(*.exe);;All(*.*)"));
    //qstring转换为标准C++string类型
    string fileName=QfileName.toStdString();
    ui->QEditFileOpen->setText(QfileName);
    return fileName;
}
void MainWindow::on_pushButton_clicked()
{
    //获取打开文件名称
    string fileName=this->QOpenFile();
    void *p=NULL;
    //开辟PEfile类内存分配给成员pPE
    //错误记录//容易写错为new(PEfile(fileName))，出现的错误为
    //Expected type-specifier before ';' token
    pPE=new PEfile(fileName);
    InitialPEinfo();
    ShowDosHeader(16);
    ShowSectionsInfo(16);
}
void MainWindow::ShowDosHeader(int numBase)
{
    //转换为16进制的QString
    QString numPre;
    if (numBase==16) numPre="0x";
    if (numBase==10) numPre="";

    if ((numBase!=10)&&(numBase!=16))
        QMessageBox::information(0,"显示进制设置错误",QString::number(numBase,10));
    else
    {
        ui->QEditNTHeadersOffset->setText(numPre+QString::number(this->DosHeader.e_lfanew,numBase).toUpper());
        ui->QEditSectionNum->setText(numPre+QString::number(this->NtHeaders.FileHeader.NumberOfSections,numBase).toUpper());
        ui->QEditImageBase->setText(numPre+QString::number(this->NtHeaders.OptionalHeader.ImageBase,numBase).toUpper());
        ui->QEditImageSize->setText(numPre+QString::number(this->NtHeaders.OptionalHeader.SizeOfImage,numBase).toUpper());
        ui->QEditFileAliment->setText(numPre+QString::number(this->NtHeaders.OptionalHeader.FileAlignment,numBase).toUpper());
        ui->QEditMemoryAliment->setText(numPre+QString::number(this->NtHeaders.OptionalHeader.SectionAlignment,numBase).toUpper());
        ui->QEditCodeStart->setText(numPre+QString::number(this->NtHeaders.OptionalHeader.BaseOfCode,numBase).toUpper());
        ui->QEditDataStart->setText(numPre+QString::number(this->NtHeaders.OptionalHeader.BaseOfData,numBase).toUpper());
     }

}
 void MainWindow::InitialPEinfo()
 {

     DosHeader=this->pPE->PEGetDosHeader();
     NtHeaders=this->pPE->PEGetNtHeaders();
 }

void MainWindow::on_pushButton_Hex_clicked()
{
    if (pPE==NULL)
        QMessageBox::information(0,"Attention","先打开文件");
    else
    {
        ShowDosHeader(16);
        ShowSectionsInfo(16);
    }
}
void MainWindow::checkFileOpen()
{
    if (pPE==NULL)
        QMessageBox::information(0,"Attention","先打开文件");
}

void MainWindow::on_pushButton_Dec_clicked()
{
    if (pPE==NULL)
        QMessageBox::information(0,"Attention","先打开文件");
    else
    {
        ShowDosHeader(10);
        ShowSectionsInfo(10);
    }
}
void MainWindow::DeletePointer(void *p)
{
    if (p!=NULL) delete p;
}
void MainWindow::ShowSectionsInfo(int numBase)
{
//    qDebug("%x",sectionNum);
//    ui->setupUi(this);
//    百度解释该句：setupUi(this)是由.ui文件生成的类的构造函数
//    作用是对界面进行初始化，它按照我们在Qt设计器里设计的样子把窗体画出来
//    把我们在Qt设计器里面定义的信号和槽建立起来
//    也可以说，setupUi 是我们画界面和写程序之间的桥梁
    //转换为16进制的QString

    //设置表头字体加粗
    QFont font = ui->tableWidgetSections->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidgetSections->horizontalHeader()->setFont(font);

    ui->tableWidgetSections->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidgetSections->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidgetSections->verticalHeader()->setDefaultSectionSize(20); //设置行高
//    ui->tableWidgetSections->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）

    QString numPre;
    if (numBase==16) numPre="0x";
    if (numBase==10) numPre="";

    if ((numBase==10)||(numBase==16))
    {
        int sectionNum=NtHeaders.FileHeader.NumberOfSections;
        ui->tableWidgetSections->setColumnCount(3);
        ui->tableWidgetSections->setRowCount(sectionNum);
        ui->tableWidgetSections->setHorizontalHeaderLabels(QStringList()<<"区段"<<"区段起始offset"<<"区段大小");
        ui->tableWidgetSections->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
        ui->tableWidgetSections->setSelectionMode(QAbstractItemView::SingleSelection);//设置为可以选中单个
        ui->tableWidgetSections->setStyleSheet("selection-background-color:lightblue;");//设置选中背景色
        ui->tableWidgetSections->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色

        IMAGE_SECTION_HEADER * pSectionHeaderFirst=this->pPE->PEGetFirstSectionPointer();
        IMAGE_SECTION_HEADER * pSection=NULL;
        //section name长度为8个字符，该字符串末尾不带零，每个字符为1个字节，转换为末尾为NULL的字符串
        //采用字节复制的方式
        char pName[9];
        for (int i=0;i<sectionNum;i++)
        {
            pSection=this->pPE->PEGetSectionPointer(pSectionHeaderFirst,i);
            memcpy (pName,pSection->Name,8);
            pName[8]=NULL;
            ui->tableWidgetSections->setItem(i,0,new QTableWidgetItem(pName));
            ui->tableWidgetSections->setItem(i,1,new QTableWidgetItem(numPre+(QString::number(pSection->PointerToRawData,numBase)).toUpper()));
            ui->tableWidgetSections->setItem(i,2,new QTableWidgetItem(numPre+QString::number(pSection->SizeOfRawData,numBase).toUpper()));
        }
    }
    else
    {QMessageBox::information(0,"显示进制设置错误",QString::number(numBase,10));}

}

//void MainWindow::contextMenuEvent(QContextMenuEvent *event)
//{
//    QMenu * Context = new QMenu();
////    Context->addAction(ui->actionCut);
////    Context->addAction(ui->actionCope);
////    Context->addAction(ui->actionPase);
//    Context->addMenu("just fuck it");
////    spreadsheet->setContextMenuPolicy(Qt::DefaultContextMenu);
//    Context->exec(QCursor::pos());
//}
