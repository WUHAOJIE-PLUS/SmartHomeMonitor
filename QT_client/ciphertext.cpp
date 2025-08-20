#include "ciphertext.h"

CIPHERString::CIPHERString(const QString & data)
    : _data(data){}

QString CIPHERString::generateMD5() const
{
    QByteArray arr = QCryptographicHash::hash(_data.toUtf8(), QCryptographicHash::Md5);
    return arr.toHex();
}
