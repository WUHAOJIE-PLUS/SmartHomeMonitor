#ifndef CIPHERTEXT_H
#define CIPHERTEXT_H

#include <QCryptographicHash>
#include <QString>

class CIPHERString
{
public:
    CIPHERString(const QString & data);
    QString generateMD5() const;

private:
    QString _data;
};
#endif // CIPHERTEXT_H
