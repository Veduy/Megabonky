#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX

#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <mutex>
#include <memory>

#include "Common.h"
#include "json.hpp"
#include "httplib.h"

#include "mysql-connector/include/jdbc/mysql_connection.h"
#include "mysql-connector/include/jdbc/cppconn/driver.h"
#include "mysql-connector/include/jdbc/cppconn/exception.h"
#include "mysql-connector/include/jdbc/cppconn/resultset.h"
#include "mysql-connector/include/jdbc/cppconn/statement.h"
#include "mysql-connector/include/jdbc/cppconn/prepared_statement.h"

#pragma comment(lib, "mysqlcppconn.lib")
#pragma comment(lib, "Common.lib")

using json = nlohmann::json;

int main()
{
    WSAData WsaData;
    WSAStartup(MAKEWORD(2, 2), &WsaData);

    httplib::Server svr;
    
    // MySQL connection settings
    const std::string db_url = "tcp://127.0.0.1:3306";
    const std::string db_user = "root";
    const std::string db_pass = "NewPass1234!";
    const std::string db_schema = "Megabonky";

    sql::Driver* Driver = get_driver_instance();
    sql::Connection* Connection = nullptr;
    std::mutex DbMutex;

    try
    {
        Connection = Driver->connect(db_url, db_user, db_pass);
        Connection->setSchema(db_schema);
    }
    catch (sql::SQLException& e)
    {
        std::cout << "[DB CONNECT FAIL]\n";
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "ErrorCode: " << e.getErrorCode() << std::endl;
        std::cout << "SQLState: " << e.getSQLState() << std::endl;

        Connection = nullptr;
    }

    svr.Post("/api/login", [&](const httplib::Request& req, httplib::Response& res)
    {
        std::cout << "[REQUEST START] POST /api/login from " << req.remote_addr << std::endl;
        
        json RequestBody;
        try
        {
            RequestBody = json::parse(req.body);
        }
        catch (json::parse_error& e)
        {
            std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            res.status = 400;
            res.set_content(CreateJsonResponse("", false), "application/json");
            return;
        }

        std::string user_id = RequestBody.value("user_id", "");
        std::string passwd = RequestBody.value("passwd", "");

        if (user_id.empty() || passwd.empty())
        {
            res.set_content(CreateJsonResponse("", false), "application/json");
            return;
        }

        std::lock_guard<std::mutex> lock(DbMutex);

        if (!Connection)
        {
            res.status = 503;
            res.set_content("{\"error\": \"Database not available\"}", "application/json");
            return;
        }

        try
        {
            // TODO: SHA2 without per-user salt is vulnerable to rainbow table attacks. Consider bcrypt or adding a salt column.
            std::unique_ptr<sql::PreparedStatement> PreparedStatement(Connection->prepareStatement(
                "SELECT `id`, `user_id` FROM user WHERE `user_id` = ? AND passwd = SHA2(?, 256);"
            ));

            PreparedStatement->setString(1, user_id);
            PreparedStatement->setString(2, passwd);

            std::unique_ptr<sql::ResultSet> res_db(PreparedStatement->executeQuery());

            if (res_db->next())
            {
                std::string logged_in_user_id = res_db->getString("user_id");

                // 조회 결과를 바로 언리얼로 HTTP 응답
                // 언리얼에서 user_id(String), result(Boolean)으로 받음
                res.set_content(CreateJsonResponse(logged_in_user_id, true), "application/json");
                std::cout << "Login Success: " << logged_in_user_id << std::endl;
            }
            else
            {
                res.set_content(CreateJsonResponse("", false), "application/json");
                std::cout << "Login Failed: user_id=" << user_id << std::endl;
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "MySQL Error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content("{\"error\": \"Database error\"}", "application/json");
        }
        catch (std::exception& e)
        {
            std::cerr << "General Error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content("{\"error\": \"Internal server error\"}", "application/json");
        }
        
        if (res.status == -1) res.status = 200;
        std::cout << "[REQUEST END] POST /api/login handled with status: " << res.status << std::endl;
    });

    svr.Get("/api/check/ranking", [&](const httplib::Request& req, httplib::Response& res)
    {
        std::cout << "[REQUEST START] GET /api/check/ranking from " << req.remote_addr << std::endl;
        
        std::lock_guard<std::mutex> lock(DbMutex);

        if (!Connection)
        {
            res.status = 503;
            res.set_content("{\"error\": \"Database not available\"}", "application/json");
            return;
        }

        try
        {
            std::unique_ptr<sql::PreparedStatement> PreparedStatement(Connection->prepareStatement(
                "SELECT RANK() OVER(ORDER BY score DESC) AS ranking, user_name,score FROM ranking ORDER BY ranking LIMIT 10"));

            std::unique_ptr<sql::ResultSet> res_db(PreparedStatement->executeQuery());

            json Ranking;

            while (res_db->next())
            {
                json Row =
                {
                    {"rank", std::to_string(res_db->getInt("ranking"))},
                    {"name", res_db->getString("user_name")},
                    {"score", std::to_string(res_db->getInt("score"))}
                };
                Ranking.push_back(Row);
            }

            res.set_content(Ranking.dump(), "application/json");
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "MySQL Error (Ranking): " << e.what() << std::endl;
            res.status = 500;
            res.set_content("{\"error\": \"Database error during ranking fetch\"}", "application/json");
        }
        
        if (res.status == -1) res.status = 200;
        std::cout << "[REQUEST END] GET /api/check/ranking handled with status: " << res.status << std::endl;
    });
  
    svr.Post("/api/save_kills", [&](const httplib::Request& req, httplib::Response& res)
    {
        std::cout << "[REQUEST START] POST /api/save_kills from " << req.remote_addr << std::endl;
        
        json RequestBody;
        try
        {
            RequestBody = json::parse(req.body);
        }
        catch (json::parse_error& e)
        {
            std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            res.status = 400;
            res.set_content(CreateJsonResponse("", false), "application/json");
            return;
        }

        std::string user_id = RequestBody.value("user_id", "");
        int total_kills = RequestBody.value("total_kills", 0);

        if (user_id.empty())
        {
            res.set_content(CreateJsonResponse("", false), "application/json");
            return;
        }

        std::lock_guard<std::mutex> lock(DbMutex);

        if (!Connection)
        {
            res.status = 503;
            res.set_content("{\"error\": \"Database not available\"}", "application/json");
            return;
        }

        try
        {
            std::unique_ptr<sql::PreparedStatement> PreparedStatement(Connection->prepareStatement(
                "INSERT INTO user_stats (user_id, total_kills) VALUES (?, ?) ON DUPLICATE KEY UPDATE total_kills = ?"
            ));

            PreparedStatement->setString(1, user_id);
            PreparedStatement->setInt(2, total_kills);
            PreparedStatement->setInt(3, total_kills);

            PreparedStatement->executeUpdate();

            res.set_content(CreateJsonResponse("Success", true), "application/json");
            std::cout << "Save Kills Success: user_id=" << user_id << ", total_kills=" << total_kills << std::endl;
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "MySQL Error (Save Kills): " << e.what() << std::endl;
            res.status = 500;
            res.set_content("{\"error\": \"Database error during save_kills\"}", "application/json");
        }
        
        if (res.status == -1) res.status = 200;
        std::cout << "[REQUEST END] POST /api/save_kills handled with status: " << res.status << std::endl;
    });

    std::cout << "WebServer starting at http://127.0.0.1:8080/api/login" << std::endl;
    
    svr.listen("0.0.0.0", 8080); //블로킹 상태 진입함.

    delete Connection;
    WSACleanup();

    return 0;
}