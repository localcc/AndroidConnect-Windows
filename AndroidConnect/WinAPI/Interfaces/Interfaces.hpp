//Here you will have to add every interface for any API you want to use
#pragma once

#include "../Notification/NotificationHelper.hpp"

namespace AndroidConnect {
	struct Interface {
		void Init() {
			// Init function of WinAPI
		}
		void Invoke(void* bytes) {
			// Invoke function
		}
	};

	struct NotificationInterface: Interface {
		NotificationHelper helper;
		void Init() {
			helper = NotificationHelper::NotificationHelper();
		}
		void Invoke(void* bytes) {
			helper.SendNotification(bytes);
		}
	};
}