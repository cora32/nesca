﻿#pragma once
#include "STh.h"
#include "resource.h"

bool __savingBackUpFile = false;
bool horLineFlag = false;
QJsonArray *jsonArr = new QJsonArray();
static int portArr[65536] = {0};
int gThreadDelay = 10;
int gC = 0;
int gTimeOut = 3;
int PieAnomC1 = 0, PieBA = 0, PieSusp = 0, PieLowl = 0, PieWF = 0, PieSSH = 0;
int AnomC1 = 0, Filt = 0, Overl = 0, Lowl = 0, Alive = 0, saved = 0, Susp = 0, WF = 0, offlines = 0, ssh = 0;
int GlobalWSAErr = 0;
int GlobalNegativeSize = 0;
int ovrlIPs = 0, ipCounter = 0;
int mode;
int found = 0, fillerFlag = 0, indexIP = 1;
int gMaxSize = 65536;
int gMode;
int OnLiner = 0;
int MaxPass = 0, MaxLogin = 0, MaxTags = 0, MaxWFLogin = 0, MaxWFPass = 0, MaxSSHPass = 0;
int ipsstart[4], ipsend[4], 
	startNum, endNum, overallPorts, flCounter, octet[4];
unsigned char **ipsstartfl = NULL, **ipsendfl = NULL, **starterIP = NULL;
unsigned int importFileSize = 0;
int gPingTimeout = 2000;
double ips = 0;
char top_level_domain[128] = {0};
char startM[64] = {0}, endM[64] = {0};
char endIP2[128] = {0};
char **GlobalNegatives = 0;
char **loginLst, **passLst;
char **wfLoginLst, **wfPassLst;
char **sshlpLst;
char saveStartIP[128] = {0};
char saveEndIP[128] = {0};
char gRange[128] = {0};
char gFirstDom[128] = {0};
char gPorts[65536] = {0};
char metaIPDNS[256] = {0};
char metaRange[256] = {0};
char metaPercent[256] = {0};
char metaIPS[256] = {0};
char metaTargets[256] = {0};
char metaETA[256] = {0};
char metaOffline[256] = {0};
bool ErrLogFirstTime = true;
bool gPingNScan = false;
volatile bool ConnLocked = false;
unsigned long long gTargets = 0, gTargetsOverall = 1, targets, Activity = 0;
volatile int gThreads;
volatile int cons = 0;
volatile int BA = 0;
volatile int BrutingThrds = 0;
volatile int threads = 20;

void SaveErrorLog(char *sender, char *MesSent, char *ReplRecv)
{
	FILE *errFile = fopen("./logs/ns-track_errors.html", "r");
	if(errFile != NULL)
	{
		fclose(errFile);
	};
#if defined(WIN32)
	CreateDirectory(L"./logs", NULL);
#else
	struct stat st = {0};
	if (stat("./logs", &st) == -1) {
		mkdir("./logs", 0700);
	}
#endif
	time_t rtime;
	time(&rtime);
	char cdate[32] = {0};
	strcpy (cdate, ctime (&rtime));

	char *totalErrLog = NULL;
	char stylePart[] = {"<style>html{background-color:#373737;}#mainblock{background-color:#646464;width:100%;border:1px dotted black;}#sender-time{color:#ffffff;}#msr{color:#b9b9b9;}#msc{width:99,8%;border:1px solid black;}pre{width:99,8%;border:1px solid gray;white-space:-moz-pre-wrap;white-space:-pre-wrap;white-space:-o-pre-wrap;white-space:pre-wrap;word-wrap:break-word;}</style>"};
	char firstPart[] = {"<div id=\"mainblock\"><div id=\"sender-time\">"};
	char secondPart[] = {"</div><span id=\"msr\">Message sent:</span><pre>"};
	char thirdPart[] = {"</pre><span id=\"msr\">Reply received:</span><pre><iframe width=\"100%\" height=\"600px\" id=\"msc\" src=\"data:text/html;base64,"};
	char forthPart[] = {"\"></iframe></pre></div>"};
	
	int sz = strlen(stylePart) + strlen(firstPart) + strlen(secondPart) + strlen(thirdPart) + strlen(forthPart) + strlen(QByteArray(MesSent).replace("\r\n", "\n").data()) + (strlen(ReplRecv) + 50*strlen(ReplRecv)/100) + strlen(cdate) + strlen(sender);

	totalErrLog = new char[sz + 4];
	ZeroMemory(totalErrLog, sz);
	if(ErrLogFirstTime) strcat(totalErrLog, stylePart);
	strcat(totalErrLog, firstPart);
	strcat(totalErrLog, sender);
	strcat(totalErrLog, " - ");
	strcat(totalErrLog, cdate);
	strcat(totalErrLog, secondPart);
	strcat(totalErrLog, QByteArray(MesSent).replace("\r\n", "\n").data());
	strcat(totalErrLog, thirdPart);
	strcat(totalErrLog, base64_encode((const unsigned char *)ReplRecv, strlen(ReplRecv)).c_str());
	strcat(totalErrLog, forthPart);
	memset(totalErrLog + sz, '\0', 1);

	errFile = fopen("./logs/ns-track_errors.html", "a");
	if(errFile != NULL)	
	{
		fwrite(totalErrLog, sz, 1, errFile);
		fclose(errFile);
	}
	else
	{
		stt->doEmitionRedFoundData("[Log] -Cant open log file!");
	};
	if(totalErrLog != NULL)
	{
		delete []totalErrLog;
		totalErrLog = NULL;
	};
};
QString GetNSErrorDefinition(char *str, char *elem)
{
	char *temp = strstr(str, elem);

	if(temp != NULL)
	{
		char definition[128] = {0};
		char *firstComma = strstr(temp + strlen(elem) + 1, "\"");
		char *lastComma = strstr(firstComma + 1, "\"");

		int sz = lastComma - firstComma - 1;

		strncpy(definition, firstComma + 1, (sz < 128 ? sz : 128));

		return QString(definition);
	}
	else return QString("No definition found!");
};
void ConInc()
{
	while(ConnLocked) Sleep(20);
	
	ConnLocked = true;
	__asm
	{
		add cons, 1;
	};
	ConnLocked = false;
	#pragma region QTGUI_Area
	stt->doEmitionThreads(QString::number(cons) + "/" + QString::number(gThreads));
	#pragma endregion
};
volatile bool ConnLocked2 = false;
void ConDec()
{
	while(ConnLocked) Sleep(10 + (rand() % 5 + 1));
	
	ConnLocked = true;
	while(ConnLocked2) Sleep(18);
	ConnLocked2 = true;
	if(cons > 0)
	{
		__asm
		{
			sub cons, 1;
		};
	};
	ConnLocked2 = false;
	ConnLocked = false;
	#pragma region QTGUI_Area
	stt->doEmitionThreads(QString::number(cons) + "/" + QString::number(gThreads));
	#pragma endregion
};
void _SaveBackupToFile()
{
	char saveStr[512] = {0};
	char saveBuffer[65536] = {0};
	char endStr[128] = {0};

	if(gMode == 0 || gMode == 1)
	{
		if(gMode == 1) 
		{
			strcpy(endStr, endIP2);
		}
		else 
		{
			if(strstr(endIP2, "-") != NULL) strcpy(endStr, strstr(endIP2, "-"));
			else if(strstr(endIP2, "/") != NULL) strcpy(endStr, strstr(endIP2, "/"));
			else
			{
				char temp[512] = {0};
				strncpy(temp, endIP2, 512);
				strcat(endIP2, "-");
				strcat(endIP2, temp);
				strcpy(endStr, strstr(endIP2, "-"));
			};
		};

		if(strlen(endIP2) > 0)
		{
			strcpy(saveStr, "[SESSION]:");
			strcat(saveStr, std::to_string((long double)gMode).c_str());
			strcat(saveStr, " ");
			if(gMode == 0) strcat(saveStr, saveStartIP);
			strcat(saveStr, endStr);
			if(gMode == 1)
			{
				strcat(saveStr, " ");
				strcat(saveStr, top_level_domain);
			};
			strcat(saveStr, " ");
			strcat(saveStr, std::to_string((long double)gThreads).c_str());
			strcat(saveStr, " ");
			strcat(saveStr, gPorts);

			strcat(saveStr, "\n");
			strcat(saveBuffer, saveStr);
			ZeroMemory(saveStr, sizeof(saveStr));
		};
	}
	else if(gMode == -1)
	{
		char curIpStart[16] = {0};
		char curIpEnd[16] = {0};
		char ipRange[128] = {0};

		if(flCounter > 0)
		{
			FILE *savingFile = fopen("tempIPLst.bk", "w");
			if (savingFile != NULL)
			{
				for(int tCounter = gC; tCounter < flCounter; ++tCounter)
				{
					for(int i = 0; i < 4; ++i)
					{
						strcat(curIpStart, std::to_string((long double)ipsstartfl[tCounter][i]).c_str());
						if ( i != 3) strcat(curIpStart, ".");
						strcat(curIpEnd, std::to_string((long double)ipsendfl[tCounter][i]).c_str());
						if ( i != 3) strcat(curIpEnd, ".");
					};

					strcpy(ipRange, curIpStart);
					strcat(ipRange, "-");
					strcat(ipRange, curIpEnd);
					strcat(ipRange, "\n");

					fputs(ipRange, savingFile);

					ZeroMemory(ipRange, sizeof(ipRange));
					ZeroMemory(curIpStart, sizeof(curIpStart));
					ZeroMemory(curIpEnd, sizeof(curIpEnd));
				};
				fclose(savingFile);
			}
			else stt->doEmitionRedFoundData("[_saver] Cannot open file.");
		};

		strcpy(saveStr, "[SESSION]:");
		strcat(saveStr, std::to_string((long double)gMode).c_str());
		strcat(saveStr, " RESTORE_IMPORT_SESSION");
		strcat(saveStr, " ");
		strcat(saveStr, std::to_string((long double)gThreads).c_str());
		strcat(saveStr, " ");
		strcat(saveStr, gPorts);

		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
	};

		strcpy(saveStr, "[NDBSERVER]:");
		strcat(saveStr, trcSrv);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[NDBSCRIPT]:");
		strcat(saveStr, trcScr);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[NDBPORT]:");
		strcat(saveStr, trcSrvPortLine);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[PROXY]:");
		strcat(saveStr, trcProxy);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[PROXYPORT]:");
		strcat(saveStr, trcPort);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCSERVER]:");
		strcat(saveStr, ircServer);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCPORT]:");
		strcat(saveStr, ircPort);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCPROXY]:");
		strcat(saveStr, ircProxy);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCPROXYPORT]:");
		strcat(saveStr, ircProxyPort);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[IRCNICK]:");
		strcat(saveStr, ircNick);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
		
		strcpy(saveStr, "[PING]:");
		strcat(saveStr, gPingNScan ? "true" : "false");
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
		
		strcpy(saveStr, "[PING_TO]:");
		char tb[16] = {0};
		strcat(saveStr, itoa(gPingTimeout, tb, 10));
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[THREAD_DELAY]:");
		ZeroMemory(tb, 16);
		strcat(saveStr, itoa(gThreadDelay, tb, 10));
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
		
		strcpy(saveStr, "[TIMEOUT]:");
		ZeroMemory(tb, 16);
		strcat(saveStr, itoa(gTimeOut, tb, 10));
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
		
		strcpy(saveStr, "[MAXBTHR]:");
		ZeroMemory(tb, 16);
		strcat(saveStr, itoa(gMaxBrutingThreads, tb, 10));
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));

		strcpy(saveStr, "[PERSKEY]:");
		strcat(saveStr, trcPersKey);
		strcat(saveStr, "\n");
		strcat(saveBuffer, saveStr);
		ZeroMemory(saveStr, sizeof(saveStr));
		
		FILE *savingFile = fopen("restore", "w");

		if (savingFile != NULL)
		{	
			fputs(saveBuffer, savingFile);
			fclose(savingFile);
		}
		else stt->doEmitionRedFoundData("[_saver] Cannot open file.");

		ZeroMemory(saveStr, strlen(saveStr));
		ZeroMemory(saveBuffer, strlen(saveBuffer));
};
void _saver()	
{
	while(globalScanFlag)
	{
		__savingBackUpFile = true;
		_SaveBackupToFile();
		__savingBackUpFile = false;
		Sleep(10000);
	};
};

#if defined(WIN32)
void _timer()	
#else
void *_timer()	
#endif
{	
	char dbuffer[32] = {0}, timeLeft[64] = {0}, b[32] = {0};
	int ticks = 0;
	ovrlIPs = 0;
	ips = 1;
	Sleep(50);
	while(globalScanFlag)
	{
		++ticks;
		ovrlIPs += ipCounter;
		ipCounter = 0;
		ips = ovrlIPs/ticks;
		strncpy(b, QString::number(ips).toLocal8Bit().data(), 5);

		strcpy(metaIPS, b);
		strcat(timeLeft, b);
		strcat(timeLeft, "/s (");
		
		if(ips > 0) 
		{
			strncpy(dbuffer, std::to_string((long double)((gTargets + 1)/ips/3600/24)).c_str(), 5);
		}
		else strcpy(dbuffer, "INF");
		strcpy(metaETA, dbuffer);
		strcat(dbuffer, "d)");
		strcat(timeLeft,  (strcmp(dbuffer, "1.$d)") == 0 ? "INF)" : dbuffer));

		#pragma region QTGUI_Area
		stt->doEmitionIPS(QString(timeLeft));
		stt->doEmitionOffline(QString::number(offlines));
		#pragma endregion
		ZeroMemory(timeLeft, sizeof(timeLeft));
		ZeroMemory(dbuffer, sizeof(dbuffer));
		Sleep(1000);
	};
};
bool trackAlreadyGoing = false;
#if defined(WIN32)
void _tracker()
#else
void *_tracker()
#endif
{
	if(trackAlreadyGoing == false)
	{
		while(trackerOK)
		{
			if(globalScanFlag == false && jsonArr->size() == 0) break;
			char rBuffT[250000] = {0};
			char *msg = new char[4096];
			ZeroMemory(msg, sizeof(msg));
			char ndbServer[64] = {0};
			char ndbScriptT[64] = {0};
			char ndbScript[64] = {0};
		
		sockaddr_in sockAddr;  
		sockAddr.sin_family = AF_INET;  
		sockAddr.sin_port = htons(atoi(trcSrvPortLine));

		strcpy(msg, "GET /");
		strcat(msg, trcScr);
		strcat(msg, " HTTP/1.1\r\nHost: ");
		strcat(msg, trcSrv);
		strcat(msg, "\r\nX-Nescav3: True\r\nContent-Type: application/x-www-form-urlencoded\r\nConnection: close\r\n\r\n");

		HOSTENT *host; 

#if defined(WIN32)
		if(inet_addr(trcSrv) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(trcSrv);  
		else if(host=gethostbyname (trcSrv)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
#else
		if(inet_addr(trcSrv) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(trcSrv);  
		else if(host=gethostbyname (trcSrv)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
		SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ); 

		int test = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
		if(test == -1)
		{
			closesocket(sock);
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[NS-Track] -Cannot connect to balancer! " + QString::number(WSAGetLastError()) + ".");
#pragma endregion
			continue;
		};

		test = send(sock, msg, strlen(msg), 0);

		if(test == -1)
		{
			closesocket(sock);
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[NS-Track] -Cannot send to balancer! " + QString::number(WSAGetLastError()) + ".");
#pragma endregion
			continue;
		};

		ZeroMemory(rBuffT, sizeof(rBuffT));
		char rBuff[512] = {0};
		test = recv(sock, rBuff, sizeof(rBuff), 0);
		strcpy(rBuffT, rBuff);
		while((test = recv(sock, rBuff, sizeof(rBuff), 0)) != 0)
		{
			if(strlen(rBuffT) > 200000)
			{
				stt->doEmitionRedFoundData("[NS-Track] (Outer) -Large error received from server (>200000b) " + QString::number(WSAGetLastError()) + ".");
				SaveErrorLog("NS-Track", msg, rBuffT);
				break;
			};
			strcat(rBuffT, rBuff);
		};
		if(test == -1)
		{
			closesocket(sock);
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[NS-Track] -Cannot recv from balancer! " + QString::number(WSAGetLastError()) + ".");
#pragma endregion
			continue;
		};

		char *t1;
		char *t2;
		if(strstr(rBuffT, "http://") != NULL) 
		{
			t1 = strstr(rBuffT, "http://");
			if(strstr((char*)(t1 + strlen("http://")), "/") != NULL)
			{
				t2 = strstr((char*)(t1 + strlen("http://")), "/");
				int ln = t2 - t1 - strlen("http://");
				if(ln > 64)
				{
					closesocket(sock);
#pragma region QTGUI_Area
					stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");
					SaveErrorLog("NS-Track", msg, rBuffT);
#pragma endregion
					continue;
				}
				else strncpy(ndbServer, (char*)(t1 + strlen("http://")), ln);

				if(strlen(t2) > 64)
				{
#pragma region QTGUI_Area
					stt->doEmitionYellowFoundData("[NS-Track] -Fragmentation detected!");
#pragma endregion
					if(strstr(t2, "\r\n") != NULL)
					{
						char *t3 = strstr(t2, "\r\n");
						int y = (int)(t3 - t2);

						if(y > 64)
						{
							closesocket(sock);
#pragma region QTGUI_Area
							stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");
#pragma endregion
							SaveErrorLog("NS-Track", msg, rBuffT);
							continue;
						}
						else
						{
							strncpy(ndbScriptT, t2, y);
							closesocket(sock);
#pragma region QTGUI_Area
							stt->doEmitionGreenFoundData("[NS-Track] -OK! -Fragmented server string aquired! Starting NS-Track loop...");
#pragma endregion
							strncpy(ndbScript, ndbScriptT, strlen(ndbScriptT) );
						};
					}
					else
					{
						closesocket(sock);
#pragma region QTGUI_Area
						stt->doEmitionRedFoundData("[NS-Track] -Received server string is not valid!");
#pragma endregion
					SaveErrorLog("NS-Track", msg, rBuffT);
					continue;
					};
				} 
				else 
				{
					strcpy(ndbScriptT, t2);
#pragma region QTGUI_Area
					stt->doEmitionGreenFoundData("[NS-Track] -OK! -Server string aquired! Starting NS-Track loop...");
#pragma endregion
					closesocket(sock);
					strncpy(ndbScript, ndbScriptT, strlen(ndbScriptT) - 2 );
				};
			}
			else
			{
				closesocket(sock);
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[NS-Track] -Cannot receive script value!");
#pragma endregion
				continue;
			};

			ZeroMemory(rBuffT, sizeof(rBuffT));

			while(trackerOK)
			{
				if(globalScanFlag == false && jsonArr->size() == 0) break;
				trackAlreadyGoing = true;
				if(jsonArr->size() > 0)
				{
					QJsonObject jsonKey;
					if(jsonArr == NULL) jsonArr = new QJsonArray();
					
					QJsonObject jsonMeta;
						if(mode == 0) jsonMeta.insert("mode", QJsonValue(QString("IP")));				//
						else if(mode == 1) jsonMeta.insert("mode", QJsonValue(QString("DNS")));			//Mode
						else if(mode == -1) jsonMeta.insert("mode", QJsonValue(QString("Import")));		//
						jsonMeta.insert("range", QJsonValue(QString(metaRange)) );
						jsonMeta.insert("current", QJsonValue(QString(metaIPDNS)) );
						if(mode == 1) jsonMeta.insert("tld", QJsonValue(QString(top_level_domain)));			//TLD
						jsonMeta.insert("targets", QJsonValue(QString(metaTargets)) );
						jsonMeta.insert("percent", QJsonValue(QString(metaPercent)) );
						jsonMeta.insert("saved", QJsonValue(QString::number(saved)) );
						jsonMeta.insert("found", QJsonValue(QString::number(found)) );
						jsonMeta.insert("speed", QJsonValue(QString(metaIPS)) );
						jsonMeta.insert("eta", QJsonValue(QString(metaETA)) );
						jsonMeta.insert("threads", QJsonValue(QString::number(cons) + "/" + QString::number(gThreads)) );
						jsonMeta.insert("bads", QJsonValue(QString::number(offlines)) );
						jsonMeta.insert("version", QJsonValue(QString(gVER)) );

					jsonArr->push_front(QJsonValue(jsonMeta) );
					memset(trcPersKey + 32, '\0', 1);
					jsonKey.insert("key", QJsonValue(QString(trcPersKey)) );
					jsonArr->push_front(jsonKey);

					QJsonDocument js;
					js.setArray(*jsonArr);
					QByteArray r = js.toJson();
					
					sockAddr.sin_family = AF_INET;  
					sockAddr.sin_port = htons(atoi(trcSrvPortLine));

					if(msg != NULL) 
					{
						delete []msg;
						msg = 0;
					};
					msg = new char[r.size() + 1024];
					ZeroMemory(msg, sizeof(msg));

					strcpy(msg, "POST /");
					strcat(msg, ndbScript);
					strcat(msg, " HTTP/1.1\r\nHost: ");
					strcat(msg, ndbServer);
					strcat(msg, "\r\nContent-Type: application/json\r\nAccept-Encoding: application/json\r\nContent-Length: ");

					strcat(msg, std::to_string((long double)r.size()).c_str());
					strcat(msg, "\r\nConnection: close\r\n\r\n");

					strcat(msg, r.data());

					delete jsonArr;
					jsonArr = new QJsonArray();

#if defined(WIN32)
					if(inet_addr(ndbServer) != INADDR_NONE) sockAddr.sin_addr.S_un.S_addr = inet_addr(ndbServer);  
					else if(host=gethostbyname (ndbServer)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];  
#else
					if(inet_addr(ndbServer) != INADDR_NONE) sockAddr.sin_addr.s_addr = inet_addr(ndbServer);  
					else if(host=gethostbyname (ndbServer)) ((unsigned long*) &sockAddr.sin_addr)[0] = ((unsigned long**)host->h_addr_list)[0][0];
#endif
					sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ); 

					if(gDebugMode)
					{
						stt->doEmitionDebugFoundData("Connecting to " + QString(ndbServer));
					};
					test = connect(sock, (sockaddr*)&sockAddr, sizeof(sockAddr));
					if(test == -1)
					{
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[NS-Track] -connect() returned " + QString::number(WSAGetLastError()) + ".");
#pragma endregion
					continue;
					};

					if(gDebugMode)
					{
						stt->doEmitionDebugFoundData("Sending!");
						stt->doEmitionDebugFoundData("Key: [" + QString(trcPersKey) + "]");
						stt->doEmitionDebugFoundData("MSG: [" + QString(msg) + "]");
					};
					test = send(sock, msg, strlen(msg), 0);

					if(test == -1)
					{
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[NS-Track] -send() returned " + QString::number(WSAGetLastError()) + ".");
#pragma endregion
						SaveErrorLog("NS-Track", msg, "");
						continue;
					};

					ZeroMemory(rBuffT, sizeof(rBuffT));
					char msgR[32] = {0};

					if(gDebugMode)
					{
						stt->doEmitionDebugFoundData("Receiving...");
					};
					test = recv(sock, rBuff, 512, 0);
					if(gDebugMode)
					{
						stt->doEmitionDebugFoundData("Received: " + QString(rBuff));
					};
					strncpy(msgR, rBuff, 32);
					strcpy(rBuffT, rBuff);
					while(test > 0)
					{
						if(test <= 0) break;

						if(strlen(rBuffT) > 200000)
						{
							stt->doEmitionRedFoundData("[NS-Track] (Inner) -Large error received from server (>200000b) " + QString::number(WSAGetLastError()) + ".");
							SaveErrorLog("NS-Track", msg, rBuffT);
							break;
						};
						strcat(rBuffT, rBuff);
						test = recv(sock, rBuff, 512, 0);
						if(gDebugMode)
						{
							stt->doEmitionDebugFoundData("Received: " + QString(rBuff));
						};
					};
					if(test == -1)
					{
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[NS-Track] -recv() returned " + QString::number(WSAGetLastError()) + ".");
#pragma endregion
						SaveErrorLog("NS-Track", msg, "");
						continue;
					};
					if(strstr(rBuffT, "201 Created") != NULL)
					{
#pragma region QTGUI_Area
						if(gDebugMode) stt->doEmitionYellowFoundData("[NS-Track] -OK. Data saved!");
						stt->doEmitionDataSaved(true);
						Sleep(1000);
						stt->doEmitionDataSaved(false);
#pragma endregion
					}
					else if(strstr(rBuffT, "400 Bad Request") != NULL)
					{
#pragma region QTGUI_Area
						QString errorDef = GetNSErrorDefinition(rBuffT, "notify");
						if(errorDef == "Invalid access key") stt->doEmitionYellowFoundData("[NS-Track] [Key is unauthorized] A valid key is required.");
						else stt->doEmitionYellowFoundData("[NS-Track] -FAIL! [400 Bad Request : " + errorDef + "]");
#pragma endregion
						SaveErrorLog("NS-Track", msg, rBuffT);
					}
					else
					{
#pragma region QTGUI_Area
				stt->doEmitionYellowFoundData("[NS-Track] -FAIL! An error occured [" + QString(msgR) + "]");
#pragma endregion
						SaveErrorLog("NS-Track", msg, rBuffT);
					};

					ZeroMemory(msgR, sizeof(msgR));			
					ZeroMemory(rBuffT, sizeof(rBuffT));			
					ZeroMemory(msg, sizeof(msg));
					if(msg != NULL) 
					{
						delete []msg;
						msg = 0;
					};

					shutdown(sock, 2);
					closesocket(sock);
				};
				Sleep(10000);
			};
		}
		else
		{
			shutdown(sock, 2);
			closesocket(sock);
#pragma region QTGUI_Area
			stt->doEmitionRedFoundData("[NS-Track] -Balancer replied with invalid string.");
			SaveErrorLog("NS-Track", msg, rBuffT);
#pragma endregion
		};

	};
	trackAlreadyGoing = false;
	};
};

unsigned long int numOfIps(int ipsstart[], int ipsend[])
{
	gTargets += 256*256*256*(ipsend[0] - ipsstart[0]);
	gTargets += 256 * 256 * (ipsend[1] - ipsstart[1]);
	gTargets += 256 * (ipsend[2] - ipsstart[2]);
	gTargets += (ipsend[3] - ipsstart[3]);
	gTargetsOverall = gTargets - 1;
	return gTargets;
};
//#include <sys/types.h>
//#include <stdio.h>
//#include <signal.h>
//#include <adns.h>
//char *GetHost(char *ip)
//{
//	char res[256] = {0};
//	adns_state ads = NULL;
//	adns_init(&ads, adns_if_nosigpipe, 0);
//
//	adns_query aquery = 0;
//	adns_answer* answer = NULL;
//	sockaddr_in sockAddr;  
//	sockAddr.sin_family = AF_INET;
//	sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);
//
//	adns_submit_reverse(ads, 
//		(struct sockaddr *)&sockAddr, 
//		(adns_rrtype)adns_r_ptr, 
//		(adns_queryflags)adns_qf_quoteok_anshost, 
//		NULL, 
//		&aquery);
//	adns_wait(ads, &aquery, &answer, NULL); 
//	
//	if(answer->status == adns_s_ok)
//	{
//		strncpy(res, (const char*)answer->rrs.bytes, 256);
//	};
//
//	adns_finish(ads);
//	//struct hostent *result;
//	//unsigned long addr = inet_addr(ip);
//	//result = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
//	return res;
//
//#pragma region Reverse-ip
//	//struct hostent *result;
//	//unsigned long addr = inet_addr(ip);
//	//result = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
//	//char res[256] = {0};
//
//	//if( result == NULL ) 
//	//{
//	//	strcpy(res, "");
//	//} 
//	//else 
//	//{
//	//	if( result->h_name == NULL ) 
//	//	{
//	//		strcpy(res, "");
//	//	}
//	//	else 
//	//	{
//	//		strcpy(res, "Hostname: ");
//	//		//if(result->h_length != NULL) strcat(res, std::to_string((long double)result->h_length).c_str());
//	//		//strcat(res, " ");
//	//		//if(result->h_name != NULL) strcat(res, (result->h_name));
//	//		//strcat(res, ":");
//	//		if(result->h_addr_list[0] != NULL) 
//	//		{
//	//			int sz = strlen((result->h_addr_list[0] + 4));
//	//			if(sz > 200) stt->doEmitionYellowFoundData("[LOL] Very long host detected [<a href=\"http://" + QString(ip) + "/\">" + QString(ip) + "</a>]");
//	//			strncpy(res, (result->h_addr_list[0] + 4), sz < 256 ? sz : 256);
//	//		};
//	//	};
//	//};
//#pragma endregion
//
//	return res;
//};

Connector con;
#if defined(WIN32)
void _connect(void* ss)
#else
void *_connect(void* ss)
#endif
{	
	++ipCounter;
	char ip[MAX_ADDR_LEN] = {0};
	strcpy(ip, ((sockstruct*)ss)->argv);
	//char hostLog[256] = {0};
	//strcpy(hostLog, GetHost(ip));
	delete []ss;

	for(int i = 0; i <= overallPorts; ++i)
	{
		if(globalScanFlag == false) break;		
		con._ConnectToPort( ip, std::to_string((long double)portArr[i]).c_str(), "" );
	};

	ConDec();
};
void targetAndIPWriter(unsigned long int target, char *buff)
{
	char curIPBuff[256] = {0}, targetNPers[32] = {0}, dbuffer[32] = {0};
	strcpy(metaIPDNS, buff);
	char b[32] = {0};
	sprintf(b, "%Lu", target);
	strcpy(targetNPers, b);
	strcpy(metaTargets, targetNPers);
	if(gTargetsOverall != 0) sprintf(dbuffer, "%.1f", (100 - target/(double)gTargetsOverall * 100));
	else strcpy(dbuffer, "0");
	strcat(targetNPers, " (");
	strcat(targetNPers, dbuffer);
	strcat(targetNPers, "%)");
	strcpy(metaPercent, dbuffer);

	strcpy(curIPBuff, "--->");
	strcat(curIPBuff, buff);
		
		#pragma region QTGUI_Area
		stt->doEmitionIPRANGE(QString(curIPBuff));
		stt->doEmitionTargetsLeft(QString(targetNPers));
		#pragma endregion
};
void _passLoginFapper()
{
	MaxLogin = 0;
	MaxPass = 0;

	FILE *loginList;
	FILE *passList;
	
	char buffFG[32] = {0};
	int i = 0;

	loginList = fopen("login.txt", "r");
	passList = fopen("pass.txt", "r");

	if(passList != NULL && loginList != NULL)
	{
		while(fgets(buffFG, 32, loginList) != NULL)
		{
			MaxLogin++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		while(fgets(buffFG, 32, passList) != NULL)
		{
			MaxPass++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(loginList);
		rewind(passList);

		loginLst = new char*[MaxLogin];
		passLst = new char*[MaxPass];

		for(int j = 0; j < MaxLogin; j++)
		{
			loginLst[j] = new char[32];
		};

		for(int j = 0; j < MaxPass; j++)
		{
			passLst[j] = new char[32];
		};

		while(fgets(buffFG, 32, passList) != NULL)
		{
			memset(passLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(passLst[i++], buffFG, strlen(buffFG) - 1);
			else  strncat(passLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

	#pragma region QTGUI_Area
		stt->doEmitionGreenFoundData("Password list loaded (" + QString(std::to_string((long double)MaxPass).c_str()) + " entries)");
	#pragma endregion

		i = 0;

		while(fgets(buffFG, 32, loginList) != NULL)
		{
			memset(loginLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(loginLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(loginLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		#pragma region QTGUI_Area
		stt->doEmitionGreenFoundData("Login list loaded (" + QString(std::to_string((long double)MaxLogin).c_str()) + " entries)");
		#pragma endregion

		fclose(loginList);
		fclose(passList);
	} 
	else 
	{
		#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("No password/login list found");
		stt->doEmitionKillSttThread();
		#pragma endregion
	};

	MaxWFLogin = 0;
	MaxWFPass = 0;

	FILE *wfLoginList;
	FILE *wfPassList;
	
	ZeroMemory(buffFG, sizeof(buffFG));
	i = 0;

	wfLoginList = fopen("wflogin.txt", "r");
	wfPassList = fopen("wfpass.txt", "r");

	if(wfPassList != NULL && wfLoginList != NULL)
	{
		while(fgets(buffFG, 32, wfLoginList) != NULL)
		{
			MaxWFLogin++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		while(fgets(buffFG, 32, wfPassList) != NULL)
		{
			MaxWFPass++;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(wfLoginList);
		rewind(wfPassList);

		wfLoginLst = new char*[MaxWFLogin];
		wfPassLst = new char*[MaxWFPass];

		for(int j = 0; j < MaxWFLogin; j++)
		{
			wfLoginLst[j] = new char[32];
		};

		for(int j = 0; j < MaxWFPass; j++)
		{
			wfPassLst[j] = new char[32];
		};

		while(fgets(buffFG, 32, wfPassList) != NULL)
		{
			memset(wfPassLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(wfPassLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(wfPassLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

	#pragma region QTGUI_Area
		stt->doEmitionGreenFoundData("WFPassword list loaded (" + QString(std::to_string((long double)MaxWFPass).c_str()) + " entries)");
	#pragma endregion

		i = 0;

		while(fgets(buffFG, 32, wfLoginList) != NULL)
		{
			memset(wfLoginLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(wfLoginLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(wfLoginLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		#pragma region QTGUI_Area
		stt->doEmitionGreenFoundData("WFLogin list loaded (" + QString(std::to_string((long double)MaxWFLogin).c_str()) + " entries)");
		#pragma endregion

		fclose(wfLoginList);
		fclose(wfLoginList);
	} 
	else 
	{
		#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("No password/login list found");
		stt->doEmitionKillSttThread();
		#pragma endregion
	};

#pragma region SSHPASS
	MaxSSHPass = 0;

	FILE *sshlpList;
	
	ZeroMemory(buffFG, sizeof(buffFG));
	i = 0;

	sshlpList = fopen("sshpass.txt", "r");

	if(sshlpList != NULL)
	{
		while(fgets(buffFG, 32, sshlpList) != NULL)
		{
			++MaxSSHPass;
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(sshlpList);

		sshlpLst = new char*[MaxSSHPass];

		for(int j = 0; j < MaxSSHPass; j++)
		{
			sshlpLst[j] = new char[32];
		};

		while(fgets(buffFG, 32, sshlpList) != NULL)
		{
			memset(sshlpLst[i], 0, strlen(buffFG) + 1);

			if(strstr(buffFG, "\n") != NULL) strncat(sshlpLst[i++], buffFG, strlen(buffFG) - 1);
			else strncat(sshlpLst[i++], buffFG, strlen(buffFG));
			ZeroMemory(buffFG, sizeof(buffFG));
		};

	#pragma region QTGUI_Area
		stt->doEmitionGreenFoundData("SSH Password list loaded (" + QString(std::to_string((long double)MaxSSHPass).c_str()) + " entries)");
	#pragma endregion
		
		fclose(sshlpList);
	} 
	else 
	{
		#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("No password/login list found");
		stt->doEmitionKillSttThread();
		#pragma endregion
	};
#pragma endregion

	#pragma region QTGUI_Area
	stt->doEmitionYellowFoundData("BA: ~" + QString(std::to_string((long double)MaxLogin * MaxPass/gTimeOut/60).c_str()) + "; WF: ~" + QString(std::to_string((long double)MaxWFLogin * MaxWFPass/gTimeOut/60).c_str()) + "; SSH: ~" + QString(std::to_string((long double)MaxSSHPass/gTimeOut/60).c_str()));
	#pragma endregion
};
void ReadUTF8(FILE* nFile, char *cp)
{
	char buffFG[256] = {0};
	int i = 0;
	GlobalNegativeSize = 0;

	if(nFile != NULL)
	{
		while(fgets((char*)buffFG, sizeof(buffFG), nFile) != NULL)
		{
			if(buffFG[0] != '#' && buffFG[0] != ' ' && buffFG[0] != '\n' && buffFG[0] != '\r' && strcmp(buffFG, "") != 0 && 
				((buffFG[0] == '/' && buffFG[1] == '/') == false) && ((buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t' && (buffFG[3] == 13 || buffFG[3] == 10 || buffFG[3] == '#')) == false)
				&& (buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t' && (buffFG[3] == '/' && buffFG[4] == '/')) == false)
			{
				++GlobalNegativeSize;
			};
			ZeroMemory(buffFG, sizeof(buffFG));
		};

		rewind(nFile);
		if(strcmp(cp, "UTF") == 0) fseek(nFile, 3, 0);

		GlobalNegatives = new char*[GlobalNegativeSize + 2];
		
		char buffcpy[256] = {0};
		while(fgets(buffFG, sizeof(buffFG), nFile) != NULL)
		{
			if(buffFG[0] == '#' || buffFG[0] == ' ' || buffFG[0] == '\n' || buffFG[0] == '\r' || strcmp(buffFG, "") == 0 || 
				(buffFG[0] == '/' && buffFG[1] == '/')) 
			{
				ZeroMemory(buffFG, sizeof(buffFG));
				continue;
			};

			if(buffFG[0] == '\t' && buffFG[1] == '\t' && buffFG[2] == '\t')
			{
				char buffFGT[256] = {0};
				strcpy(buffFGT, buffFG);
				char *ptr1 = strstr(buffFGT, "\t\t\t");
				ZeroMemory(buffFG, sizeof(buffFG));
				strcpy(buffFG, ptr1 + 3);
			};

			int bSz = strlen(buffFG);
			if((bSz == 2 && buffFG[0] == 13 && buffFG[1] == 10) || (bSz == 1 && (buffFG[0] == 13 || buffFG[0] == 10)))
			{
				ZeroMemory(buffFG, sizeof(buffFG));
				continue;
			};
			if(buffFG[bSz] == 13 || buffFG[bSz] == 10) 
			{
				buffFG[bSz] = '\0';
			};
			if(buffFG[bSz - 1] == 13 || buffFG[bSz - 1] == 10) 
			{
				buffFG[bSz - 1] = '\0';
			};
			if(buffFG[bSz - 2] == 13 || buffFG[bSz - 2] == 10) 
			{
				buffFG[bSz - 2] = '\0';
			};

			if(strstr((char*)buffFG, "\n") != 0) 
			{				
				std::string res = xcode(buffFG, CP_UTF8, CP_ACP);
				int sz = res.size();
				GlobalNegatives[i] = new char[sz + 1];
				ZeroMemory(GlobalNegatives[i], sizeof(GlobalNegatives[i]));
				memcpy(GlobalNegatives[i], toLowerStr(res.c_str()).c_str(), sz - 1);
				memset(GlobalNegatives[i] + sz - 1, '\0', 1);
				++i;
			}
			else 
			{
				std::string res = xcode(buffFG, CP_UTF8, CP_ACP);
				int sz = res.size();
				GlobalNegatives[i] = new char[sz + 1];
				ZeroMemory(GlobalNegatives[i], sizeof(GlobalNegatives[i]));
				memcpy(GlobalNegatives[i], toLowerStr(res.c_str()).c_str(), sz);
				memset(GlobalNegatives[i] + sz, '\0', 1);
				++i;
			};
		
			unsigned char buffcpy2[256] = {0};
			int sz = strlen((char*)buffFG);
#ifdef WIN32
			strncpy((char*)buffcpy2, xcode(buffFG, CP_ACP, CP_UTF8).c_str(), sz);
#else
			strncpy((char*)buffcpy2, buffFG, sz);
#endif

			ZeroMemory(buffFG, sizeof(buffFG));
		};

		#pragma region QTGUI_Area
		stt->doEmitionGreenFoundData("Negative list loaded (" + QString::number(GlobalNegativeSize) + " entries)");
		#pragma endregion
		ZeroMemory(buffFG, sizeof(buffFG));

		fclose(nFile);
	}
	else
	{
		#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("No negative list found");
		stt->doEmitionKillSttThread();
		#pragma endregion
	};
}
#ifdef WIN32
string xcode(LPCSTR src, UINT srcCodePage, UINT dstCodePage)
{
	string res;
    int wsize = MultiByteToWideChar(srcCodePage, 0, src, -1, 0, 0);
    LPWSTR wbuf = (LPWSTR)new char[wsize * sizeof(WCHAR)];
    MultiByteToWideChar(srcCodePage, 0, src, -1, wbuf, wsize);
    int size = WideCharToMultiByte(dstCodePage, 0, wbuf, -1, 0, 0, 0, 0);
    char * buf = (char *)new char[size];
    WideCharToMultiByte(dstCodePage, 0, wbuf, -1, buf, size, 0, 0);
    delete [] wbuf;
	res.append(buf);
    delete [] buf;
	return res;
}
#endif
void _NegativeFapper()
{
	FILE *nFile = fopen("negatives.txt", "rb");
	char buffFG[256] = {0};
	unsigned char buffcpy[256] = {0};

	if( nFile != NULL)
	{
		unsigned char b[3] = {0};
		fread(b,1,2, nFile);
		if( b[0] == 0xEF && b[1] == 0xBB)
		{
			fread(b,1,1,nFile); // 0xBF
			ReadUTF8(nFile, "UTF");
		}
		else
		{
			ReadUTF8(nFile, "1251");
		};
    }
};
void CheckMaskBits(char *res, int index)
{	
	char *bitsStr = strstr(res, "/");
	int bitsNum = atoi(bitsStr + 1);
	int finalBit = 1;
	int bitCounter = 0;

	if(bitsNum <= 32 && bitsNum > 24) //4 octet
	{
		bitCounter = 32 - bitsNum;
		for(int i = 0; i < bitCounter; i++) finalBit *= 2;
		ipsstartfl[index][0] = ipsendfl[index][0] = octet[0];
		ipsstartfl[index][1] = ipsendfl[index][1] = octet[1];
		ipsstartfl[index][2] = ipsendfl[index][2] = octet[2];
		ipsstartfl[index][3] = octet[3];
		ipsendfl[index][3] = octet[3] + finalBit - 1;

	}
	else if(bitsNum <= 24 && bitsNum > 16) //3 octet
	{
		bitCounter = 24 - bitsNum;
		for(int i = 0; i < bitCounter; i++) finalBit *= 2;
		ipsstartfl[index][0] = ipsendfl[index][0] = octet[0];
		ipsstartfl[index][1] = ipsendfl[index][1] = octet[1];
		ipsstartfl[index][2] = octet[2];
		ipsendfl[index][2] = octet[2] + finalBit - 1;
		ipsstartfl[index][3] = octet[3];
		ipsendfl[index][3] = 255;
	}
	else if(bitsNum <= 16 && bitsNum > 8) //2 octet
	{
		bitCounter = 16 - bitsNum;
		for(int i = 0; i < bitCounter; i++) finalBit *= 2;
		ipsstartfl[index][0] = ipsendfl[index][0] = octet[0];
		ipsstartfl[index][1] = octet[1];
		ipsendfl[index][1] = octet[1] + finalBit - 1;
		ipsstartfl[index][2] = octet[2];
		ipsendfl[index][2] = 255;		
		ipsstartfl[index][3] = octet[3];
		ipsendfl[index][3] = 255;

	}
	else if(bitsNum <= 8 && bitsNum > 0) //1 octet
	{
		bitCounter = 8 - bitsNum;
		for(int i = 0; i < bitCounter; i++) finalBit *= 2;
		ipsstartfl[index][0] = octet[0];
		ipsendfl[index][0] = octet[0] + finalBit - 1;
		ipsstartfl[index][1] = octet[1];
		ipsendfl[index][1] = 255;
		ipsstartfl[index][2] = octet[2];
		ipsendfl[index][2] = 255;		
		ipsstartfl[index][3] = octet[3];
		ipsendfl[index][3] = 255;
	}
	else
	{
#pragma region QTGUI_Area
				stt->doEmitionRedFoundData("[CheckMaskBits] Cannot parse IP list");
				stt->doEmitionKillSttThread();
#pragma endregion
	};
};
void GetOctets(char *curIP)
{
	char *str1;
	char *str2;
	char temp[8] = {0};

	if(strstr(curIP, ".") != NULL) 
	{
		str1 = strstr(curIP, "."); //1
		strncpy(temp, curIP, str1 - curIP);
		octet[0] = atoi(temp);

		ZeroMemory(temp, sizeof(temp));
	}
	else
	{
		str1 = strstr(curIP, "/");
		strncpy(temp, curIP, str1 - curIP);

		octet[0] = atoi(temp);
		octet[1] = 0;
		octet[2] = 0;
		octet[3] = 0;
		return;
	};

	if(strstr(str1 + 1, ".") != NULL)
	{
		str2 = strstr(str1 + 1, "."); //2
		strncpy(temp, str1 + 1, str2 - str1);
		octet[1] = atoi(temp);

		ZeroMemory(temp, sizeof(temp));
	}
	else
	{		
		str2 = strstr(str1 + 1, "/");
		strncpy(temp, str1 + 1, str2 - str1);

		octet[1] = atoi(temp);
		octet[2] = 0;
		octet[3] = 0;
		return;
	};

	if(strstr(str2 + 1, ".") != NULL) 
	{
		str1 = strstr(str2 + 1, "."); //3
		strncpy(temp, str2 + 1, str1 - str2);
		octet[2] = atoi(temp);

		ZeroMemory(temp, sizeof(temp));
	}
	else
	{
		str1 = strstr(str2 + 1, "/");
		strncpy(temp, str2 + 1, str1 - str2);

		octet[2] = atoi(temp);
		octet[3] = 0;
		return;
	};

	if(strstr(str1 + 1, ".") != NULL) 
	{
		str2 = strstr(str1 + 1, "."); //4
		strncpy(temp, str1 + 1, str2 - str1);
		octet[3] = atoi(temp);

		ZeroMemory(temp, sizeof(temp));
	}
	else
	{
		str2 = strstr(str1 + 1, "/");
		strncpy(temp, str1 + 1, str2 - str1 - 1);

		octet[3] = atoi(temp);
		return;
	};
};
int fInit(int InitMode, char *gR)
{
	strcpy(metaRange, gR);
	if (InitMode == 0) 
	{
		if(strstr(gR, "/") != NULL)
		{
			++flCounter;

			char *str1;
			char *str2;
			char res[8] = {0}; 

			GetOctets(gR);

			if(strstr(gR, ".") != NULL) 
			{
				str1 = strstr(gR, ".");  //1 byte
				strncpy(res, gR, (int)((char*)str1 - gR));
			}
			else strcpy(res, gR);

			if(strstr(res, "/") != NULL)
			{
				CheckMaskBits(res, flCounter);
			}
			else
			{
				starterIP[flCounter][0] = atoi(res);
				ipsstartfl[flCounter][0] = atoi(res);
				ipsendfl[flCounter][0] = atoi(res);
			};
			ZeroMemory(res, sizeof(res));

			if(strstr(str1 + 1, ".") != NULL) 
			{
				str2 = strstr(str1 + 1, "."); //2 byte
				strncpy(res, str1 + 1, (int)((char*)str2 - str1) - 1);
			}
			else strcpy(res, str1 + 1);

			if(strstr(res, "/") != NULL)
			{
				CheckMaskBits(res, flCounter);
			}
			else
			{
				starterIP[flCounter][1] = atoi(res);
				ipsstartfl[flCounter][1] = atoi(res);
				ipsendfl[flCounter][1] = atoi(res);
			};

			ZeroMemory(res, sizeof(res));

			if(strstr(str2 + 1, ".") != NULL) 
			{
				str1 = strstr(str2 + 1, "."); //3 byte
				strncpy(res, str2 + 1, (int)((char*)str1 - str2) - 1);
			}
			else strcpy(res, str2 + 1);

			if(strstr(res, "/") != NULL)
			{
				CheckMaskBits(res, flCounter);
			}
			else
			{
				starterIP[flCounter][2] = atoi(res);
				ipsstartfl[flCounter][2] = atoi(res);
				ipsendfl[flCounter][2] = atoi(res);
			};
			ZeroMemory(res, sizeof(res));

			if(strstr(str1 + 1, ".") != NULL) 
			{
				str2 = strstr(str1 + 1, "."); //4 byte
				strncpy(res, str1 + 1, (int)((char*)str2 - str1) - 1);
			}
			else strcpy(res, str1 + 1);

			if(strstr(res, "/") != NULL)
			{
				CheckMaskBits(res, flCounter);
			}
			else
			{
				starterIP[flCounter][3] = atoi(res);
				ipsstartfl[flCounter][3] = atoi(res);
				ipsendfl[flCounter][3] = atoi(res);
			};

			ipsstart[0] = ipsstartfl[flCounter][0];
			ipsstart[1] = ipsstartfl[flCounter][1];
			ipsstart[2] = ipsstartfl[flCounter][2];
			ipsstart[3] = ipsstartfl[flCounter][3];

			ipsend[0] = ipsendfl[flCounter][0];
			ipsend[1] = ipsendfl[flCounter][1];
			ipsend[2] = ipsendfl[flCounter][2];
			ipsend[3] = ipsendfl[flCounter][3];
		}
		else
		{
			int x; 
			char des1[64] = {0};

			for(int i = 0; i < 3; i++)																		//Filling the range-starting ip mass.
			{
				x = strcspn(gR, ".");
				memcpy(des1, gR, x*sizeof(int));
				memset(gR, ' ', x + 1);
				ipsstart[i] = atoi(des1);
				ZeroMemory(des1, sizeof(des1));
				if(ipsstart[i] > 255) 
				{
					stt->doEmitionRedFoundData("[Error] Incorrect range.");
					stt->doEmitionKillSttThread();
					return -1;
				};
			};

			x = strcspn(gR, "-");
			if(strstr(gR, "-") == NULL)
			{
				stt->doEmitionRedFoundData("[Error] Incorrect range.");
				stt->doEmitionKillSttThread();
				return -1;
			};

			memcpy(des1, gR, x);
			ipsstart[3] = atoi(des1);
			memset(gR, ' ', x + 1);

			if(strstr(gR, ".") == NULL)
			{
				stt->doEmitionRedFoundData("[Error] Incorrect range.");
				stt->doEmitionKillSttThread();
				return -1;
			};

			for(int i = 0; i < 4; i++)																		//Filling the range-closing ip mass.
			{
				x = strcspn(gR, ".");
				memcpy(des1, gR, x);
				memset(gR, ' ', x + 1);
				ipsend[i] = atoi(des1);

				if(ipsend[i] > 255) 
				{
					stt->doEmitionRedFoundData("[Error] Incorrect range.");
					stt->doEmitionKillSttThread();
					return -1;
				};
			};

		};
		if((ipsstart[0] > ipsend[0]) 
			|| 
			(
			(ipsstart[0] >= ipsend[0]) && (ipsstart[1] > ipsend[1])
			) || 
			(
			(ipsstart[0] >= ipsend[0]) && (ipsstart[1] >= ipsend[1]) && (ipsstart[2] > ipsend[2])
			) || 
			(
			(ipsstart[0] >= ipsend[0]) && (ipsstart[1] >= ipsend[1]) && (ipsstart[2] >= ipsend[2]) && (ipsstart[3] > ipsend[3]) 
			)
			) 
		{
			stt->doEmitionRedFoundData("[Error] Incorrect range.");
			stt->doEmitionKillSttThread();
			return -1;
		};

		targets = numOfIps(ipsstart, ipsend);
	}
	else if (InitMode == -1)
	{
		//targets = numOfIpsFL();
		///DUMMY///
	};
};
void FileLoader(char *str)
{
	char res[64] = {0}; 
	char curIP[64] = {0}, curIPCopy[64] = {0};
	char tempBuff[4] = {0};

	FILE *fl = fopen(str, "r");
	if(fl != NULL)
	{
		while(fgets((char*)curIP, sizeof(curIP), fl) != NULL)
		{
			if(curIP[0] != '#' && curIP[0] != ' ' && curIP[0] != '\n' && curIP[0] != '\r' && strcmp(curIP, "") != 0 && 
				((curIP[0] == '/' && curIP[1] == '/') == false) && ((curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == 13 || curIP[3] == 10 || curIP[3] == '#')) == false)
				&& (curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == '/' && curIP[4] == '/')) == false)
			{
				++importFileSize;
			};
			ZeroMemory(curIP, sizeof(curIP));
		};

		ipsstartfl = new unsigned char*[importFileSize + 2];
		ipsendfl = new unsigned char*[importFileSize + 2];
		starterIP = new unsigned char*[importFileSize + 2];
		ZeroMemory(ipsstartfl, importFileSize);
		ZeroMemory(ipsendfl, importFileSize);
		ZeroMemory(starterIP, importFileSize);
		for(int i = 0; i < importFileSize; ++i)
		{
			ipsstartfl[i] = new unsigned char[4];
			ipsendfl[i] = new unsigned char[4];
			starterIP[i] = new unsigned char[4];
			
			ZeroMemory(ipsstartfl[i], 4);
			ZeroMemory(ipsendfl[i], 4);
			ZeroMemory(starterIP[i], 4);
		};

		rewind(fl);

		while(fgets(curIP, 64, fl) != NULL)
		{
			if(curIP[0] != '#' && curIP[0] != ' ' && curIP[0] != '\n' && curIP[0] != '\r' && strcmp(curIP, "") != 0 && 
				((curIP[0] == '/' && curIP[1] == '/') == false) && ((curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == 13 || curIP[3] == 10 || curIP[3] == '#')) == false)
				&& (curIP[0] == '\t' && curIP[1] == '\t' && curIP[2] == '\t' && (curIP[3] == '/' && curIP[4] == '/')) == false)
			{
				strcpy(curIPCopy, curIP);
				char *ptr1 = strstr(curIP, " ");
				if(ptr1 != NULL) curIP[(int)(ptr1 - curIP)] = '\0';
				ptr1 = strstr(curIP, "	");
				if(ptr1 != NULL) curIP[(int)(ptr1 - curIP) - 1] = '\0';
				ptr1 = strstr(curIP, "#");
				if(ptr1 != NULL) curIP[(int)(ptr1 - curIP) - 1] = '\0';
				if(strcmp(curIP, "") == 0 || strcmp(curIP, " ") == 0 || strcmp(curIP, "\r\n") == 0 || strcmp(curIP, "\n") == 0 || curIP[0] == ' ' || curIP[0] == '#') 
				{
					ZeroMemory(curIPCopy, sizeof(curIPCopy));
					ZeroMemory(curIP, sizeof(curIP));
					continue;
				};

				if(strstr(curIP, "-") != NULL)
				{
					bool firstPart = true;
					int offset = 0;
					int curNIndex = 0;
					char curS;
					char curN[32] = {0};
					for(int i = 0; i < strlen(curIP); ++i)
					{
						curS = curIP[i];
						if(curS == '.') 
						{
							if(firstPart) 
							{
								starterIP[flCounter][offset] = atoi(curN);
								ipsstartfl[flCounter][offset] = atoi(curN);
							}
							else ipsendfl[flCounter][offset] = atoi(curN);
							++offset;
							curNIndex = 0;
							ZeroMemory(curN, 32);
							continue;
						}
						else if(curS == '-') 
						{
							if(firstPart) 
							{
								starterIP[flCounter][offset] = atoi(curN);
								ipsstartfl[flCounter][offset] = atoi(curN);
							}
							else ipsendfl[flCounter][offset] = atoi(curN);
							offset = 0;
							firstPart = false;
							curNIndex = 0;
							ZeroMemory(curN, 32);
							continue;
						};
						curN[curNIndex++] = curS;
						if(i == strlen(curIP) - 1)
						{
							ipsendfl[flCounter][offset] = atoi(curN);
						};
					};

					if(ipsstartfl[flCounter][0] > ipsendfl[flCounter][0]
					|| (ipsstartfl[flCounter][0] >= ipsendfl[flCounter][0] 
						&& ipsstartfl[flCounter][1] > ipsendfl[flCounter][1]
						)
					|| (ipsstartfl[flCounter][0] >= ipsendfl[flCounter][0] 
						&& ipsstartfl[flCounter][1] >= ipsendfl[flCounter][1]
						&& ipsstartfl[flCounter][2] > ipsendfl[flCounter][2]
						)
					|| (ipsstartfl[flCounter][0] >= ipsendfl[flCounter][0] 
						&& ipsstartfl[flCounter][1] >= ipsendfl[flCounter][1]
						&& ipsstartfl[flCounter][2] >= ipsendfl[flCounter][2]
						&& ipsstartfl[flCounter][3] > ipsendfl[flCounter][3]
						)
					)
					{
						char tempMsg[64] = {0};
						strcpy(tempMsg, "[IP Loader]Error in IP list. Line-> [");
						strcat(tempMsg, std::to_string((long double)flCounter).c_str());
						strcat(tempMsg, "] String-> [");
						strcat(tempMsg, curIPCopy);
						strcat(tempMsg, "]");
						stt->doEmitionRedFoundData(QString(tempMsg));
						return;
					};

					gTargets += 256*256*256*(ipsendfl[flCounter][0] - ipsstartfl[flCounter][0]);
					gTargets += 256*256*(ipsendfl[flCounter][1] - ipsstartfl[flCounter][1]);
					gTargets += 256*(ipsendfl[flCounter][2] - ipsstartfl[flCounter][2]);
					gTargets += (ipsendfl[flCounter][3] - ipsstartfl[flCounter][3]);
					++flCounter;
				}
				else if(strstr(curIP, "/") != NULL)
				{
					char *str1;
					char *str2;
					char res[8] = {0}; 

					int mask = 0;
					char *ptr1 = strstr(curIP, "/");
					GetOctets(curIP);

					mask = atoi(ptr1 + 1);
					unsigned char mOctet[4];
					mOctet[0] = 1;
					mOctet[1] = 1;
					mOctet[2] = 1;
					mOctet[3] = 1;

					if(mask >= 24)
					{
						mOctet[0] = 255;
						mOctet[1] = 255;
						mOctet[2] = 255;
						for(int i = 0; i < mask - 24 - 1; ++i)
						{
							mOctet[3] = mOctet[3] << 1;
							mOctet[3] |= 1;
						};
						mOctet[3] = mOctet[3] << 8 - (mask - 24);
					}
					else if(mask >= 16)
					{
						mOctet[0] = 255;
						mOctet[1] = 255;
						for(int i = 0; i < mask - 16 - 1; ++i)
						{
							mOctet[2] = mOctet[2] << 1;
							mOctet[2] |= 1;
						};
						mOctet[2] = mOctet[2] << 8 - (mask - 16);
						mOctet[3] = 0;
					}
					else if(mask >= 8)
					{
						mOctet[0] = 255;
						for(int i = 0; i < mask - 8 - 1; ++i)
						{
							mOctet[1] = mOctet[1] << 1;
							mOctet[1] |= 1;
						};
						mOctet[1] = mOctet[1] << 8 - (mask - 8);
						mOctet[2] = 0;
						mOctet[3] = 0;
					}
					else
					{
						for(int i = 0; i < mask - 1; ++i)
						{
							mOctet[0] = mOctet[0]<< 1;
							mOctet[0] |= 1;
						};
						mOctet[0] = mOctet[0] << 8 - mask;
						mOctet[1] = 0;
						mOctet[2] = 0;
						mOctet[3] = 0;
					};
				
					unsigned char ocRes = 0;
					for(int i = 0; i < 4; ++i)
					{
						ocRes = octet[i]&mOctet[i];
						starterIP[flCounter][i] = ocRes;
						ipsstartfl[flCounter][i] = ocRes;
						if(mOctet[i] == 255) ipsendfl[flCounter][i] = octet[i];
						else ipsendfl[flCounter][i] = octet[i]|~mOctet[i];
					};

					gTargets += pow((float)2, (32 - mask));
					++flCounter;
				}
				else if(strstr(curIP, "RESTORE_IMPORT_SESSION") != NULL)
				{
					///DUMMY///
				}
				else
				{
					stt->doEmitionRedFoundData("[IP Loader] Wrong list format. String: " + QString(curIP));
				};
				ZeroMemory(curIP, sizeof(curIP));
			};
		};
		gTargets -= 1;
		gTargetsOverall = gTargets;
		targets = gTargets;

		stt->doEmitionYellowFoundData("List loader - [OK] (" + QString::number(gTargetsOverall + 1) + " hosts)");
		fclose(fl);
	}
	else
	{
		stt->doEmitionRedFoundData("[IP Loader] Cannot open IP list.");
	};
};
char *GetCIDRRangeStr(char *str)
{
	char result[128] = {0};
	char start[32] = {0};
	char end[32] = {0};
	char buff[16] = {0};

	int mask = 0;
	char *ptr1 = strstr(str, "/");
	GetOctets(str);

	mask = atoi(ptr1 + 1);
	unsigned char mOctet[4];
	mOctet[0] = 1;
	mOctet[1] = 1;
	mOctet[2] = 1;
	mOctet[3] = 1;

	if(mask >= 24)
	{
		mOctet[0] = 255;
		mOctet[1] = 255;
		mOctet[2] = 255;
		for(int i = 0; i < mask - 24 - 1; ++i)
		{
			mOctet[3] = mOctet[3] << 1;
			mOctet[3] |= 1;
		};
		mOctet[3] = mOctet[3] << 8 - (mask - 24);
	}
	else if(mask >= 16)
	{
		mOctet[0] = 255;
		mOctet[1] = 255;
		for(int i = 0; i < mask - 16 - 1; ++i)
		{
			mOctet[2] = mOctet[2] << 1;
			mOctet[2] |= 1;
		};
		mOctet[2] = mOctet[2] << 8 - (mask - 16);
		mOctet[3] = 0;
	}
	else if(mask >= 8)
	{
		mOctet[0] = 255;
		for(int i = 0; i < mask - 8 - 1; ++i)
		{
			mOctet[1] = mOctet[1] << 1;
			mOctet[1] |= 1;
		};
		mOctet[1] = mOctet[1] << 8 - (mask - 8);
		mOctet[2] = 0;
		mOctet[3] = 0;
	}
	else
	{
		for(int i = 0; i < mask - 1; ++i)
		{
			mOctet[0] = mOctet[0]<< 1;
			mOctet[0] |= 1;
		};
		mOctet[0] = mOctet[0] << 8 - mask;
		mOctet[1] = 0;
		mOctet[2] = 0;
		mOctet[3] = 0;
	};

	strcpy(start, itoa(octet[0]&mOctet[0], buff, 10));
	strcat(start, ".");
	strcat(start, itoa(octet[1]&mOctet[1], buff, 10));
	strcat(start, ".");
	strcat(start, itoa(octet[2]&mOctet[2], buff, 10));
	strcat(start, ".");
	strcat(start, itoa(octet[3]&mOctet[3], buff, 10));
	
	unsigned char tempRes = 0;
	if(mOctet[0] == 255) tempRes = octet[0];
	else tempRes = octet[0]|~mOctet[0];
	strcat(end, itoa(tempRes, buff, 10));
	strcat(end, ".");
	if(mOctet[1] == 255) tempRes = octet[1];
	else tempRes = octet[1]|~mOctet[1];
	strcat(end, itoa(tempRes, buff, 10));
	strcat(end, ".");
	if(mOctet[2] == 255) tempRes = octet[2];
	else tempRes = octet[2]|~mOctet[2];
	strcat(end, itoa(tempRes, buff, 10));
	strcat(end, ".");
	if(mOctet[3] == 255) tempRes = octet[3];
	else tempRes = octet[3]|~mOctet[3];
	strcat(end, itoa(tempRes, buff, 10));
	
	strcpy(result, start);
	strcat(result, "-");
	strcat(result, end);

	return result;
};
int ParseArgs(int argc, char *argv[])
{
	int s = 0;
	int indexPorts = 0;
	
	if(strstr(argv[1], "-f") != NULL)
	{
		gThreads = atoi(argv[3]);
		threads = gThreads;

		FileLoader(argv[2]);
		gMode = -1;
	}
	else
	{
		gMode = atoi(argv[1]);

		if(gMode == 0) //Mode 0 (IP scan)
		{
			gThreads = atoi(argv[3]);
		}
		else if(gMode == 1) //Mode 1 (DNS scan)
		{
			strcpy(gFirstDom, argv[3]);
			gThreads = atoi(argv[4]);
		};

		threads = gThreads;
	};

	if(gMode == 0)
	{
		if(strstr(argv[2], "-") != NULL)
		{
			strcpy(gRange, argv[2]);
			strcpy(saveEndIP, argv[2]); 
			strcpy(endIP2, argv[2]); 
		}
		else
		{
			if(strstr(argv[2], "/") != NULL)
			{
				strcpy(gRange, GetCIDRRangeStr(argv[2]));
			}
			else
			{
				strcpy(gRange, argv[2]);
				strcat(gRange, "-");
				strcat(gRange, argv[2]);
			};
			strcpy(saveEndIP, gRange); 
			strcpy(endIP2, gRange); 
		};
	}
	else if(gMode == 1)
	{
		if(strstr(argv[2], "/") != NULL)
		{
			strcpy(gRange, argv[2]);
			strcpy(saveEndIP, argv[2]); 
			strcpy(endIP2, argv[2]);
		}
		else
		{
			strcpy(gRange, argv[2]);
			strcpy(saveEndIP, gRange);
			strcpy(endIP2, gRange);
		};

	};

	for(int i = 1; i <= argc - 1; i++)
	{
		s += strlen(argv[i]) + 2;
	};

	char *argString = new char [s + 1];
	ZeroMemory(argString, sizeof(argString));

	for(int i = 1; i <= argc - 1; i++)
	{
		strcat(argString, argv[i]);
		if(i != argc - 1) strcat(argString, "::");
	};

	if(strstr(argString, "-p") != NULL)
	{
		char *portString = strstr(argString, "-p");
		char p[4096] = {0};
		if(strstr(portString, "::") != NULL) 
		{
			strncpy(gPorts, portString, (int)(strstr(portString, "::") - portString));
			strncpy(p, strstr(argString, "-p") + 2, (int)(strstr(portString, "::") - portString));
		}
		else
		{
			strcpy(gPorts, portString);
			strcpy(p, strstr(argString, "-p") + 2);
		};
		char *lex;

		if(strstr(p, ",") != NULL)
		{
			
			lex = strtok(p, ",");
			portArr[indexPorts++] = atoi(lex);	

			while ((lex = strtok(NULL, ",")) != NULL) 
			{ 
				portArr[indexPorts++] = atoi(lex);
				overallPorts++;
			};
		}
		else if(strstr(p, "-") != NULL)
		{
			char *startp;
			char *endp;
			char buffForPorts[16] = {0};
			
			lex = strtok(p, "-p");
			startp = lex;
			lex = strtok(NULL, "-");
			endp = lex;

			for(int i = atoi(startp); i <= atoi(endp); i++ )
			{
				portArr[indexPorts++] = i;				
				overallPorts++;
			};

			ZeroMemory(buffForPorts, sizeof(buffForPorts));
		}
		else
		{
			
			lex = strtok(p, "-p");
			portArr[indexPorts++] = atoi(lex);
		};
	}
	else
	{
		portArr[0] = 80;
		portArr[1] = 81;
		portArr[2] = 88;
		portArr[3] = 8080;
		portArr[4] = 8081;
		portArr[5] = 60001;
		portArr[6] = 60002;
		portArr[7] = 8008;
		portArr[8] = 8888;
		portArr[9] = 554;
		portArr[10] = 9000;
		portArr[11] = 441;
		portArr[12] = 4111;
		portArr[13] = 6667;
		portArr[14] = 3536;
		portArr[15] = 22;
		portArr[16] = 21;

		overallPorts = 16;

		strcpy(gPorts, "--DEFAULT");
	};

	ZeroMemory(argString, sizeof(argString));

	delete[] argString;

return 0;
};
	
char charAll[38] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '-'};

sockstruct *st = NULL;
char iip[256] = {0};
int _getPos(char l)
{
	for(int i = 0; i < 38; ++i)
	{
		if(l == charAll[i]) return i;
	};
	return -1;
};
int _getChunkCount(char *data)
{
	int firstPos = _getPos(data[1]);
	int secondPos = _getPos(data[2]);
	return secondPos - firstPos + 1;
};
int _GetDNSFromMask(char *mask, char *saveMask, char *saveMaskEnder)
{
	if(strstr(mask, "[") != NULL)
	{
		char maskEnd[1024] = {0};
		char maskRes[1024] = {0};
		char *ptr1 = strstr(mask, "[");
		char *ptr2 = strstr(ptr1, "]");
		int sz = ptr2 - ptr1;
		char chunk[8] = {0};
		strncpy(chunk, ptr1, sz + 1);
		int firstPos = _getPos(chunk[1]);
		int secondPos = _getPos(chunk[2]);

		if(firstPos > secondPos)
		{
			stt->doEmitionRedFoundData("Error at mask (Wrong letter sequence)");
			return -1;
		};

		char maskStart[1024] = {0};
		int startPosition = strstr(mask, "[") - mask;
		strncpy(maskStart, mask, startPosition);

		char maskEntry[1024] = {0};
		if(saveMaskEnder != NULL) 
		{
			int startSavePosition = strstr(saveMaskEnder, "[") - saveMaskEnder;
			strncpy(maskEntry, saveMaskEnder, startSavePosition);
		};

		int szMask = strlen(mask);
		int szOffset = startPosition + 2;
		if(szMask != szOffset) strcpy(maskEnd, strstr(mask, "]") + 1);
		else
		{
			ZeroMemory(maskEnd, sizeof(maskEnd));
		};
		char maskSaver[128] = {0};
		if(firstPos != -1 && secondPos != -1)
		{
			for(int i = firstPos; i <= secondPos; ++i)
			{
				if(globalScanFlag == false) break;
				strcpy(maskSaver, saveMask);
				strcat(maskSaver, maskEntry);
				chunk[1] = charAll[i];
				strcat(maskSaver, chunk);
				strcpy(maskRes, maskStart);
				maskRes[startPosition] = charAll[i];
				strcat(maskRes, maskEnd);

				if(_GetDNSFromMask(maskRes, maskSaver, maskEnd) == -1) return -1;
				ZeroMemory(maskSaver, sizeof(maskSaver));	
				ZeroMemory(maskRes, sizeof(maskRes));	
			};
		};
	}
	else
	{
#pragma region DNS-SCAN
		if(globalScanFlag == false) return 0;
		strcpy(endIP2, saveMask);
		st = new sockstruct();
		ZeroMemory(st->argv, sizeof(st->argv));
		ZeroMemory(iip, sizeof(iip));
		while(cons >= gThreads) Sleep(300);
		strcpy(iip, mask);
		strcpy(saveStartIP, iip);
		strcat(iip, top_level_domain);

		++indexIP;

		strcpy(st->argv, iip);

		targetAndIPWriter(--gTargets, st->argv);

		ConInc();
#if defined(WIN32)
		if(globalScanFlag) _beginthread( (void(*)(void*))_connect, 0, st );
#else
		if(globalScanFlag)
		{
			pthread_t thrc;
			pthread_create(&thrc, NULL, (void *(*)(void*))&_connect, st );
		};
#endif
		Sleep(gThreadDelay);
#pragma endregion
	};
};

int startScan(char* args)
{	
	SSL_library_init();
	OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
	SSL_load_error_strings();   /* Bring in and register error messages */

	horLineFlag = false;
	flCounter = 0;
	PieAnomC1 = 0, PieWF = 0, PieBA = 0, PieSusp = 0, PieLowl = 0, PieSSH = 0;
	AnomC1 = 0, BA = 0, Filt = 0, Overl = 0, Lowl = 0, Alive = 0, Activity = 0, saved = 0, Susp = 0, WF = 0, offlines = 0;
	BrutingThrds = 0;
	found = 0;
	gTargets = 0;
	targets = 0;
	cons = 0;
	overallPorts = 0;
	found = 0;
	indexIP = 0;
	ZeroMemory(octet, sizeof(octet));
	ZeroMemory(ipsstart, sizeof(ipsstart));
	ZeroMemory(ipsend, sizeof(ipsend));
		
#if defined(WIN32)
	CreateDirectory(L"./result_files", NULL);
#else
	struct stat st = {0};
	if (stat("./result_files", &st) == -1) {
		mkdir("./result_files", 0700);
	}
#endif

	int argc = 0;
	char *argv[512] = {0};

	
	char *tStr = strtok(args, "|");
	while(tStr != NULL)
	{
		argv[argc++] = tStr;
		tStr = strtok(NULL, "|");
	};
	
	ParseArgs(argc, argv);
	mode = gMode;
	int resInit = fInit(gMode, gRange);
	if(resInit == -1 ) 
	{
		#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("[Error] fInit failure");
		stt->doEmitionKillSttThread();
		#pragma endregion
		return -1;
	};

stt->doEmitionIPRANGE(QString(saveEndIP));
stt->doEmitionThreads(QString::number(0) + "/" + QString::number(gThreads));

	_passLoginFapper();
	_NegativeFapper();
	
	char res[256] = {0};
	if(gMode == 0)
	{
#if defined(WIN32)
		_beginthread( (void(*)(void*))_saver, 0, NULL );
#else
		pthread_t thrs;
		pthread_create(&thrs, NULL, (void *(*)(void*))&_saver, NULL);
#endif
#if defined(WIN32)
		if(trackerOK) _beginthread( (void(*)(void*))_tracker, 0, NULL );
		Sleep(50);
		_beginthread( (void(*)(void*))_timer, 0, NULL );
#else
		pthread_t thrt;
		pthread_t thrtt;
		if(trackerOK) pthread_create(&thrt, NULL, (void *(*)(void*))&_tracker, NULL);
		pthread_create(&thrtt, NULL, (void *(*)(void*))&_timer, NULL);
#endif
		int eor0 = 0, eor1 = 0, eor2 = 0, eor3 = 0;
		stt->doEmitionChangeStatus("Scanning...");
		sockstruct *st = NULL;
		while ((eor0 == 0 || eor1 == 0 || eor2 == 0 || eor3 == 0 ) && globalScanFlag)			
		{
			if(globalScanFlag == false) break;
			while(ipsstart[0] < 256 && eor0 == 0)
			{
				if(ipsstart[0] == ipsend[0]) eor0 = 1;
				if(globalScanFlag == false) break;
				while(ipsstart[1] < 256 && eor1 == 0)
				{
					if(ipsstart[1] == ipsend[1] && eor0 == 1) eor1 = 1;
					if(globalScanFlag == false) break;
					while(ipsstart[2] < 256 && eor2 == 0)
					{
						if(ipsstart[2] == ipsend[2] && eor1 == 1) eor2 = 1;
						if(globalScanFlag == false) break;
						while(ipsstart[3] < 256 && eor3 == 0)
						{
							if(globalScanFlag == false) break;
							st = new sockstruct();
							ZeroMemory(st->argv, sizeof(st->argv));
							ZeroMemory(res, sizeof(res));
							while(cons >= gThreads) Sleep(300);
							if(ipsstart[3] == ipsend[3] && eor2 == 1) eor3 = 1;
							++indexIP;

							strcat(res, std::to_string((long double)ipsstart[0]).c_str());
							strcat(res, ".");
							strcat(res, std::to_string((long double)ipsstart[1]).c_str());
							strcat(res, ".");
							strcat(res, std::to_string((long double)ipsstart[2]).c_str());
							strcat(res, ".");
							strcat(res, std::to_string((long double)ipsstart[3]).c_str());
							
							strcpy(st->argv, res);
							strcpy(saveStartIP, res);

							targetAndIPWriter(gTargets--, st->argv);
							ConInc();
#if defined(WIN32)
							_beginthread( (void(*)(void*))_connect, 0, st );
#else
							pthread_t thrc;
							pthread_create(&thrc, NULL, (void *(*)(void*))&_connect, st);
#endif
							Sleep(gThreadDelay);
							++ipsstart[3];
						};
						ipsstart[3] = 0;
						++ipsstart[2];
					};
					ipsstart[3] = 0;
					ipsstart[2] = 0;
					++ipsstart[1];
				};
				ipsstart[3] = 0;
				ipsstart[2] = 0;
				ipsstart[1] = 0;
				++ipsstart[0];
			};
			ipsstart[3] = 0;
			ipsstart[2] = 0;
			ipsstart[1] = 0;
		};
	}
	else if(gMode == 1 )
	{
#if defined(WIN32)
		_beginthread( (void(*)(void*))_saver, 0, NULL );
#else
		pthread_t thrs;
		pthread_create(&thrs, NULL, (void *(*)(void*))&_saver, NULL);
#endif
		char iipFinish[64] = {0};

		strcpy(top_level_domain, gFirstDom);

		if(trackerOK) 
		{
#if defined(WIN32)
		if(trackerOK) _beginthread( (void(*)(void*))_tracker, 0, NULL );
#else
		pthread_t thrt;
		if(trackerOK) pthread_create(&thrt, NULL, (void *(*)(void*))&_tracker, NULL);
#endif
		};

		Sleep(40);
#if defined(WIN32)
		_beginthread( (void(*)(void*))_timer, 0, NULL );
#else
		pthread_t thrtt;
		pthread_create(&thrtt, NULL, (void *(*)(void*))&_timer, NULL);
#endif


	char dataEntry[1024] = {0};

	int innerCounter = 0;
	int sz = strlen(saveEndIP);
	for(int i = 0; i < sz; ++i)
	{
		if(saveEndIP[i] == '\\')
		{
			if(saveEndIP[i + 1] == 'd')
			{
				strcat(dataEntry, "[09]");
				++i;
				innerCounter += 4;
				continue;
			}
			else if(saveEndIP[i + 1] == 'w')
			{
				strcat(dataEntry, "[0z]");
				++i;
				innerCounter += 4;
				continue;
			}
			else if(saveEndIP[i + 1] == 'l')
			{
				strcat(dataEntry, "[az]");
				++i;
				innerCounter += 4;
				continue;
			}
			else
			{
				QString errStr = "Error at mask (Position:" + QString::number(i+1);
				errStr += ") \"";
				errStr += QString(saveEndIP).mid(0, i == 0 ? 0 : i);
				errStr += "<u>";
				errStr += QString(saveEndIP).mid(i, i == 0 ? i+2 : i+1);
				errStr += "</u>";
				errStr += QString(saveEndIP).mid(i+2, strlen(saveEndIP));
				errStr += "\"";

				stt->doEmitionRedFoundData(errStr);
				return -1;
			};
		}
		else
		{
			memset(dataEntry + innerCounter++, saveEndIP[i], 1);
		};
	};
		memset(dataEntry + innerCounter + 1, '\0', 1);

		for(int i = 0; i < sz; ++i)
		{
			if(dataEntry[i] == '[')
			{
				for(int j = i + 1; j < i + 3; ++j)
				{
					if((dataEntry[j] < '0' || dataEntry[j] > '9') 
						&& (dataEntry[j] < 'a' || dataEntry[j] > 'z')
						&& dataEntry[j] != '_' 
						&& dataEntry[j] != '-'
						)
					{
						QString errStr = "Error at mask (" + QString::number(j-1);
						errStr += ") \"";
						errStr += QString(dataEntry).mid(0, j-1);
						errStr += "<u>";
						errStr += QString(dataEntry).mid(j-1, j+1);
						errStr += "</u>";
						errStr += QString(dataEntry).mid(j+1, strlen(dataEntry));
						errStr += "\"";

						stt->doEmitionRedFoundData(errStr);
						return -1;
					};
				};
				i += 3;
			}
			else if(dataEntry[i] == ']')
			{
				QString errStr = "Error at mask (" + QString::number(i-1);
				errStr += ") \"";
				errStr += QString(dataEntry).mid(0, i-1);
				errStr += "<u>";
				errStr += QString(dataEntry).mid(i-1, i+1);
				errStr += "</u>";
				errStr += QString(dataEntry).mid(i+1, strlen(dataEntry));
				errStr += "\"";

				stt->doEmitionRedFoundData(errStr);
				return -1;

			};
		};

		unsigned long long dnsCounter = 1;
		char *dnsPtr1 = strstr(dataEntry, "[");
		while(dnsPtr1 != NULL)
		{
			dnsCounter *= _getChunkCount(dnsPtr1);
			dnsPtr1 = strstr(dnsPtr1 + 1, "[");
		};
		gTargets = dnsCounter;
		gTargetsOverall = gTargets;
		stt->doEmitionYellowFoundData("Starting DNS-scan...");
		stt->doEmitionChangeStatus("Scanning...");
		
		if(_GetDNSFromMask(dataEntry, "", dataEntry) == -1) 
		{
			stt->doEmitionRedFoundData("DNS-Mode error");
		};
	}
	else if(gMode == -1)
	{
		if(flCounter == 0) 
		{
			stt->doEmitionRedFoundData("Empty IP list.");
			OnLiner = 0;
			globalScanFlag = false;
			stt->doEmitionKillSttThread();

			return -1;
		};
#if defined(WIN32)
		_beginthread( (void(*)(void*))_saver, 0, NULL );
#else
		pthread_t thrs;
		pthread_create(&thrs, NULL, (void *(*)(void*))&_saver, NULL);
#endif
		if(trackerOK) 
		{
#if defined(WIN32)
		_beginthread( (void(*)(void*))_tracker, 0, NULL );
#else
		pthread_t thrt;
		pthread_create(&thrt, NULL, (void *(*)(void*))&_tracker, NULL);
#endif
		};

		Sleep(40);
#if defined(WIN32)
		_beginthread( (void(*)(void*))_timer, 0, NULL );
#else
		pthread_t thrtt;
		pthread_create(&thrtt, NULL, (void *(*)(void*))&_timer, NULL);
#endif

		int eor0 = 0, eor1 = 0, eor2 = 0, eor3 = 0;
		stt->doEmitionChangeStatus("Scanning...");
		sockstruct *st = NULL;
		for(gC = 0; gC < flCounter; ++gC)
		{
			strcpy(metaRange, std::to_string((long double)ipsstartfl[gC][0]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string((long double)ipsstartfl[gC][1]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string((long double)ipsstartfl[gC][2]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string((long double)ipsstartfl[gC][3]).c_str());
			strcat(metaRange, "-");
			strcat(metaRange, std::to_string((long double)ipsendfl[gC][0]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string((long double)ipsendfl[gC][1]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string((long double)ipsendfl[gC][2]).c_str());
			strcat(metaRange, ".");
			strcat(metaRange, std::to_string((long double)ipsendfl[gC][3]).c_str());
			
			eor0 = 0, eor1 = 0, eor2 = 0, eor3 = 0;
			while ((eor0 == 0 || eor1 == 0 || eor2 == 0 || eor3 == 0) && globalScanFlag)			
			{
				if(globalScanFlag == false) break;
				while(ipsstartfl[gC][0] < 256 && eor0 == 0)
				{
					if(globalScanFlag == false) break;
					if(ipsstartfl[gC][0] == ipsendfl[gC][0]) eor0 = 1;
					while(ipsstartfl[gC][1] < 256 && eor1 == 0)
					{
						if(globalScanFlag == false) break;
						if(ipsstartfl[gC][1] == ipsendfl[gC][1] && eor0 == 1) eor1 = 1;
						while(ipsstartfl[gC][2] < 256 && eor2 == 0)
						{
							if(globalScanFlag == false) break;
							if(ipsstartfl[gC][2] == ipsendfl[gC][2] && eor1 == 1) eor2 = 1;
							while(ipsstartfl[gC][3] < 256 && eor3 == 0)
							{
								if(globalScanFlag == false) break;
								st = new sockstruct();
								ZeroMemory(st->argv, sizeof(st->argv));
								ZeroMemory(res, sizeof(res));

								while(cons >= gThreads) Sleep(300);

								if(ipsstartfl[gC][3] == ipsendfl[gC][3] && eor2 == 1) eor3 = 1;

								++indexIP;

								strcat(res, std::to_string((long double)ipsstartfl[gC][0]).c_str());
								strcat(res, ".");
								strcat(res, std::to_string((long double)ipsstartfl[gC][1]).c_str());
								strcat(res, ".");
								strcat(res, std::to_string((long double)ipsstartfl[gC][2]).c_str());
								strcat(res, ".");
								strcat(res, std::to_string((long double)ipsstartfl[gC][3]).c_str());
								
								strcpy(st->argv, res);
								strcpy(saveStartIP, res);

								targetAndIPWriter(gTargets--, st->argv);

								ConInc();
#if defined(WIN32)
								_beginthread( (void(*)(void*))_connect, 0, st );
#else
								pthread_t thrc;
								pthread_create(&thrc, NULL, (void *(*)(void*))&_connect, st );
#endif
								Sleep(gThreadDelay);
								if(ipsstartfl[gC][3] == 255) break;
								if(ipsstartfl[gC][3] <= ipsendfl[gC][3]) ++ipsstartfl[gC][3];
							};
							ipsstartfl[gC][3] = 0;
							if(ipsstartfl[gC][2] == 255) break;
							if(ipsstartfl[gC][2] <= ipsendfl[gC][2]) ++ipsstartfl[gC][2];
						};
						ipsstartfl[gC][3] = 0;
						ipsstartfl[gC][2] = 0;
						if(ipsstartfl[gC][1] == 255) break;
						if(ipsstartfl[gC][1] <= ipsendfl[gC][1]) ++ipsstartfl[gC][1];
					};	
					ipsstartfl[gC][3] = 0;
					ipsstartfl[gC][2] = 0;
					ipsstartfl[gC][1] = 0;
					if(ipsstartfl[gC][0] == 255) break;
					if(ipsstartfl[gC][0] <= ipsendfl[gC][0]) ++ipsstartfl[gC][0];
				};
				ipsstartfl[gC][3] = 0;
				ipsstartfl[gC][2] = 0;
				ipsstartfl[gC][1] = 0;
			};
		};
	}
	else
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("Wrong parameters.");
#pragma endregion
	};

	Sleep(gTimeOut + 1);
	#pragma region QTGUI_Area
	stt->doEmitionYellowFoundData("Stopping threads...");
	stt->doEmitionChangeStatus("Stopping...");
	#pragma endregion
	
	while(cons > 0 || OnLiner == 1 || jsonArr->size() > 0) Sleep(2000);

	if(loginLst != NULL)
	{
		for(int i = 0; i < MaxLogin; ++i) delete []loginLst[i];
		delete []loginLst;
		loginLst = NULL;
	};
	if(passLst != NULL)
	{
		for(int i = 0; i < MaxPass; ++i) delete []passLst[i];
		delete []passLst;
		passLst = NULL;
	};
	if(GlobalNegatives != NULL)
	{
		char temp[512] = {0};
		for(int i = 0; i < GlobalNegativeSize; ++i) 
		{
			delete []GlobalNegatives[i];
		};
		delete []GlobalNegatives;
		GlobalNegatives = NULL;
	};
	if(wfPassLst != NULL)
	{
		for(int i = 0; i < MaxWFPass; ++i) delete []wfPassLst[i];
		delete []wfPassLst;
		wfPassLst = NULL;
	};
	if(wfLoginLst != NULL)
	{
		for(int i = 0; i < MaxWFLogin; ++i) delete []wfLoginLst[i];
		delete []wfLoginLst;
		wfLoginLst = NULL;
	};
	if(sshlpLst != NULL)
	{
		for(int i = 0; i < MaxSSHPass; ++i) delete []sshlpLst[i];
		delete []sshlpLst;
		sshlpLst = NULL;
	};
	if(ipsstartfl != NULL)
	{
		for(int i = 0; i < MaxSSHPass; ++i) delete []ipsstartfl[i];
		delete []ipsstartfl;
		ipsstartfl = NULL;
	};
	if(ipsendfl != NULL)
	{
		for(int i = 0; i < MaxSSHPass; ++i) delete []ipsendfl[i];
		delete []ipsendfl;
		ipsendfl = NULL;
	};
	if(starterIP != NULL)
	{
		for(int i = 0; i < MaxSSHPass; ++i) delete []starterIP[i];
		delete []starterIP;
		starterIP = NULL;
	};
	#pragma region QTGUI_Area
	stt->doEmitionGreenFoundData("Done. Saved: " + QString::number(saved) + "; Alive: " + QString::number(found) + ".");
	stt->doEmitionChangeParsed(QString::number(saved) + "/" + QString::number(found));
	stt->doEmitionChangeStatus("Idle");
	stt->doEmitionKillSttThread();
	#pragma endregion
};
