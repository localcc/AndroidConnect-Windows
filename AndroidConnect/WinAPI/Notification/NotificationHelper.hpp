#pragma once

#include "DesktopNotificationManagerCompat.h"
#include <NotificationActivationCallback.h>
#include <windows.ui.notifications.h>



using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::UI::Notifications;
using namespace Microsoft::WRL;

namespace AndroidConnect {


	class NotificationHelper {
	public:
		NotificationHelper();
		~NotificationHelper();
		
		HRESULT SendNotification(void *bytes, int arraySize);

	};
}