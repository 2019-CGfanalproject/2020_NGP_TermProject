#pragma once
#pragma comment(lib,"ws2_32")

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include<iostream>
#include<process.h>
#include <mutex>
#include <chrono>
#include <list>
#include <cmath>
#define SERVERPORT 9000
#define BUFSIZE	   512
using namespace std;

void error_quit(char* msg);

void error_display(const char* msg);

int recvn(SOCKET, char*, int, int);

void GameCommunicate(LPVOID arg);

void LobbyCummunicate(LPVOID arg);

void ResultCommunicate(LPVOID arg);
