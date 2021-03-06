//
// Created by Jakub Gert on 16/06/2020.
//

#ifndef GRBL_MESSAGE_H
#define GRBL_MESSAGE_H


#include <QByteArray>
#include <QString>
#include <src/grbl/parsers/setting/SettingParser.h>

struct Message {
    QByteArray data;
    unsigned int id;
private:
    explicit Message(const QByteArray &data);
public:
    static Message custom(const QString &message);
    static Message messageInfo();
    static Message messageKillAlarm();
    static Message messageSetSetting(const SettingItem &item);
    static Message messageSettings();
};


#endif //GRBL_MESSAGE_H
