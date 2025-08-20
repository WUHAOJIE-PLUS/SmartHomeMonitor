#ifndef HTTP_H
#define HTTP_H

#include <QWidget>
#include <QNetworkAccessManager>


class HttpReq : public QWidget
{
    Q_OBJECT

public:
    HttpReq(QWidget *parent = nullptr);
    ~HttpReq();
    QUrl genPTZConRequestTest(const QString &vlaue);

// private slots:
    void movePTZ(const QString &value);

private:
    QNetworkAccessManager *m_manager;
};
#endif // HTTP_H
