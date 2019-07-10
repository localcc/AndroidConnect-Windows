#include "rttr/registration.h"
#include "WinAPI/Notification/NotificationHelper.hpp"

//Register your classes here

RTTR_REGISTRATION{
	rttr::registration::class_<AndroidConnect::NotificationHelper>("Notification")
	.constructor<void>().method("Invoke", &AndroidConnect::NotificationHelper::SendNotification);
}