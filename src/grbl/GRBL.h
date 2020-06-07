//
// Created by Jakub Gert on 01/06/2020.
//

#ifndef GRBL_GRBL_H
#define GRBL_GRBL_H

#include <QObject>
#include <QString>
#include <QVector3D>

#include "src/grbl/parsers/status_report/accessory_state/AccessoryStateParser.h"
#include "src/grbl/parsers/status_report/buffer_state/BufferStateParser.h"
#include "src/grbl/parsers/status_report/feed_rate/FeedRateParser.h"
#include "src/grbl/parsers/status_report/feed_rate_spindle_speed/FeedRateAndSpindleSpeedParser.h"
#include "src/grbl/parsers/status_report/input_pin_state/InputPinStateParser.h"
#include "src/grbl/parsers/status_report/line_number/LineNumberParser.h"
#include "src/grbl/parsers/status_report/machine_state/MachineStateParser.h"
#include "src/grbl/parsers/status_report/overridden_values/OverriddenValuesParser.h"
#include "src/grbl/parsers/status_report/position_parser/PositionParser.h"

class GRBL : public QObject {
Q_OBJECT

private:
    AccessoryStateParser accessoryStateParser;
    BufferStateParser bufferStateParser;
    FeedRateParser feedRateParser;
    FeedRateAndSpindleSpeedParser feedRateAndSpindleSpeedParser;
    InputPinStateParser inputPinStateParser;
    LineNumberParser lineNumberParser;
    MachineStateParser machineStateParser;
    OverriddenValuesParser overriddenValuesParser;
    PositionParser positionParser;

    AccessoryState accessoryState;
public:
    QStringList error;
private:
    
    bool parseStatus(const QString &message);

public:

    void parse(const QByteArray &data);

signals:

    void onReceivedAccessoryState(AccessoryState state);

    void onReceivedBufferState(BufferState state);

    void onReceivedFeedRate(unsigned int feedRate);

    void onReceivedSpindleSpeed(unsigned int spindleSpeed);

    void onReceivedInputPinState(InputPinState inputPinState);

    void onReceivedLineNUmber(unsigned int lineNumber);

    void onReceivedMachineState(MachineState state);

    void onReceivedOverriddenValues(OverriddenValues overriddenValues);

    void onReceivedMPos(const vec3 &position);

    void onReceivedWPos(const vec3 &position);

};


#endif //GRBL_GRBL_H