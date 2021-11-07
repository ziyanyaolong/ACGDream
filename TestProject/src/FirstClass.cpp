#include "FirstClass.h"

FirstClass::FirstClass(QObject* parent)
    : PluginCalInterface(parent)
{
    _mui_ = new mui();
    view = new QWebEngineView(_mui_);
    view->resize(_mui_->width(), _mui_->height());
    view->setUrl(QUrl("https://steamcommunity.com/workshop/browse/?appid=294100"));
    addGui(_mui_);
}

FirstClass::~FirstClass()
{
    //_mui_->close();
}

void FirstClass::createFile(QString filePath, QString fileName)
{
    //QDir tempDir;
    ////临时保存程序当前路径
    //QString currentDir = tempDir.currentPath();
    ////如果filePath路径不存在，创建它
    //if (!tempDir.exists(filePath))
    //{
    //    qDebug() << "不存在该路径";
    //    tempDir.mkpath(filePath);
    //}
    //QFile* tempFile = new QFile;
    ////将程序的执行路径设置到filePath下
    //tempDir.setCurrent(filePath);
    //qDebug() << tempDir.currentPath();
    ////检查filePath路径下是否存在文件fileName,如果停止操作。
    //if (tempFile->exists(fileName))
    //{
    //    qDebug() << "文件存在";
    //    tempFile->remove(filePath);
    //}
    ////此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
    //tempFile->setFileName(fileName);
    //if (!tempFile->open(QIODevice::WriteOnly | QIODevice::Text))
    //{
    //    qDebug() << "打开失败";
    //}
    //tempFile->write("class KKK\n{ \n\tpublic: KKK() {} \n};\n");
    //tempFile->close();
    ////将程序当前路径设置为原来的路径
    //tempDir.setCurrent(currentDir);
    //qDebug() << tempDir.currentPath();
}