#include "pefile.h"
#include <QObject>
#include <QFileDialog>
//ifstream使用的头文件
#include <fstream>
//调试程序输出包含头文件
#include <QDebug>
//弹窗口需要包含的头文件
#include <QMessageBox>

PEfile::PEfile(string fileName)
{
    //计算文件大小
    int fileSize=this->PECalFileSize(fileName);
    //开辟文件大小内存
    pFile=(void *)PEMalloc(fileSize,__LINE__);
    //开辟映射数据内存
    pPEMap=(MAP_FILE_STRUCT *)PEMalloc(sizeof(MAP_FILE_STRUCT),__LINE__);
    //DOS头开辟内存
    pPEMap->pImage_Dos_Header=(IMAGE_DOS_HEADER *)PEMalloc(sizeof(IMAGE_DOS_HEADER),__LINE__);
    //设置文件大小
    pPEMap->dwFileSize=fileSize;
    //装载文件
    PELoadFile(fileName);
    this->PESetDosHeader();
}
 MAP_FILE_STRUCT * PEfile::PEGetInfo()
 {
     return NULL;
 }
 bool PEfile::PESetInfo(MAP_FILE_STRUCT *pPEMap)
 {
     return true;
 }
PEfile::~PEfile()
{
    DeletePointer(pFile);
    DeletePointer(pFile);
}
void * PEfile::PEMalloc(int MemSize,int LineNum)
{
    void *p=(void *)malloc(MemSize);
    if (p==NULL)
        QMessageBox::information(0,"错误",QString::number(LineNum,10));
    else
        return p;
}
//设置文件大小
void PEfile::PESetFileSize(int fileSize)
{
    pPEMap->dwFileSize=fileSize;
}


int PEfile::PECalFileSize(string fileName)
{
    std::ifstream fin(fileName.c_str(),std::ios::binary);
    int fileBegin=fin.tellg();
    //设置文件指针到文件流尾部
    fin.seekg(0,ios::end);
    //获取文件当前指向位置
    int fileEnd=fin.tellg();
    int fileSize=fileEnd-fileBegin;
    //文件重新定位到开始
    fin.seekg(0,ios::beg);
    fin.close();
    return fileSize;
}
int PEfile::PEGetFileSize()
{
    return pPEMap->dwFileSize;
}
bool PEfile::PELoadFile(string fileName)
{
    std::ifstream fin(fileName.c_str(),std::ios::binary);
    fin.read((char *)(this->pFile),this->PEGetFileSize());
    return true;
}

IMAGE_DOS_HEADER * PEfile::PESetDosHeader()
{
    pPEMap->pImage_Dos_Header=(IMAGE_DOS_HEADER *)pFile;
//  qDebug("DOS头标记%x",pPEMap->pImage_Dos_Header->e_magic);
    return (pPEMap->pImage_Dos_Header);
}
IMAGE_DOS_HEADER PEfile::PEGetDosHeader()
{
    return *(pPEMap->pImage_Dos_Header);
}

void PEfile::PETest()
{
    QMessageBox::information(0,"错误","void* PEfile::PEMapMalloc()");
}

IMAGE_NT_HEADERS PEfile::PEGetNtHeaders()
{
    IMAGE_DOS_HEADER *pDos=PESetDosHeader();
    //此处需特别注意，(void *)pDos做指针加减法的时候，要把)pDos转换为void *，如果不转换，
    //可能（当不转换为void *时）编译器在计算过程中，把pDos当作一个数组计算了
    //类似于pDos+4将翻译为(void *)pDos+4*sizeof(某结构体或数组的基本元素大小)
    IMAGE_NT_HEADERS *pNTHeader=(IMAGE_NT_HEADERS *)((void *)pDos+pPEMap->pImage_Dos_Header->e_lfanew);
    return *(pNTHeader);
}

void PEfile::DeletePointer(void *p)
{
    if (p!=NULL) delete p;
}

IMAGE_SECTION_HEADER * PEfile::PEGetFirstSectionPointer()
{


    //该处IMAGE_NT_HEADERS指针摘自函数PEGetNtHeaders()
    IMAGE_DOS_HEADER * pDos=pPEMap->pImage_Dos_Header;
    IMAGE_NT_HEADERS *pNTHeader=(IMAGE_NT_HEADERS *)((void *)pDos+pDos->e_lfanew);
//    IMAGE_NT_HEADERS *pNTHeader=((IMAGE_NT_HEADERS *)(this->pPEMap+this->pPEMap->pImage_Dos_Header->e_lfanew));
    IMAGE_SECTION_HEADER *pSectionHeaderFirst=(IMAGE_SECTION_HEADER *)((void *)pNTHeader
            +sizeof(pNTHeader->Signature)
            +sizeof(pNTHeader->FileHeader)
            +pNTHeader->FileHeader.SizeOfOptionalHeader);
    //返回第一个sectionHeader指针
    return pSectionHeaderFirst;
}

//此处约定，sectionHeader计数从零开始
IMAGE_SECTION_HEADER * PEfile::PEGetSectionPointer(IMAGE_SECTION_HEADER *pSectionHeaderFirst,int nSection)
{
    return (IMAGE_SECTION_HEADER *)(((void *)pSectionHeaderFirst)+sizeof(IMAGE_SECTION_HEADER)*nSection);
}
