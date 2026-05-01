#include <chrono>
#include <cstdint>
#include <string>
#define DISCORDPP_IMPLEMENTATION
#include "crow_all.h"
#include "./common.cpp"
#include "./protocolreg.cpp"
#include <discordpp.h>
#include <nlohmann/json.hpp>
#include "./stringcutter.cpp"

using namespace std;
using namespace discordpp;
using json = nlohmann::json;

enum WSCommand {
	SetApplicationID, Clear, SetType, SetDetails, SetState, SetName, SetTimestamps, SetAssets
};

auto client = std::make_shared<discordpp::Client>();
Activity activity;

condition_variable asdcv;
mutex asdmtx;
bool cancelled = false;

void autoshutdownThread() {
	unique_lock<mutex> lock(asdmtx);

	if (!asdcv.wait_for(lock, std::chrono::seconds(10), [] { return cancelled; })) {
		std::cout << "Closing due to inactivity...\n";
		std::exit(0);
	} else {
		// ...
	}
}

string makeCompatiblePresenceString(const string str) {
	return truncateUTF16Bytes(str, 128);
}

int main(int argsCount, char* args[]) {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	
	registerProtocol(argsCount < 2);
	if (argsCount < 2) {
		cout << "URI SCHEMA: webrichpresence://<IDENTIFIER>/<PORT>";
		exit(1);
	};

	const auto uriArgs = splitString(args[1], "/");
	const auto wrpIdentifier = uriArgs[2];
	const auto wrpPort = stoi(uriArgs[3]);

	cout << "Identifier: " << wrpIdentifier << "\n";
	cout << "Port: " << wrpPort << "\n";
	
	signal(SIGINT, [](int signal) {
		lock_guard<mutex> lock(asdmtx);
		cancelled = true;
		asdcv.notify_one();
		exit(signal);
	});

	std::thread t(autoshutdownThread);
	vector<crow::websocket::connection*> connections {};
	crow::SimpleApp app;
	app.loglevel(crow::LogLevel::Warning);

	CROW_WEBSOCKET_ROUTE(app, "/<string>")
	.onaccept([&](const crow::request& req, void**) {
		const auto identifier = req.url.substr(1);
		if (identifier != wrpIdentifier) {
			cout << "Declining connection: " << identifier << "!=" << wrpIdentifier << "\n";
			return false;
		}
		cout << "Accepted connection" << "\n";
		return true;
	})
	.onopen([&](crow::websocket::connection& conn) {
		connections.push_back(&conn);
		lock_guard<mutex> lock(asdmtx);
		cancelled = true;
		asdcv.notify_one();
	})
	.onmessage([](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
		auto jdata = json::parse(data);

		for (auto item : jdata) {
			WSCommand command { item.at("command") };

			switch (command) {
				case WSCommand::SetApplicationID: {
					auto id = stoull((string) item.at("id"));
					cout << "Application ID = " << id << "\n";
					client->SetApplicationId(id);
					break;
				}
				case WSCommand::Clear: {
					cout << "(Cleared)" << "\n";
					client->ClearRichPresence();
					break;
				}
				case WSCommand::SetType: {
					activity.SetType(item.at("type"));
					cout << "Type = " << item.at("type") << "\n";
					break;
				}
				case WSCommand::SetName: {
					const auto str = makeCompatiblePresenceString(item["name"]);
					activity.SetName(str);

					cout << "Name = " << str << "\n";
					break;
				}
				case WSCommand::SetDetails: {
					if (item.at("details").is_null()) activity.SetDetails(nullopt);
					else {
						const auto str = makeCompatiblePresenceString(item["details"]);
						activity.SetDetails(str);

						cout << "Details = " << str << "\n";
					}
					break;
				}
				case WSCommand::SetState: {
					if (item.at("state").is_null()) activity.SetState(nullopt);
					else {
						const auto str = makeCompatiblePresenceString(item["state"]);
						activity.SetState(str);

						cout << "State = " << str << "\n";
					}
					break;
				}
				case WSCommand::SetTimestamps: {
					auto timestamps = make_optional<ActivityTimestamps>();

					if (item["start"].is_null() == false) {
						uint64_t start = item.at("start");
						timestamps->SetStart(start);
						cout << "Start Timestamp = " << start << "\n";
					}

					if (item["end"].is_null() == false) {
						uint64_t end = item.at("end");
						timestamps->SetEnd(end);
						cout << "End Timestamp = " << end << "\n";
					}

					activity.SetTimestamps(timestamps);
					break;
				}
				case WSCommand::SetAssets: {
					auto assets = make_optional<ActivityAssets>();

					if (item.contains("large_image_key")) {
						string str; 
						assets->SetLargeImage(item["large_image_key"].is_null() || ((string) item["large_image_key"]).length() == 0 ? nullopt : make_optional(
							str = item["large_image_key"]
						));

						cout << "LargeImage = " << str << "\n";
					}
					if (item.contains("large_image_url")) {
						string str; 
						assets->SetLargeUrl(item["large_image_url"].is_null() ? nullopt : make_optional(
							str = item["large_image_url"]
						));

						cout << "LargeUrl = " << str << "\n";
					}
					if (item.contains("large_image_text")) {
						string str; 
						assets->SetLargeText(item["large_image_text"].is_null() ? nullopt : make_optional(
							str = makeCompatiblePresenceString(item["large_image_text"])
						));
						
						cout << "LargeText = " << str << "\n";
					}

					
					if (item.contains("small_image_key")) {
						string str; 
						assets->SetSmallImage(item["small_image_key"].is_null() || ((string) item["small_image_key"]).length() == 0  ? nullopt : make_optional(
							str = item["small_image_key"]
						));

						cout << "SmallImage = " << str << "\n";
					}
					if (item.contains("small_image_url")) {
						string str; 
						assets->SetSmallUrl(item["small_image_url"].is_null() ? nullopt : make_optional(
							str = item["large_image_url"]
						));

						cout << "SmallUrl = " << str << "\n";
					}
					if (item.contains("small_image_text")) {
						string str; 
						assets->SetSmallText(item["small_image_text"].is_null() ? nullopt : make_optional(
							str = makeCompatiblePresenceString(item["small_image_text"])
						));
						
						cout << "SmallText = " << str << "\n";
					}

					activity.SetAssets(assets);
					break;
				}
			}
		}

		client->UpdateRichPresence(activity, NULL);
	}).onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t) {
		for (int i = 0; i < connections.size(); i++) {
			if (connections[i] == &conn) {
				connections.erase(connections.begin() + i);
				break;
			}	
		}
		if (connections.size() == 0) {
			cout << "All clients are disconnected, closing..." << "\n";
			exit(1);
		};
	});

	app.bindaddr("127.0.0.1").port(wrpPort).run();
}