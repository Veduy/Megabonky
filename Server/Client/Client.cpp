#include <iostream>

#include "Common.h"
#include "httplib.h"
#include "json.hpp"

#pragma comment(lib, "Common.lib")

using json = nlohmann::json;

int main()
{
	std::cout << "client start.." << std::endl;

	int Case;
	Case = std::cin.get();

	httplib::Client cli("http://127.0.0.1:8080");
	httplib::Result res;

	if (Case == '1')
	{
		std::cin.ignore();
		std::string user_id, passwd;
		std::cout << "user_id: ";
		std::getline(std::cin, user_id);
		std::cout << "passwd: ";
		std::getline(std::cin, passwd);

		json loginBody;
		loginBody["user_id"] = user_id;
		loginBody["passwd"] = passwd;

		if (res = cli.Post("/api/login", loginBody.dump(), "application/json"))
		{
			if (res->status == 200)
			{
				// JSON 파싱
				try {
					json j = json::parse(res->body);
					std::cout << "Response: " << j.dump(4) << std::endl;
					if (j.contains("name")) 
					{
						std::string name = j["name"];
						std::cout << "Login User: " << name << std::endl;
					}
				}
				catch (json::parse_error& e) 
				{
					std::cerr << "JSON 파싱 실패: " << e.what() << std::endl;
				}
			}
			else
			{
				std::cout << "Error: " << res->status << std::endl;
			}

		}
	}
	else if (Case == '2')
	{
		if (res = cli.Get("/api/check/ranking"))
		{
			if (res->status == 200)
			{
				std::cout << "Response: " << res->body << std::endl;

				// JSON parsing
				try {
					json Ranking = json::parse(res->body);
					for (const auto& Row : Ranking)
					{
						std::string rank = Row["rank"];
						std::string name = Row["name"];
						std::string score = Row["score"];
						std::cout << "[" << rank << "위] " << name << " - 점수: " << score << std::endl;
						// 언리얼에서 UI Text 설정.
					}
				}
				catch (json::parse_error& e) 
				{
					std::cerr << "JSON 파싱 실패: " << e.what() << std::endl;
				}

			}
			else
			{
				std::cout << "Error: " << res->status << std::endl;
				std::cout << "Response: " << res->body << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Invalid input. Enter '1' for login or '2' for ranking." << std::endl;
	}

	// 종료 방지용 대기 코드
    std::cout << "\nPress Enter to exit...";
    std::cin.ignore(); // 버퍼 비우기
    std::cin.get();    // 진짜 대기

	return 0;
}