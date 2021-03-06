//
// Created by Jakub Gert on 03/06/2020.
//

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include "tests/TestSuite.h"
#include "src/grbl/GRBL.h"

Q_DECLARE_METATYPE(State)

Q_DECLARE_METATYPE(vec3)

Q_DECLARE_METATYPE(AccessoryState)

Q_DECLARE_METATYPE(BufferState)

Q_DECLARE_METATYPE(InputPinState)

Q_DECLARE_METATYPE(MachineState)

Q_DECLARE_METATYPE(OverriddenValues)

Q_DECLARE_METATYPE(SettingItem)

class GRBLTests : public TestSuite {
Q_OBJECT

private slots:

    void initTestCase() {
        qRegisterMetaType<AccessoryState>();
        qRegisterMetaType<BufferState>();
        qRegisterMetaType<InputPinState>();
        qRegisterMetaType<MachineState>();
        qRegisterMetaType<OverriddenValues>();
        qRegisterMetaType<State>();
        qRegisterMetaType<vec3>();
        qRegisterMetaType<SettingItem>();
    }

    void testIntegrationAlarm() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedAlarm);

        controller.parse("ALARM:0");

        QCOMPARE(spy.count(), 1);
        auto state = qvariant_cast<unsigned int>(spy.takeFirst()[0]);
        QCOMPARE(state, 0);

        controller.parse("ALARM:2");

        QCOMPARE(spy.count(), 1);
        state = qvariant_cast<unsigned int>(spy.takeFirst()[0]);
        QCOMPARE(state, 2);
    }

    void testIntegrationAccessoryState() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedAccessoryState);

        controller.parse("<A:SF>");

        QCOMPARE(spy.count(), 1);
        AccessoryState state = qvariant_cast<AccessoryState>(spy.takeFirst()[0]);
        QCOMPARE(state.spindleState, AccessoryState::SpindleState::enabledCW);
        QCOMPARE(state.floodCoolant, true);
        QCOMPARE(state.mistCoolant, false);
    }

    void testIntegrationBufferState() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedBufferState);

        controller.parse("<Bf:120,10>");

        QCOMPARE(spy.count(), 1);
        auto state = qvariant_cast<BufferState>(spy.takeFirst()[0]);
        QCOMPARE(state.availableBlocks, 120);
        QCOMPARE(state.availableBytesInRxBuffer, 10);
    }

    void testIntegrationFeedRate() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedFeedRate);

        controller.parse("<F:120>");

        QCOMPARE(spy.count(), 1);
        auto state = qvariant_cast<unsigned int>(spy.takeFirst()[0]);
        QCOMPARE(state, 120);
    }

    void testIntegrationFeedRateSpindleSpeed() {
        GRBL controller;

        QSignalSpy spy1(&controller, &GRBL::onReceivedFeedRate);
        QSignalSpy spy2(&controller, &GRBL::onReceivedSpindleSpeed);

        controller.parse("<FS:120,210>");

        QCOMPARE(spy1.count(), 1);
        auto state1 = qvariant_cast<unsigned int>(spy1.takeFirst()[0]);
        QCOMPARE(state1, 120);

        QCOMPARE(spy2.count(), 1);
        auto state2 = qvariant_cast<unsigned int>(spy2.takeFirst()[0]);
        QCOMPARE(state2, 210);
    }

    void testIntegrationInputPinState() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedInputPinState);

        controller.parse("<Pn:XZDHR>");

        QCOMPARE(spy.count(), 1);
        auto state = qvariant_cast<InputPinState>(spy.takeFirst()[0]);
        QCOMPARE(state.xLimit, true);
        QCOMPARE(state.yLimit, false);
        QCOMPARE(state.zLimit, true);
        QCOMPARE(state.probe, false);
        QCOMPARE(state.door, true);
        QCOMPARE(state.hold, true);
        QCOMPARE(state.softReset, true);
        QCOMPARE(state.cycleStart, false);
    }

    void testIntegrationLineNumber() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedLineNumber);

        controller.parse("<Ln:120>");

        QCOMPARE(spy.count(), 1);
        auto state = qvariant_cast<unsigned int>(spy.takeFirst()[0]);
        QCOMPARE(state, 120);
    }

    void testIntegrationMachineStateValid() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedMachineState);

        controller.parse("<Door:1>");

        QCOMPARE(spy.count(), 1);
        MachineState state = qvariant_cast<MachineState>(spy.takeFirst()[0]);
        QCOMPARE(state.state, State::Door);
        QCOMPARE(state.code, 1);
    }

    void testIntegrationMachineStateInvalid() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedMachineState);

        controller.parse("<FooState>");

        QCOMPARE(spy.count(), 0);
    }

    void testIntegrationOverriddenValues() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedOverriddenValues);

        controller.parse("<Ov:1,2,3>");

        QCOMPARE(spy.count(), 1);
        auto state = qvariant_cast<OverriddenValues>(spy.takeFirst()[0]);
        QCOMPARE(state.feed, 1);
        QCOMPARE(state.rapid, 2);
        QCOMPARE(state.spindle, 3);
    }

    void testIntegrationWPos() {
        GRBL controller;

        QSignalSpy spyMPos(&controller, &GRBL::onReceivedMPos);
        QSignalSpy spyWPos(&controller, &GRBL::onReceivedWPos);

        controller.parse("<MPos:10,20,30|WCO:50.000,60.00,70.00>");

        QCOMPARE(spyMPos.count(), 2);
        QCOMPARE(spyWPos.count(), 2);

        QCOMPARE(qvariant_cast<vec3>(spyMPos[0].takeFirst()), vec3(10, 20, 30));
        QCOMPARE(qvariant_cast<vec3>(spyWPos[0].takeFirst()), vec3(10, 20, 30));

        QCOMPARE(qvariant_cast<vec3>(spyMPos[1].takeFirst()), vec3(10, 20, 30));
        QCOMPARE(qvariant_cast<vec3>(spyWPos[1].takeFirst()), vec3(-40, -40, -40));
    }

    void testIntegrationMachineMPos() {
        GRBL controller;

        QSignalSpy spyMPos(&controller, &GRBL::onReceivedMPos);
        QSignalSpy spyWPos(&controller, &GRBL::onReceivedWPos);

        controller.parse("<WPos:10,20,30|WCO:50.000,60.00,70.00>");

        QCOMPARE(spyMPos.count(), 2);
        QCOMPARE(spyWPos.count(), 2);

        QCOMPARE(qvariant_cast<vec3>(spyMPos[0].takeFirst()), vec3(10, 20, 30));
        QCOMPARE(qvariant_cast<vec3>(spyWPos[0].takeFirst()), vec3(10, 20, 30));

        QCOMPARE(qvariant_cast<vec3>(spyMPos[1].takeFirst()), vec3(60, 80, 100));
        QCOMPARE(qvariant_cast<vec3>(spyWPos[1].takeFirst()), vec3(10, 20, 30));
    }

    void testIntegrationHelpParser() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedHelp);

        controller.parse("[HLP:$$ $# $G $I $N $x=val $Nx=line $J=line $SLP $C $X $H ~ ! ? ctrl-x]");

        QStringList expected(
                {
                        "$$", "$#", "$G", "$I", "$N",
                        "$x=val", "$Nx=line", "$J=line",
                        "$SLP", "$C", "$X",
                        "$H", "~", "!", "?", "ctrl-x"
                }
        );

        QCOMPARE(spy.count(), 1);
        QCOMPARE(qvariant_cast<QStringList>(spy[0].takeFirst()), expected);
    }

    void testIntegrationWelcomeMessage() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedWelcomeMessage);

        controller.parse("Grbl 1.1h ['$' for help]");

        QCOMPARE(spy.count(), 1);
        auto state = qvariant_cast<QString>(spy.takeFirst()[0]);
        QCOMPARE(state, "1.1h");
    }

    void testIntegrationSetting() {
        GRBL controller;

        QSignalSpy spy(&controller, &GRBL::onReceivedSetting);

        controller.parse("$10=20");

        QCOMPARE(spy.count(), 1);
        auto state = qvariant_cast<SettingItem>(spy.takeFirst()[0]);
        QCOMPARE(state.code, 10);
        QCOMPARE(state.value, "20");
    }
};

static GRBLTests T_GRBLTests;


#include "GRBL.test.moc"
