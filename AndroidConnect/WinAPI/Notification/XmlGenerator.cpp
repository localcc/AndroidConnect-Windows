#include "XmlGenerator.hpp"
#include "DesktopNotificationManagerCompat.h"
#include <SDKDDKVer.h>
#include <string>
#include <windows.ui.notifications.h>
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include <NotificationActivationCallback.h>
#include <windows.ui.notifications.h>
#include "shlwapi.h"

#define URI_MAX_PATH 1000
#define RETURN_IF_FAILED(hr) do { HRESULT _hrTemp = hr; if (FAILED(_hrTemp)) { return _hrTemp; } } while (false)

using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::UI::Notifications;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

namespace AndroidConnect {
	HRESULT XmlGenerator::SetNodeValueString(HSTRING inputString, ABI::Windows::Data::Xml::Dom::IXmlNode* node, ABI::Windows::Data::Xml::Dom::IXmlDocument* xml) {
		Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlText> inputText;
		RETURN_IF_FAILED(xml->CreateTextNode(inputString, &inputText));

		Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNode> inputTextNode;
		RETURN_IF_FAILED(inputText.As(&inputTextNode));

		Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNode> appendedChild;
		return node->AppendChild(inputTextNode.Get(), &appendedChild);
	}

	HRESULT XmlGenerator::Decode(Notification& notificationInfo, ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml) {
		Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNodeList> nodeList;
		
		toastXml->GetElementsByTagName(HStringReference(L"text").Get(), &nodeList);

		UINT32 nodeListLength;
		RETURN_IF_FAILED(nodeList->get_Length(&nodeListLength));

		// If a template was chosen with fewer text elements, also change the amount of strings
		// passed to this method.

		PCWSTR textValues[]{
			std::wstring(notificationInfo.title().begin(), notificationInfo.title().end()).c_str(),
			std::wstring(notificationInfo.text().begin(), notificationInfo.text().end()).c_str(),
			std::wstring(notificationInfo.undertext().begin(), notificationInfo.undertext().end()).c_str()
		};


		for (UINT32 i = 0; i < 3; i++)
		{
			Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNode> textNode;
			RETURN_IF_FAILED(nodeList->Item(i, &textNode));

			RETURN_IF_FAILED(SetNodeValueString(HStringReference(textValues[i]).Get(), textNode.Get(), toastXml));
		}

		return S_OK;
	}

	//TODO: use it
	HRESULT XmlGenerator::SetImageSrc(PCWSTR imageStr, ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml) {
		wchar_t imageSrcUri[URI_MAX_PATH];
		DWORD size = ARRAYSIZE(imageSrcUri);
		RETURN_IF_FAILED(::UrlCreateFromPath(imageStr, imageSrcUri, &size, 0));

		ComPtr<IXmlNodeList> nodeList;
		RETURN_IF_FAILED(toastXml->GetElementsByTagName(HStringReference(L"image").Get(), &nodeList));

		ComPtr<IXmlNode> imageNode;
		RETURN_IF_FAILED(nodeList->Item(0, &imageNode));

		ComPtr<IXmlNamedNodeMap> attributes;
		RETURN_IF_FAILED(imageNode->get_Attributes(&attributes));

		ComPtr<IXmlNode> srcAttribute;
		RETURN_IF_FAILED(attributes->GetNamedItem(HStringReference(L"src").Get(), &srcAttribute));

		return SetNodeValueString(HStringReference(imageSrcUri).Get(), srcAttribute.Get(), toastXml);
	}





}