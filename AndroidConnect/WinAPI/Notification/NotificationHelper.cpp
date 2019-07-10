#include "NotificationHelper.hpp"
#include <cstdio>
#include "../../Plugins/Notification.pb.h"
#include "XmlGenerator.hpp"



namespace AndroidConnect {
	class DECLSPEC_UUID("426CDE19-CFC8-4DA3-A2E7-D4F36039D47C") NotificationActivator WrlSealed WrlFinal
		: public RuntimeClass<RuntimeClassFlags<ClassicCom>, INotificationActivationCallback>
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE Activate(
			_In_ LPCWSTR appUserModelId,
			_In_ LPCWSTR invokedArgs,
			_In_reads_(dataCount) const NOTIFICATION_USER_INPUT_DATA * data,
			ULONG dataCount) override
		{
			return S_OK;
		}
	};

	
	// Flag class as COM creatable
	CoCreatableClass(NotificationActivator);

	NotificationHelper::NotificationHelper() {

		HRESULT hr;
		hr = DesktopNotificationManagerCompat::RegisterAumidAndComServer(L"localChicken.AndroidConnect", __uuidof(NotificationActivator));
		hr = DesktopNotificationManagerCompat::RegisterActivator();

	}

	NotificationHelper::~NotificationHelper() {

	}

	HRESULT NotificationHelper::SendNotification(void* bytes, int arraySize) {

		ComPtr<IXmlDocument> doc;
		HRESULT hr;

		hr = DesktopNotificationManagerCompat::CreateXmlDocumentFromString(L"<toast><visual><binding template='ToastGeneric'><text></text><text></text><text></text></binding></visual></toast>", &doc);

		if (SUCCEEDED(hr)) {
			Notification notification = Notification();
			notification.ParseFromArray(bytes, arraySize);

			hr = XmlGenerator::Decode(notification, doc.Get());

			if (SUCCEEDED(hr)) {
				ComPtr<IToastNotifier> notifier;
				hr = DesktopNotificationManagerCompat::CreateToastNotifier(&notifier);

				if (SUCCEEDED(hr)) {
					ComPtr<IToastNotification> notification;
					hr = DesktopNotificationManagerCompat::CreateToastNotification(doc.Get(), &notification);

					if (SUCCEEDED(hr)) {

						hr = notifier->Show(notification.Get());

					}
				}
			}
		}


		return S_OK;

	}
}