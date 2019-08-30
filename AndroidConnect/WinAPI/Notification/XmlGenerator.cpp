#include "XmlGenerator.hpp"
#include <SDKDDKVer.h>
#include <string>
#include <windows.ui.notifications.h>
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include <NotificationActivationCallback.h>
#include <windows.ui.notifications.h>

#include <winrt/Windows.Foundation.h>
#include "shlwapi.h"

#define URI_MAX_PATH 1000
#define RETURN_IF_FAILED(hr) do { HRESULT _hrTemp = hr; if (FAILED(_hrTemp)) { return _hrTemp; } } while (false)


namespace AndroidConnect {
	std::vector<std::wstring> XmlGenerator::split(std::wstring in, const wchar_t* delim) {
		std::vector<std::wstring> returnVector;
		for (int i = in.find(delim);; i = in.find(delim)) {
			returnVector.push_back(in.substr(0, i));
			if (i == in.npos) break;
			in = in.substr(i + 1, in.length() - 1);
		}
		return returnVector;
	}

	std::wstring XmlGenerator::extract_arg_value(std::wstring arg) {
		arg = arg.substr(arg.find(L"=") + 1, arg.size() - 1);
		return arg;
	}


	winrt::hstring XmlGenerator::Decode(Notification& notificationInfo, int clientId) {

		tinyxml2::XMLDocument xmlDoc;
		tinyxml2::XMLElement* toastElement = xmlDoc.NewElement("toast");
		toastElement->SetAttribute("launch", "app-defined-string");
		tinyxml2::XMLElement* visualNode = xmlDoc.NewElement("visual");
		tinyxml2::XMLElement* bindingNode = xmlDoc.NewElement("binding");
		bindingNode->SetAttribute("template", "ToastGeneric");
		tinyxml2::XMLElement* title = xmlDoc.NewElement("text"), * text = xmlDoc.NewElement("text"), * undertext = xmlDoc.NewElement("text");
		title->SetText(notificationInfo.title().c_str());
		text->SetText(notificationInfo.text().c_str());
		undertext->SetText(notificationInfo.undertext().c_str());




		bindingNode->InsertEndChild(title);
		bindingNode->InsertEndChild(text);
		bindingNode->InsertEndChild(undertext);
		visualNode->InsertEndChild(bindingNode);
		toastElement->InsertEndChild(visualNode);


		if (notificationInfo.nactions_size() > 0) {
			tinyxml2::XMLElement* actions = xmlDoc.NewElement("actions");
			bool isRepliable = false;

			for (short i = 0; i < notificationInfo.nactions_size(); ++i) {
				NotificationActions naction = notificationInfo.nactions(i);
				if (naction.isrepliable()) isRepliable = true;
				tinyxml2::XMLElement* action = xmlDoc.NewElement("action");
				action->SetAttribute("activationType", naction.atype().c_str());
				action->SetAttribute("content", naction.content().c_str());
				std::string args(naction.args().c_str());
				args.append(";clientId=");
				args.append(std::to_string(clientId));
				action->SetAttribute("arguments", args.c_str());
				actions->InsertEndChild(action);
			}

			if (isRepliable) {
				tinyxml2::XMLElement* inputBox = xmlDoc.NewElement("input");
				inputBox->SetAttribute("id", "inputBox");
				inputBox->SetAttribute("type", "text");
				actions->InsertFirstChild(inputBox);
			}
			toastElement->InsertEndChild(actions);
		}


		xmlDoc.InsertEndChild(toastElement);


		tinyxml2::XMLPrinter printer;
		xmlDoc.Accept(&printer);
		const char* string = printer.CStr();

		printf("%s\n", string);

		return winrt::to_hstring(string);

	}

	std::string XmlGenerator::FixStr(const char* str) {
		std::string origStr(str);
		origStr.erase(std::remove(origStr.begin(), origStr.end(), '\n'), origStr.end());

		return origStr;
	}





}