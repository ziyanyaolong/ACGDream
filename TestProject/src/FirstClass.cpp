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
    ////��ʱ�������ǰ·��
    //QString currentDir = tempDir.currentPath();
    ////���filePath·�������ڣ�������
    //if (!tempDir.exists(filePath))
    //{
    //    qDebug() << "�����ڸ�·��";
    //    tempDir.mkpath(filePath);
    //}
    //QFile* tempFile = new QFile;
    ////�������ִ��·�����õ�filePath��
    //tempDir.setCurrent(filePath);
    //qDebug() << tempDir.currentPath();
    ////���filePath·�����Ƿ�����ļ�fileName,���ֹͣ������
    //if (tempFile->exists(fileName))
    //{
    //    qDebug() << "�ļ�����";
    //    tempFile->remove(filePath);
    //}
    ////��ʱ��·����û��fileName�ļ���ʹ����������ڵ�ǰ·���´����ļ�
    //tempFile->setFileName(fileName);
    //if (!tempFile->open(QIODevice::WriteOnly | QIODevice::Text))
    //{
    //    qDebug() << "��ʧ��";
    //}
    //tempFile->write("class KKK\n{ \n\tpublic: KKK() {} \n};\n");
    //tempFile->close();
    ////������ǰ·������Ϊԭ����·��
    //tempDir.setCurrent(currentDir);
    //qDebug() << tempDir.currentPath();
}