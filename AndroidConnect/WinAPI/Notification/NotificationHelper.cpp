#include "NotificationHelper.hpp"
#include "XmlGenerator.hpp"
#include "DesktopNotificationManagerCompat.h"
#include "../../TCPServer/TCPServer.hpp"


namespace AndroidConnect {
	//TODO: Migrate to winrt

	// The UUID CLSID must be unique to your app. Create a new GUID if copying this code.
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
			printf("Arg: %ls\n", std::wstring(invokedArgs).c_str());
			std::wstring args(invokedArgs);
			std::vector<std::wstring> splitArgs(XmlGenerator::split(args, L";"));
			for (std::wstring str : splitArgs) {
				printf("Arg kv: %ls\n", str.c_str());
			}
			std::wstring clid = XmlGenerator::extract_arg_value(splitArgs.at(splitArgs.size() - 1));
			std::wstring action = XmlGenerator::extract_arg_value(splitArgs.at(0));
			int clientId = _wtoi(clid.c_str());
			AndroidConnect::Notification notification;
			notification.set_text(winrt::to_string(action));
			std::wstring data_val(data[0].Value);
			if (!data_val._Equal(L"")) {
				AndroidConnect::NotificationReply* reply = new AndroidConnect::NotificationReply();

				reply->set_replyvalue(winrt::to_string(data[0].Value));
				notification.set_allocated_reply(reply);

			}
			int size = notification.ByteSize();
			unsigned char* dataArr = (unsigned char*)malloc(size);
			notification.SerializeToArray(dataArr, size);
			AndroidConnect::TCPServer::GetInstance()->GetClient(clientId)->Callback(dataArr, 0, size);
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

	void NotificationHelper::SendNotification(unsigned char *data, int size, unsigned int clientId) {
		HRESULT hr = S_OK;
		Notification notification;
		if (notification.ParseFromArray(data, size)) {

		
			ComPtr<IXmlDocument> govnoDoc;
			hr = DesktopNotificationManagerCompat::CreateXmlDocumentFromString(XmlGenerator::Decode(notification, clientId).c_str(), &govnoDoc);
			ComPtr<IToastNotifier> notifier;
			hr = DesktopNotificationManagerCompat::CreateToastNotifier(&notifier);
			if (SUCCEEDED(hr)) {
				ComPtr<IToastNotification> notification;
				hr = DesktopNotificationManagerCompat::CreateToastNotification(govnoDoc.Get(), &notification);
				if (SUCCEEDED(hr)) {
					hr = notifier->Show(notification.Get());
				}
			}
		}
	}

}