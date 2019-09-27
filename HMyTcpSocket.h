#ifndef HMYTCPSOCKET_H
#define HMYTCPSOCKET_H

/*
 * ----------------------------------------
 * | 8 bit  |   data                      |
 * ----------------------------------------
 * �����ݽ����˼򵥵ķ����ǰ��8λ��ʾ�������ݵĳ��ȣ���ȡ��д��ʱȫ�����ʽ���У�
*/
#include <QObject>
#include <QTcpSocket>

class HMyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit HMyTcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~HMyTcpSocket();

private:
    qintptr socketID;//����id��

signals:

public slots:
    void sendData(QString data, const int); //�������ݣ������˼򵥵ķ����
    void disConnectTcp(int id);             //�Ͽ�����
};

#endif // HMYTCPSOCKET_H
