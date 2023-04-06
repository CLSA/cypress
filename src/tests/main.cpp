#include <QTest>

#include "tst_grip_strength.h"
#include "tst_retinal_camera.h"

int main(int argc, char *argv[])
{
    int status = 0;

    status |= QTest::qExec(new TestGripStrength, argc, argv);
    status |= QTest::qExec(new TestRetinalCamera, argc, argv);

    return status;
}
