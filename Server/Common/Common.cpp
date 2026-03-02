#include "Common.h"

int SendPacket(SOCKET Socket, flatbuffers::FlatBufferBuilder& Builder)
{
    // [size(4Byte-int)][FlatBuffer Payload(~)]
    const int PayloadSize = (int)Builder.GetSize();
    int NetPayloadSize = htonl(PayloadSize);

    int Sent = send(Socket, (char*)(&NetPayloadSize), sizeof(int), 0);
    if (Sent != sizeof(int))
    {
        return SOCKET_ERROR;
    }

    const char* Payload = (const char*)(Builder.GetBufferPointer());

 /*   char Buffer[4096] = { 0 };
    memcpy(Buffer + sizeof(int), Builder.GetBufferPointer(), (int)Builder.GetSize());*/

    int TotalSent = 0;
    while(TotalSent < PayloadSize)
    {
        int Sent = send(Socket, Payload + TotalSent, PayloadSize - TotalSent, 0);

        if (Sent == SOCKET_ERROR || Sent == 0)
        {
            return SOCKET_ERROR;
        }

        TotalSent += Sent;
    }

    return TotalSent;
}

int RecvPacket(SOCKET Socket, char* Buffer, int BufferSize)
{
    int PayloadSize = 0;

    // 1. payload size (4 bytes)
    int Received = 0;
    while (Received < sizeof(int))
    {
        int Recvbytes = recv(Socket, (char*)(&PayloadSize) + Received, sizeof(int) - Received, 0);

        if (Recvbytes <= 0)
        {
            return -1;
        }

        Received += Recvbytes;
    }

    PayloadSize = ntohl(PayloadSize);

    if (PayloadSize <= 0 || PayloadSize > BufferSize)
    {
        return -1;
    }

    // 2. payload ����
    Received = 0;
    while (Received < PayloadSize)
    {
        int Recvbytes = recv(Socket, Buffer + Received, PayloadSize - Received, 0);

        if (Recvbytes <= 0)
        {
            return -1;
        }

        Received += Recvbytes;
    }

    // ���� �� payload ũ�� ��ȯ
    return PayloadSize;
}

std::string CreateJsonResponse(const std::string& name, bool result)
{
    if (name.empty())
    {
        return "{\"result\": false}";
    }

    std::string escaped;
    escaped.reserve(name.size());
    for (char c : name)
    {
        switch (c)
        {
        case '"':  escaped += "\\\""; break;
        case '\\': escaped += "\\\\"; break;
        case '\n': escaped += "\\n"; break;
        case '\r': escaped += "\\r"; break;
        case '\t': escaped += "\\t"; break;
        default:   escaped += c; break;
        }
    }

    return "{\"name\": \"" + escaped + "\", \"result\": " + (result ? "true" : "false") + "}";
}
