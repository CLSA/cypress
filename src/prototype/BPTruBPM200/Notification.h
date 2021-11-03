#pragma once

#include "BPMMessage.h"
#include "BPTruBPM200.h"
#include "NotificationEnum.h"

class Notification
{
public:
	static QString GetPrintableType(BPMMessage* msg) {
		switch (msg->GetData0()) {
		case (int)NotificationEnum::Reset:
			return "Reset";
		default:
			return msg->GetFullMsg();
		}
	}

	static void Interpret(BPMMessage* msg, Ui::BPTruBPM200Class* bpm200) {
		switch (msg->GetData0()) {
		case 8:
			break;
		}
	}
private:
	static void Reset(BPMMessage* msg, Ui::BPTruBPM200Class* bpm200) {
		bool isPowerUp = msg->GetData1();
		qDebug() << "Reset notification. Is power up = " << (isPowerUp ? "True" : "False") << endl;
	}
};
