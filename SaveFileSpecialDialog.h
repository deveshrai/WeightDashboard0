#ifndef SAVEFILESPECIALDIALOG_H
#define SAVEFILESPECIALDIALOG_H
#pragma once
#include <QString>

class SaveFileSpecialDialog
{
public:
    static QString getSaveFileName();

    static QString saveFile(const QByteArray &data, const QString &suggestedName);
};


#endif // SAVEFILESPECIALDIALOG_H
