#include "http.h"
#include <QTimer>


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "cryptstring.h"

HttpReq::HttpReq(QWidget *parent)
    : QWidget(parent)

{
    m_manager = new QNetworkAccessManager(this);

}

HttpReq::~HttpReq()
{

}


QUrl HttpReq::genPTZConRequestTest(const QString &vlaue)
{
    KVQuery kvs;

    kvs.add("channel", "0");
    kvs.add("value", vlaue.toStdString());

    QString tmp = QString::fromStdString(kvs.toString());
    // qDebug() << "before crypt tmp:" << tmp << Qt::endl;
    // ------------------------------------------------
    CryptString cryptStr(tmp);
    QString token = cryptStr.generateMD5();
    // qDebug() << "token:" << token << Qt::endl;//生成token
    kvs.add("token", token.toStdString());//加入参数
    kvs.erase("secret");//去掉secret
    //重新生成query
    tmp = QString::fromStdString(kvs.toString());
    // qDebug() << "query:" << tmp << Qt::endl;
    // ------------------------------------------------
    QUrl url("http://192.168.5.222/xsw/api/ptz/control");
    url.setQuery(tmp);
    // qDebug() << "url:" << url.toString() << Qt::endl;
    return url;
}


void HttpReq::movePTZ(const QString &value)
{
    //获取文本，也就是网站字符串
    QUrl url(genPTZConRequestTest(value));


    //构建请求的QNetworkRequest对象，需要通过url
    QNetworkRequest request(url);

    //发送请求给服务器（此处的服务器可以就是浏览器）
    QNetworkReply *preply = m_manager->get(request);
    //此外, 发QNetworkReply还有⼀个重要的信号finished会在客户端 收到完整的响应数据 之后触发。
    connect(preply, &QNetworkReply::finished, [=]{
        if(preply->error() == QNetworkReply::NoError)
        {
            //获取传递回来的信息
            QByteArray ba = preply->readAll();
            qDebug() << QString::fromUtf8(ba);
            // ui->textEdit->setText(QString::fromUtf8(ba));
        }
        else
        {
            //获取错误信息
            QString errMsg = preply->errorString();
            qDebug() << errMsg;
        }
    });

    // 添加超时处理
    QTimer::singleShot(5000, [preply]() {
        if(preply && preply->isRunning()) {
            preply->abort();
        }
    });

    // 添加错误处理
    connect(preply, &QNetworkReply::errorOccurred, [=](QNetworkReply::NetworkError code){
        qDebug() << "Network error:" << code;
        preply->deleteLater();
    });
}

