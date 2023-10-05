#include "nakama-cpp/Nakama.h"
#include "nakama-cpp/realtime/NRtDefaultClientListener.h"
#include "nakama-cpp/data/NAccount.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <optional>

#if __ANDROID__
#include <android_native_app_glue.h>
#include <jni.h>
#endif


Nakama::NClientPtr _client;
Nakama::NSessionPtr _session;

void getAccount() {
	auto successCallback = [](const Nakama::NAccount& account)
	{
		std::cout << "User's wallet: " << account.wallet.c_str() << std::endl;

		std::cout << "User's email: " << account.email << std::endl;
	};

	_client->getAccount(_session, successCallback);
}

void init() {
	getAccount();
}

int mainHelper() {
	Nakama::NLogger::initWithConsoleSink(Nakama::NLogLevel::Debug);
	Nakama::NClientParameters params;
	params.serverKey = "defaultkey";
	params.host = "127.0.0.1";
	params.port = Nakama::DEFAULT_PORT;
	auto client = Nakama::createDefaultClient(params);
	Nakama::NRtClientPtr rtClient = nullptr;
	bool done = false;
	auto loginFailedCallback = [&done](const Nakama::NError& error) {
		NLOG_INFO("Failed to login");
		NLOG_INFO(error.message);
		done = true;
	};

	auto connectSucceededCallback = [&done]() {
		NLOG_INFO("Done connecting socket");
		done = true;
	};

	auto rtErrorCallback = [&done](const Nakama::NRtError& error) {
		NLOG_INFO("Error from socket:...");
		NLOG_INFO(error.message);
		done = true;
	};

	auto loginSucceededCallback = [&done, &connectSucceededCallback, &rtErrorCallback, &client, &rtClient](Nakama::NSessionPtr session) {
		NLOG_INFO("Login successful");
		NLOG_INFO(session->getAuthToken()); // raw JWT token
		Nakama::NRtDefaultClientListener listener;
		listener.setConnectCallback(connectSucceededCallback);
		listener.setErrorCallback(rtErrorCallback);
		rtClient = client->createRtClient();
		rtClient->setListener(&listener);
		NLOG_INFO("Connecting socket");
		rtClient->connect(session, true, Nakama::NRtClientProtocol::Json);

		_client = client;
		_session = session;
	};

	std::string deviceId = "fb14c1b4-f629-48ff-be5a-237c37a913cf";
	NLOG_INFO("Authenticating...");

	client->authenticateDevice(
		deviceId,
		Nakama::opt::nullopt,
		Nakama::opt::nullopt,
		{},
		loginSucceededCallback,
		loginFailedCallback);

	while (!done) {
		client->tick();

		if (rtClient)
		{
			static bool initialized = false;
			if (!initialized) {
				init();
			}

			rtClient->tick();
		}

		/*std::this_thread::sleep_for(std::chrono::milliseconds(50));*/
	}

	NLOG_INFO("Press any key to continue");
	getchar();
	client->disconnect();
	return 0;
}

#if __ANDROID__

extern "C"
{
    void android_main(struct android_app* app) {
        mainHelper();
    }
}

#else
int main() {
    mainHelper();
}

#endif