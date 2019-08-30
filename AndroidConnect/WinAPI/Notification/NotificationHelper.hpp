#pragma once
#include <Unknwn.h>

#include <combaseapi.h>
#include <NotificationActivationCallback.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.UI.Notifications.Management.h>
#include <winrt/Windows.Data.Xml.Dom.h>

#include <winrt\Windows.Foundation.h>


namespace AndroidConnect {
	class NotificationHelper {
	public:
		NotificationHelper();
		~NotificationHelper();
		void SendNotification(unsigned char* data, int size, unsigned int clientId);
	private:
		void RegisterCom(GUID clsId, const wchar_t* exePath);

		void RegisterInProc(GUID clsId, const wchar_t* dllPath);
		void RegisterAumidAndCom(const wchar_t* aumid, GUID clsId);
	};
}