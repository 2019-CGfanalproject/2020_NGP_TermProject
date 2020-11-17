#include "S_global.h"
#include "S_packet.h"

//void error_quit(char* msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL,
//		WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPSTR)&lpMsgBuf, 0, NULL);
//	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
//	LocalFree(lpMsgBuf);
//	exit(1);
//
//}

void error_display(char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}



//이용해서 %출력
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}


int main(int argc, char* argv[])
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET);// error_quit("socket()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR);// error_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);

	cout << "listen\n";
	if (retval == SOCKET_ERROR); //error_quit("listen()");
	HANDLE hThread;
	
	//데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	login_packet::CS_Nickname nicknamePacket;
	while (1) {
		//accept()

		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		cout << "accept\n";
		if (client_sock == INVALID_SOCKET) {
			error_display((char*)"accept()");
			break;
		}
		
		retval = recv(client_sock, (char*)nicknamePacket.buf, sizeof(nicknamePacket.buf), 0);
		
		cout << nicknamePacket.buf << endl;
	}



	closesocket(client_sock);
	closesocket(listen_sock);

	WSACleanup();

}