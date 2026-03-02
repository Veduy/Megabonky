#pragma once

#define NOMINMAX

#include <WinSock2.h>
#include <Windows.h>
#include <string>

#include "UserEvents_generated.h"
#include "MatchingEvents_generated.h"

#include "flatbuffers/flatbuffers.h"

#pragma comment(lib, "ws2_32")

int SendPacket(SOCKET Socket, flatbuffers::FlatBufferBuilder& Builder);
int RecvPacket(SOCKET Socket, char* Buffer, int BufferSize);

// Helper function to create JSON response (minimal implementation)
std::string CreateJsonResponse(const std::string& name, bool result);