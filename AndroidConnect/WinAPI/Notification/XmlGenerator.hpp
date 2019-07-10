#pragma once
#include "DesktopNotificationManagerCompat.h"
#include <SDKDDKVer.h>
#include <string>
#include <windows.ui.notifications.h>
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include <NotificationActivationCallback.h>
#include <windows.ui.notifications.h>
#include "../../Plugins/Notification.pb.h"

namespace AndroidConnect {
	class XmlGenerator {
	public:
		static HRESULT SetNodeValueString(HSTRING inputString, ABI::Windows::Data::Xml::Dom::IXmlNode* node, ABI::Windows::Data::Xml::Dom::IXmlDocument* xml);
		static HRESULT Decode(AndroidConnect::Notification& notificationInfo, ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml);
		static HRESULT SetImageSrc(PCWSTR imagePath, ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml);
	};
}