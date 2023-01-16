#include <QTest>
#include "tst_audiometer.h"
#include "tst_blood_pressure.h"
#include "tst_body_composition.h"
#include "tst_cdtt.h"
#include "tst_choice_reaction.h"
#include "tst_dxa.h"
#include "tst_ecg.h"
#include "tst_emr.h"
#include "tst_frax.h"
#include "tst_grip_strength.h"
#include "tst_retinal_camera.h"
#include "tst_tonometer.h"
#include "tst_ultrasound.h"
#include "tst_weigh_scale.h"

int main(int argc, char *argv[])
{
    int status = 0;

    status |= QTest::qExec(new TestAudiometer, argc, argv);
    status |= QTest::qExec(new TestBloodPressure, argc, argv);
    status |= QTest::qExec(new TestBodyComposition, argc, argv);
    status |= QTest::qExec(new TestCDTT, argc, argv);
    status |= QTest::qExec(new TestChoiceReaction, argc, argv);
    status |= QTest::qExec(new TestDxa, argc, argv);
    status |= QTest::qExec(new TestEcg, argc, argv);
    status |= QTest::qExec(new TestEmr, argc, argv);
    status |= QTest::qExec(new TestFrax, argc, argv);
    status |= QTest::qExec(new TestGripStrength, argc, argv);
    status |= QTest::qExec(new TestRetinalCamera, argc, argv);
    status |= QTest::qExec(new TestTonometer, argc, argv);
    status |= QTest::qExec(new TestUltrasound, argc, argv);
    status |= QTest::qExec(new TestWeighScale, argc, argv);

    return status;
}
