// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

// Windows ͷ�ļ�:
#include <windows.h>
#include <shlobj.h>
#include <TlHelp32.h>
#include <shellapi.h>
#pragma comment(lib,"shell32.lib")
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <direct.h>
#include <restartmanager.h>
#pragma comment(lib, "rstrtmgr.lib")
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#include <cctype>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

// C ����ʱͷ�ļ�
#include <tchar.h>
#include <stdint.h>

// C++ ����ʱͷ�ļ�
#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#define ENABLE_LIBZIP 0

// ��������
#include "miniz.h"

#if ENABLE_LIBZIP
#include <libzip/zip.h>
#ifdef _DEBUG
#pragma comment(lib, "lib/libzip/zipd.lib")
#pragma comment(lib, "lib/zlib/zlibstaticd.lib")
#else
#pragma comment(lib, "lib/libzip/zip.lib")
#pragma comment(lib, "lib/zlib/zlibstatic.lib")
#endif // _DEBUG
#endif // ENABLE_LIBZIP

#define CURL_STATICLIB
#include <curl/curl.h>
#pragma comment(lib, "crypt32.lib")
#ifdef _DEBUG
#pragma comment(lib, "lib/curl/libcurl-d.lib")
#else
#pragma comment(lib, "lib/curl/libcurl.lib")
#endif // _DEBUG

#include "sha256.h"
#define nullptr NULL

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
