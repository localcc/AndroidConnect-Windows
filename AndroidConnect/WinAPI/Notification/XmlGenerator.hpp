#pragma once
#include <SDKDDKVer.h>
#include <string>
#include <windows.ui.notifications.h>
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include <NotificationActivationCallback.h>
#include <windows.ui.notifications.h>
#include "../../Plugins/Notification.pb.h"
#include <tinyxml2.h>
#include <winrt/Windows.Data.Xml.Dom.h>

using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::UI::Notifications;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

namespace AndroidConnect {
	class XmlGenerator {
	public:
		static winrt::hstring Decode(AndroidConnect::Notification& notificationInfo, int clientId);
		static std::vector<std::wstring> split(std::wstring in, const wchar_t* delim);
		static std::wstring extract_arg_value(std::wstring arg);
	private:
		static std::string FixStr(const char* str);
	};
}