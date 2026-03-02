# Change Log - 2026-02-05

## Security Fixes

### Buffer Overflow in RecvPacket (Common.h, Common.cpp)
- `RecvPacket`에 `BufferSize` 파라미터 추가
- 네트워크에서 수신한 `PayloadSize`가 버퍼 크기를 초과하는지 검증
- 악의적인 클라이언트의 버퍼 오버플로우 공격 방지

### JSON Injection in CreateJsonResponse (Common.cpp)
- `name` 문자열에 JSON 특수문자(`"`, `\`, `\n`, `\r`, `\t`) 이스케이프 처리 추가
- JSON 인젝션 취약점 제거

### Unsalted Password Hashing (WebServer.cpp)
- SHA2 무솔트 해싱에 대한 TODO 주석 추가 (bcrypt 또는 per-user salt 도입 필요)

## Bug Fixes

### Null Connection Dereference (WebServer.cpp)
- DB 연결 실패 시 `Connection`이 `nullptr`인 상태에서 핸들러 접근 시 크래시 발생하던 문제 수정
- 두 핸들러(`/api/login`, `/api/check/ranking`)에 null 체크 추가, 503 응답 반환

### Memory Leak - ResultSet in Ranking Endpoint (WebServer.cpp)
- `/api/check/ranking` 핸들러에서 `res_db`가 delete 되지 않던 메모리 누수 수정

### Memory Leak on Exception in Login Endpoint (WebServer.cpp)
- 예외 발생 시 `PreparedStatement`, `res_db`가 해제되지 않던 문제 수정
- `std::unique_ptr`(RAII) 패턴으로 변경하여 자동 해제 보장

### Client GET/POST Mismatch (Client.cpp)
- 서버는 `/api/login`을 POST로 정의했으나 클라이언트가 GET으로 호출하던 문제 수정
- `cli.Post()`로 변경, JSON body(`user_id`, `passwd`) 전송하도록 수정

### Undefined Behavior in Client (Client.cpp)
- 입력값이 `'1'` 또는 `'2'`가 아닐 때 초기화되지 않은 `httplib::Result`에 접근하던 UB 수정
- 유효하지 않은 입력 안내 메시지 출력으로 변경

## Thread Safety

### Shared MySQL Connection (WebServer.cpp)
- `httplib::Server`가 멀티스레드로 동작하므로 공유 `Connection`에 `std::mutex` 추가
- 두 핸들러에 `std::lock_guard<std::mutex>` 적용

## Resource Management

### MySQL Connection Cleanup (WebServer.cpp)
- 서버 종료 시 `delete Connection` 추가

### WSACleanup (WebServer.cpp)
- `WSAStartup` 호출에 대응하는 `WSACleanup()` 추가

### select() Error Handling (GatewayServer.cpp)
- `select()`가 `SOCKET_ERROR` 반환 시 에러 로깅 후 루프 탈출 처리 추가

### RecvPacket Buffer Size (GatewayServer.cpp)
- `RecvPacket` 호출 시 `sizeof(Buffer)` 전달하여 버퍼 크기 제한 적용
