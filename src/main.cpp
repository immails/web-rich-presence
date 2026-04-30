#include <cstdint>
#define DISCORDPP_IMPLEMENTATION
#include "crow_all.h"
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

	if (!asdcv.wait_for(lock, std::chrono::minutes(1), [] { return cancelled; })) {
		std::cout << "Closing due inactivity...\n";
		std::exit(0);
	} else {
		// ...
	}
}

int main(int argsCount, char* args[]) {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	
	cout << "Args count: " << argsCount << "\n";

	for (int i = 0; i < argsCount; i++) {
		cout << i << ": " << args[i] << "\n";
	}

	registerProtocol(argsCount < 2);
	if (argsCount < 2) exit(1);

	signal(SIGINT, [](int signal) {
		lock_guard<mutex> lock(asdmtx);
		cancelled = true;
		asdcv.notify_one();
		exit(signal);
	});

	std::thread t(autoshutdownThread);
	
	crow::SimpleApp app;

	CROW_WEBSOCKET_ROUTE(app, "/")
	.onopen([&](crow::websocket::connection& conn) {
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
					cout << "Set Application ID to " << id << "\n";
					client->SetApplicationId(id);
					break;
				}
				case WSCommand::Clear: {
					cout << "Cleared" << "\n";
					client->ClearRichPresence();
					break;
				}
				case WSCommand::SetType: {
					cout << "Set Type to " << item.at("type") << "\n";
					activity.SetType(item.at("type"));
					break;
				}
				case WSCommand::SetName: {
					const auto string = truncateUTF16Bytes(item.at("name"), 128);
					cout << "Set Name to " << string << "\n";
					activity.SetName(string);
					break;
				}
				case WSCommand::SetDetails: {
					if (item.at("details").is_null()) activity.SetDetails(nullopt);
					else {
						const auto string = truncateUTF16Bytes(item.at("details"), 128);
						cout << "Set Details to " << string << "\n";
						activity.SetDetails(string);
					}
					break;
				}
				case WSCommand::SetState: {
					if (item.at("state").is_null()) activity.SetState(nullopt);
					else {
						const auto string = truncateUTF16Bytes(item.at("state"), 128);
						cout << "Set Details to " << string << "\n";
						activity.SetState(string);
					}
					break;
				}
				case WSCommand::SetTimestamps: {
					auto timestamps = make_optional<ActivityTimestamps>();

					if (item["start"].is_null() == false) {
						uint64_t start = item.at("start");
						timestamps->SetStart(start);
						cout << "Set Start Timestamp to " << start << "\n";
					}

					if (item["end"].is_null() == false) {
						uint64_t end = item.at("end");
						timestamps->SetEnd(end);
						cout << "Set End Timestamp to " << end << "\n";
					}

					activity.SetTimestamps(timestamps);
					break;
				}
				case WSCommand::SetAssets: {
					auto assets = make_optional<ActivityAssets>();

					if (item.contains("large_image_key")) assets->SetLargeImage(item["large_image_key"].is_null() ? nullopt : make_optional(item["large_image_key"]));
					if (item.contains("large_image_url")) assets->SetLargeUrl(item["large_image_url"].is_null() ? nullopt : make_optional(item["large_image_url"]));
					if (item.contains("large_image_text")) assets->SetLargeText(item["large_image_text"].is_null() ? nullopt : make_optional(item["large_image_text"]));

					
					if (item.contains("small_image_key")) assets->SetSmallImage(item["small_image_key"].is_null() ? nullopt : make_optional(item["small_image_key"]));
					if (item.contains("small_image_url")) assets->SetSmallUrl(item["small_image_url"].is_null() ? nullopt : make_optional(item["large_image_url"]));
					if (item.contains("small_image_text")) assets->SetSmallText(item["small_image_text"].is_null() ? nullopt : make_optional(item["small_image_text"]));

					cout << "Updated assets" << "\n";

					activity.SetAssets(assets);
					break;
				}
			}
		}

		client->UpdateRichPresence(activity, NULL);
	}).onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t) {
		exit(1);
	});

	app.port(46895).run();
}