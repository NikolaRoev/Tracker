#include "pch.h"
#include "Utility.h"

QString util::current_datetime() {
	return QDateTime::currentDateTime().toString("yyyy-MMMM-dd hh:mm:ss");
}
