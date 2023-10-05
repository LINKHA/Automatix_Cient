#include "nakama-cpp/Nakama.h"
#include "nakama-cpp/realtime/NRtDefaultClientListener.h"
#include "nakama-cpp/data/NAccount.h"
#include "nakama-cpp/NClientInterface.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <optional>
#include <vector>


#if __ANDROID__
#include <android_native_app_glue.h>
#include <jni.h>
#endif

using namespace Nakama;
using namespace std;
Nakama::NClientPtr client;
Nakama::NSessionPtr session;
Nakama::NRtClientPtr rtClient;
Nakama::NRtDefaultClientListener rtListener;

void writeObjects() {
	auto successCallback = [](const NStorageObjectAcks& acks)
	{
		std::cout << "Successfully stored objects " << acks.size() << std::endl;
	};

	std::vector<NStorageObjectWrite> objects;
	NStorageObjectWrite savesObject, statsObject;
	savesObject.collection = "saves";
	savesObject.key = "savegame";
	savesObject.value = R"({ "progress": 50 })";
	objects.push_back(savesObject);

	statsObject.collection = "stats";
	statsObject.key = "skills";
	statsObject.value = R"({ "skill": 24 })";
	objects.push_back(statsObject);
	client->writeStorageObjects(session, objects, successCallback);
}

void readObjects() {
	auto successCallback = [](const NStorageObjects& objects)
	{
		for (auto& object : objects)
		{
			std::cout << "Object key: " << object.key << ", value: " << object.value << std::endl;
		}
	};

	std::vector<NReadStorageObjectId> objectIds;
	NReadStorageObjectId objectId;
	objectId.collection = "saves";
	objectId.key = "savegame";
	objectId.userId = session->getUserId();
	objectIds.push_back(objectId);
	client->readStorageObjects(session, objectIds, successCallback);
}

void getAccount() {
	auto successCallback = [](const Nakama::NAccount& account)
	{
		std::cout << "User's wallet: " << account.wallet.c_str() << std::endl;

		std::cout << "User's email: " << account.email << std::endl;
	};

	client->getAccount(session, successCallback);
}

//Friends
// 
//添加好友
void addFriends() {
	vector<string> ids = { "93b917a8-f7e0-41c3-aaee-332360c29b9d" };
	vector<string> usernames = { "a421698843" };
	client->addFriends(session, ids, usernames);
}

//列出好友
void listFriends() {
	auto successCallback = [](NFriendListPtr friends)
	{
		std::cout << "Successfully retrieved friend list: " << friends->friends.size() << std::endl;
	};

	client->listFriends(session, {}, {}, {}, successCallback);
}

//移除好友
void deleteFriends() {
	vector<string> ids = { "93b917a8-f7e0-41c3-aaee-332360c29b9d" };
	vector<string> usernames = { "a421698843" };
	client->deleteFriends(session, ids, usernames);
}

//屏蔽好友
void blockFriends() {
	vector<string> ids = { "93b917a8-f7e0-41c3-aaee-332360c29b9d" };
	vector<string> usernames = { "a421698843" };
	client->blockFriends(session, ids, usernames);
}

//群组

//创建群组
void createGroup() {
	auto successCallback = [](const NGroup& group)
	{
		std::cout << "New group ID: " << group.id << std::endl;
	};

	string group_name = "pizza-lovers";
	string description = "pizza lovers, pineapple haters";
	client->createGroup(session, group_name, description, "", "en_US", true, {}, successCallback);
}

//实时聊天

void createRoom() {
	auto successCallback = [](NChannelPtr channel)
	{
		std::cout << "Now connected to channel id: " << channel->id << std::endl;
	};

	string roomname = "MarvelMovieFans";
	rtClient->joinChat(roomname, NChannelType::ROOM, true, false, successCallback);
}

//中继多人游戏
//创建比赛
void createMatch() {
	rtClient->createMatch([](const NMatch& match)
		{
			std::cout << "Created Match with ID: " << match.matchId << std::endl;
		});
}

void init() {
	//getAccount();
	//writeObjects();
	//readObjects();
	//addFriends();

	//listFriends();
	//deleteFriends();

	//createGroup()

	//createRoom();

	createMatch();
}

int mainHelper() {
	Nakama::NLogger::initWithConsoleSink(Nakama::NLogLevel::Debug);
	Nakama::NClientParameters params;
	params.serverKey = "defaultkey";
	params.host = "127.0.0.1";
	params.port = Nakama::DEFAULT_PORT;
	auto _client = Nakama::createDefaultClient(params);
	Nakama::NRtClientPtr _rtClient = nullptr;
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

	auto loginSucceededCallback = [&done, &connectSucceededCallback, &rtErrorCallback, &_client, &_rtClient](Nakama::NSessionPtr _session) {
		NLOG_INFO("Login successful");
		NLOG_INFO(_session->getAuthToken()); // raw JWT token
		Nakama::NRtDefaultClientListener _listener;
		_listener.setConnectCallback(connectSucceededCallback);
		_listener.setErrorCallback(rtErrorCallback);
		_rtClient = _client->createRtClient();
		_rtClient->setListener(&_listener);
		NLOG_INFO("Connecting socket");
		_rtClient->connect(_session, true, Nakama::NRtClientProtocol::Json);

		client = _client;
		session = _session;
		rtClient = _rtClient;
		rtListener = _listener;
	};

	std::string deviceId = "fb14c1b4-f629-48ff-be5a-237c37a913cf";
	NLOG_INFO("Authenticating...");

	_client->authenticateDevice(
		deviceId,
		Nakama::opt::nullopt,
		Nakama::opt::nullopt,
		{},
		loginSucceededCallback,
		loginFailedCallback);

	while (!done) {
		_client->tick();

		if (_rtClient)
		{
			static bool initialized = false;
			if (!initialized) {
				initialized = true;
				init();
			}

			_rtClient->tick();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	NLOG_INFO("Press any key to continue");
	getchar();
	_client->disconnect();
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