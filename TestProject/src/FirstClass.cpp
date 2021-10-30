#include "FirstClass.h"

FirstClass::FirstClass(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    createFile("","sss.h");
}

FirstClass::~FirstClass()
{
}

void FirstClass::createFile(QString filePath, QString fileName)
{
    QDir tempDir;
    //��ʱ�������ǰ·��
    QString currentDir = tempDir.currentPath();
    //���filePath·�������ڣ�������
    if (!tempDir.exists(filePath))
    {
        qDebug() << "�����ڸ�·��";
        tempDir.mkpath(filePath);
    }
    QFile* tempFile = new QFile;
    //�������ִ��·�����õ�filePath��
    tempDir.setCurrent(filePath);
    qDebug() << tempDir.currentPath();
    //���filePath·�����Ƿ�����ļ�fileName,���ֹͣ������
    if (tempFile->exists(fileName))
    {
        qDebug() << "�ļ�����";
        tempFile->remove(filePath);
    }
    //��ʱ��·����û��fileName�ļ���ʹ����������ڵ�ǰ·���´����ļ�
    tempFile->setFileName(fileName);
    if (!tempFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "��ʧ��";
    }
    tempFile->write("class KKK\n{ \n\tpublic: KKK() {} \n};\n");
    tempFile->close();
    //������ǰ·������Ϊԭ����·��
    tempDir.setCurrent(currentDir);
    qDebug() << tempDir.currentPath();
}