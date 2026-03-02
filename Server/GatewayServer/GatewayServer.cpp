
#include <iostream>
#include <vector>
#include <utility>

#include "Common.h"

#pragma comment(lib, "Common.lib")



int main()
{
	WSAData WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	ListenSockAddr.sin_port = htons(18488);

	int Result = bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 5);

	fd_set MasterReadSet;
	FD_ZERO(&MasterReadSet);
	FD_SET(ListenSocket, &MasterReadSet);

	printf("GatewayServer started...\n");

	std::vector<std::pair<int, std::string>> WaitingUsers;

	while (true)
	{
		TIMEVAL TimeOut;
		TimeOut.tv_sec = 0;
		TimeOut.tv_usec = 100;

		fd_set WorkingReadSet = MasterReadSet;
		int SelectCount = select(0, &WorkingReadSet, nullptr, nullptr, &TimeOut);
		if (SelectCount == SOCKET_ERROR)
		{
			std::cerr << "select() error: " << WSAGetLastError() << std::endl;
			break;
		}
		if (SelectCount == 0)
		{
			// 타임아웃
			continue;
		}

		// 지금상태에선 어차피 WebServer랑만 통신이 열려있는거니까, 연결중인 Socket이 하나밖에 없잖아.
		for (int i = 0; i < (int)WorkingReadSet.fd_count; ++i)
		{
			SOCKET SelectedSocket = WorkingReadSet.fd_array[i];

			if (FD_ISSET(SelectedSocket, &WorkingReadSet))
			{
				if (SelectedSocket == ListenSocket)
				{
					// 연결요청
					SOCKADDR_IN ClientSockAddr;
					memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
					int ClientSockAddrLength = sizeof(ClientSockAddr);
					SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
					if (ClientSocket == INVALID_SOCKET)
					{
						int Error = WSAGetLastError();
						continue;
					}
					 
					FD_SET(ClientSocket, &MasterReadSet);
				}
				else
				{
					// 패킷처리
					char Buffer[4096] = { 0 };
					int RecvBytes = RecvPacket(SelectedSocket, Buffer, sizeof(Buffer));

					if (RecvBytes <= 0)
					{
						closesocket(SelectedSocket);
						FD_CLR(SelectedSocket, &MasterReadSet);
						continue;
					}

					auto UserEvent = UserEvents::GetEventData(Buffer);
					switch (UserEvent->data_type())
					{
					case UserEvents::EventType_ServerLogin:
					{
						auto ServerLoginData = UserEvent->data_as_ServerLogin();
						std::cout << "User Login: " << ServerLoginData->idx() << " (" << ServerLoginData->name()->c_str() << ")" << std::endl;

						WaitingUsers.push_back({ ServerLoginData->idx(), ServerLoginData->name()->str() });

						// 들어온 순서대로 2명.
						if (WaitingUsers.size() >= 2)
						{
							std::cout << "WaitingUsers Num Over 2, Match! MatchInfo Send to WebServer..." << std::endl;
							flatbuffers::FlatBufferBuilder MatchBuilder;

							std::vector<flatbuffers::Offset<MatchingEvents::UserInfo>> UserOffsets;
							for (const auto& user : WaitingUsers)
							{
								auto nameOffset = MatchBuilder.CreateString(user.second);
								UserOffsets.push_back(MatchingEvents::CreateUserInfo(MatchBuilder, user.first, nameOffset));
							}

							auto UsersVector = MatchBuilder.CreateVector(UserOffsets);
							auto MatchDataOffset = MatchingEvents::CreateMatchData(MatchBuilder, UsersVector);
							MatchBuilder.Finish(MatchDataOffset);

							SendPacket(SelectedSocket, MatchBuilder);
							WaitingUsers.clear();
						}
					break;
					}
					}
				}
			}
		}
	}

	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}