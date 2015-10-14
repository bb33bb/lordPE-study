#ifndef PEFILE_H
#define PEFILE_H

#include <QString>
//文件打开函数QFileDialog::getOpenFileName需要包含的头文件
//PE头数据结构
#include <peinfo.h>
//标准C++处理字符串需要包含的头文件
#include <string>
using namespace std;
//标准C++打开文件需要的头文件std::ifstream
#include <peinfo.h>

typedef struct _MAP_FILE_STRUCT
{
    IMAGE_DOS_HEADER *pImage_Dos_Header;
    //HANDLE hFile;         //打开的文件句柄
    //HANDLE hMapping;      //文件映射到内存的句柄
    //void ImageBase;       //文件的数据
    DWORD dwFileSize;       //文件的大小
}  MAP_FILE_STRUCT,*PMAP_FILE_STRUCT;

//文件打开时的处理方式
enum {FILE_FROM_NOMAL=0,FILE_FROM_FILE=1,FILE_FROM_MEMORY=2};


class PEfile
{
public:

    PEfile(string fileName);
    ~PEfile();
    //计算打开文件大小
    int PECalFileSize(string fileName);
    //读取文件到内存
    bool PELoadFile(string fileName);
    //文件大小已经存储在数据结构中以后，获取文件大小
    int PEGetFileSize();
    //设置文件大小到类成员
    void PESetFileSize(int fileSize);
    //获取文件信息
    MAP_FILE_STRUCT * PEGetInfo();
    //设置文件信息
    bool PESetInfo(MAP_FILE_STRUCT *pPEMap);
    //释放内存
    void DeletePointer(void *p);
    //分配内存函数模板
    void* PEMalloc(int MemSize,int LineNum);

    //设置dosheader32//
    IMAGE_DOS_HEADER * PESetDosHeader();
    //获取文件的DOS HEADER结构体
    IMAGE_DOS_HEADER PEGetDosHeader();
    //获取文件的IMAGE_FILE_HEADER结构体
    IMAGE_NT_HEADERS PEGetNtHeaders();
    //获取第一个区段头IMAGE_SECTION_HEADER结构体
    IMAGE_SECTION_HEADER * PEGetFirstSectionPointer();
    //获取第n个去断头IMAGE_SECTION_HEADER结构体
    IMAGE_SECTION_HEADER * PEGetSectionPointer(IMAGE_SECTION_HEADER *pSectionHeaderFirst,int nSection);
    //有时候单独测试几个语句，便放在这个函数中直接调用了
    void PETest();

private:
    //bLoadStatus标识文件打开状态
    BYTE bLoadStatus;
    //文件部分结构摘要，提取自pFile指向结构
    MAP_FILE_STRUCT * pPEMap=NULL;
    //文件在磁盘所有内容指针
    void *pFile=NULL;
};

#endif // PEFILE_H
