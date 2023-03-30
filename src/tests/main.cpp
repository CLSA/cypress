#include <QTest>
#include "tst_grip_strength.h"

int main(int argc, char *argv[])
{
    int status = 0;

    status |= QTest::qExec(new TestGripStrength, argc, argv);

    return status;
}
