#include "stdafx.h"
#include "utility.h"

extern void LogInfo(const std::string& str);
extern void LogInfo(const char* fmt, ...);
extern void LogWarn(const std::string& str);
extern void LogWarn(const char* fmt, ...);
extern void LogError(const std::string& str);
extern void LogError(const char* fmt, ...);
extern void ChangeStatusText(const char* fmt, ...);
extern HWND hWnd;
extern BOOL bIsUpgradeFinish;
extern UINT uInstallProgress;

//������ѡ��
static const char* cmdline_option_arr[] = {
	"-old_version_process_id",//1
	"-old_version_process_name",//2
	"-source_path",//3
	"-destination_path",//4
	"-is_create_shortcut",//5
	"-is_recursion_replace_shortcut",//6
	"-is_delete_update_file",//7
	"-start_process_command_line",//8
	"-is_hide_window",//9
	"-is_show_topmost",//10
	"-is_backup_file",//11
	"-window_title",//12
	"-window_width",//13
	"-window_height",//14
	"-is_compress_backup_file",//15
	"-sensitive_dll",//16
	"-latest_version",//17
	"-is_only_start_process",//18
	"-start_process_file_path",//19
	"-start_process_working_directory",//20
	"-is_prompt_if_error",//21
	"-server_address",//22
	"-server_port",//23
	"-upload_update_record_flag",//24
	"-window_style",//25
	"-is_delete_old_version_backup_file",//26
	"-is_wait_old_version_exit",//27
	"-wait_old_version_exit_timeout",//28
	"-kill_old_version_process_flag",//29
	"-upload_update_log_max_size",//30
	nullptr//N - 1
};

//������ѡ��ע��
static const char* cmdline_option_comment_arr[] = {
	"�ɰ汾����ID",//1
	"�ɰ汾������",//2
	"Դ·�������µ��ļ���·����",//3
	"Ŀ��·������Ҫ���µ��ļ���·����",//4
	"�Ƿ񴴽���ݷ�ʽ",//5
	"�Ƿ�ݹ��滻��ݷ�ʽ�������������棬�滻���п�ݷ�ʽ��",//6
	"�Ƿ�ɾ�������ļ�",//7
	"�������������в���",//8
	"�Ƿ����ش���",//9
	"�Ƿ񽫴����ö�",//10
	"�Ƿ񱸷��ļ�",//11
	"���ڱ��⣨��������ʽ0ʱ��Ч��",//12
	"���ڿ�ȣ���������ʽ0ʱ��Ч��",//13
	"���ڸ߶ȣ���������ʽ0ʱ��Ч��",//14
	"�Ƿ�ѹ�������ļ����������ļ�ʱ��Ч��",//15
	"���еĶ�̬���ӿ⣨�����ƹ�ɱ��������أ���ʽ��a.dll;b.dll����",//16
	"���°汾�ţ������ļ�ʱ��ʹ�õ���",//17
	"�Ƿ�ֻ�������̣����Ϊtrue�����ڽ��ᱻǿ�����أ�",//18
	"���������ļ�·�������ڿ���ֻ��������ʱ��Ч��",//19
	"�������̹���·�������ڿ���ֻ��������ʱ��Ч��",//20
	"��������Ƿ񵯴���ʾ���������ش��ںʹ�����ʽΪ1ʱ��Ч��",//21
	"��������ַ�������ϴ����¼�¼��",//22
	"�������˿ڣ������ϴ����¼�¼��",//23
	"�ϴ����¼�¼��־��0�����ϴ���1��������������־��2�����ʧ�ܰ���������־��3������������־��",//24
	"������ʽ��0�����洰����ʽ��1��������ʽ��",//25
	"�Ƿ�ɾ���ɰ汾�����ļ�",//26
	"�Ƿ�ȴ��ɰ汾�˳�",//27
	"�ȴ��ɰ汾�˳���ʱ",//28
	"ɱ���ɰ汾���̱�־��0�����ݽ���ID���йرգ�1�����ݽ��������йرգ�",//29
	"�ϴ�������־����С����λ��MB��Ĭ��5MB��",//30
	nullptr//N - 1
};

//�������˵��ļ���׺
//static const char* often_filter_file_suffix[] = {
//	".log", //1
//	".txt", //2
//	".csv", //3
//	".tmp", //4
//	".dump", //5
//	".cache", //6
//	".bak", //7
//	".jpg", //8
//	".jpeg", //9
//	".bmp", //10
//	".png", //11
//	".gif", //12
//	".tiff", //13
//	".svg", //14
//	".ico", //15
//	".raw", //16
//	".pcx", //17
//	".ppm", //18
//	".psd", //19
//	".mp3", //20
//	".wav", //21
//	".mp4", //22
//	".avi", //23
//	nullptr //N -1
//};

//�������ص�dll����
static const char* often_intercepted_dll_name[] = {
	//INVO
	"invo_mes_plugin.dll",
	"invo_mes_plugin_x64.dll",
	"libcrypto-3.dll",
	"libssl-3.dll",
	"libqui.dll",
	//QT
	"qsvgicon.dll",
	"qgif.dll",
	"qicns.dll",
	"qico.dll",
	"qjpeg.dll",
	"qsvg.dll",
	"qtga.dll",
	"qtiff.dll",
	"qwbmp.dll",
	"qwebp.dll",
	"qwindows.dll",
	"qwindowsvistastyle.dll",
	"Qt5Core.dll",
	"Qt5Gui.dll",
	"Qt5Svg.dll",
	"Qt5Widgets.dll",
	"vcruntime140.dll",
	"libGLESv2.dll",
	"libEGL.dll",
	"opengl32sw.dll",
	"D3Dcompiler_47.dll",
	"ucrtbase.dll",
	//MSVC
	"api-ms-win-core-console-l1-1-0.dll",
	"api-ms-win-core-datetime-l1-1-0.dll",
	"api-ms-win-core-debug-l1-1-0.dll",
	"api-ms-win-core-errorhandling-l1-1-0.dll",
	"api-ms-win-core-file-l1-1-0.dll",
	"api-ms-win-core-file-l1-2-0.dll",
	"api-ms-win-core-file-l2-1-0.dll",
	"api-ms-win-core-handle-l1-1-0.dll",
	"api-ms-win-core-heap-l1-1-0.dll",
	"api-ms-win-core-heap-l2-1-0.dll",
	"api-ms-win-core-interlocked-l1-1-0.dll",
	"api-ms-win-core-libraryloader-l1-1-0.dll",
	"api-ms-win-core-libraryloader-l1-2-0.dll",
	"api-ms-win-core-localization-l1-2-0.dll",
	"api-ms-win-core-memory-l1-1-0.dll",
	"api-ms-win-core-namedpipe-l1-1-0.dll",
	"api-ms-win-core-processenvironment-l1-1-0.dll",
	"api-ms-win-core-processthreads-l1-1-0.dll",
	"api-ms-win-core-processthreads-l1-1-1.dll",
	"api-ms-win-core-processtopology-obsolete-l1-1-0.dll",
	"api-ms-win-core-profile-l1-1-0.dll",
	"api-ms-win-core-rtlsupport-l1-1-0.dll",
	"api-ms-win-core-shlwapi-legacy-l1-1-0.dll",
	"api-ms-win-core-string-l1-1-0.dll",
	"api-ms-win-core-synch-l1-1-0.dll",
	"api-ms-win-core-synch-l1-2-0.dll",
	"api-ms-win-core-sysinfo-l1-1-0.dll",
	"api-ms-win-core-timezone-l1-1-0.dll",
	"api-ms-win-core-util-l1-1-0.dll",
	"api-ms-win-crt-conio-l1-1-0.dll",
	"api-ms-win-crt-convert-l1-1-0.dll",
	"api-ms-win-crt-environment-l1-1-0.dll",
	"api-ms-win-crt-filesystem-l1-1-0.dll",
	"api-ms-win-crt-heap-l1-1-0.dll",
	"api-ms-win-crt-locale-l1-1-0.dll",
	"api-ms-win-crt-math-l1-1-0.dll",
	"api-ms-win-crt-multibyte-l1-1-0.dll",
	"api-ms-win-crt-runtime-l1-1-0.dll",
	"api-ms-win-crt-stdio-l1-1-0.dll",
	"api-ms-win-crt-string-l1-1-0.dll",
	"api-ms-win-crt-time-l1-1-0.dll",
	"api-ms-win-crt-utility-l1-1-0.dll",
	nullptr
};

//#define MAX_UPLOAD_UPDATE_LOG_SIZE (1024 * 1024 * 5)

std::string last_error;
static FILE* log_fileptr = nullptr;
static std::string log_filepath;
static std::string log_filetext;

_cmdline_option::_cmdline_option() {
	is_create_shortcut = true;
	is_recursion_replace_shortcut = false;
	is_delete_update_file = true;
	is_hide_window = false;
	is_show_topmost = false;
	is_backup_file = true;
	is_compress_backup_file = false;
	is_only_start_process = false;
	is_prompt_if_error = true;

	old_version_process_id = 0;
	window_width = 500;
	window_height = 350;

	latest_version = "0.0.0.0";
	server_address = "www.invomes.com";
	server_port = 60682;
	upload_update_record_flag = 1;
	window_style = 1;
	is_delete_old_version_backup_file = false;
	is_wait_old_version_exit = false;
	wait_old_version_exit_timeout = 3000;
	kill_old_version_process_flag = 0;
	upload_update_log_max_size = 5;
}

_cmdline_option::~_cmdline_option() {

}

cmdline_option_t cmdline_option;
std::set<std::string> sensitive_dll_set;

static bool replace_with_windows_path_symbol_cb(char ch)
{
	return ch == '/';
}

void replace_with_windows_path_symbol(std::string& filepath)
{
	std::replace_if(filepath.begin(), filepath.end(), replace_with_windows_path_symbol_cb, '\\');
}

static bool replace_with_linux_path_symbol_cb(char ch) {
	return ch == '\\';
}

void replace_with_linux_path_symbol(std::string& filepath)
{
	std::replace_if(filepath.begin(), filepath.end(), replace_with_linux_path_symbol_cb, '/');
}

std::string format_error(uint32_t error_code) 
{
	const int bufferSize = 8192; // ���Ը�����Ҫ������������С
	std::vector<char> buffer(bufferSize);

	// FormatMessage() �������ڸ�ʽ��һ����Ϣ�ַ���
	// FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ����ָ����ϵͳ��Ϣ���л�ȡ�ַ����������Բ�������
	DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	if (FormatMessage(flags, nullptr, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		&buffer.front(), bufferSize, nullptr) == 0) {
			// ��� FormatMessage() ʧ�ܣ��򷵻�Ĭ�ϵĴ�����Ϣ
			return "Unknown error";
	}

	// ȥ����β�Ļ��з�
	if (!buffer.empty() && buffer.back() == '\n') {
		buffer.pop_back();
	}

	if (!buffer.empty() && buffer.back() == '\r') {
		buffer.pop_back();
	}

	return std::string(&buffer.front());
}

void trim_str(std::string& str) {
	if (str.empty()) return;
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ') + 1);
}

std::list<std::string>::iterator find_str_list(std::list<std::string>& ls, const std::string& str, bool case_insensitive)
{
	std::list<std::string>::iterator iter = ls.end();
	for (iter = ls.begin(); iter != ls.end(); ++iter) {
		if (case_insensitive) {
			if (!_stricmp((*iter).c_str(), str.c_str())) {
				break;
			}
		}
		else {
			if (!strcmp((*iter).c_str(), str.c_str())) {
				break;
			}
		}
	}
	return iter;
}

std::set<std::string>::iterator find_str_set(std::set<std::string>& ss, const std::string& str, bool case_insensitive)
{
	std::set<std::string>::iterator iter = ss.end();
	for (iter = ss.begin(); iter != ss.end(); ++iter) {
		if (case_insensitive) {
			if (!_stricmp((*iter).c_str(), str.c_str())) {
				break;
			}
		}
		else {
			if (!strcmp((*iter).c_str(), str.c_str())) {
				break;
			}
		}
	}
	return iter;
}

std::vector<std::string>::iterator find_str_vector(std::vector<std::string>& sv, const std::string& str, bool case_insensitive)
{
	std::vector<std::string>::iterator iter = sv.end();
	for (iter = sv.begin(); iter != sv.end(); ++iter) {
		if (case_insensitive) {
			if (!_stricmp((*iter).c_str(), str.c_str())) {
				break;
			}
		}
		else {
			if (!strcmp((*iter).c_str(), str.c_str())) {
				break;
			}
		}
	}
	return iter;
}

time_t get_time_stamp_s()
{
	return time(nullptr);
}

time_t get_time_stamp_ms()
{
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	return time_t(nullptr) * 1000 + st.wMilliseconds;
}

std::vector<std::string> get_cmdline_option_arr()
{
	std::vector<std::string> cmdline_option_list;
	for (int i = 0; i < sizeof(cmdline_option_arr) / sizeof(*cmdline_option_arr); ++i) {
		if (cmdline_option_arr[i]) {
			cmdline_option_list.push_back(cmdline_option_arr[i]);
		}
	}
	return cmdline_option_list;
}

std::vector<std::string> get_cmdline_option_comment_arr()
{
	std::vector<std::string> cmdline_option_comment_list;
	for (int i = 0; i < sizeof(cmdline_option_comment_arr) / sizeof(*cmdline_option_comment_arr); ++i) {
		if (cmdline_option_comment_arr[i]) {
			cmdline_option_comment_list.push_back(cmdline_option_comment_arr[i]);
		}
	}
	return cmdline_option_comment_list;
}

std::vector<std::string> get_often_intercepted_dll_name()
{
	std::vector<std::string> result;
	for (int i = 0; often_intercepted_dll_name[i]; ++i) {
		result.push_back(often_intercepted_dll_name[i]);
	}
	return result;
}

void open_log_file()
{
	if (cmdline_option.is_only_start_process) {
		return;
	}

	std::string dirpath = get_current_path() + "\\install_log";
	if (_access(dirpath.c_str(), 0) != 0) {
		_mkdir(dirpath.c_str());
	}

	char filename[256]= { 0 };
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(filename, "%04d%02d%02d%02d%02d%02d.log", 
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	std::string filepath = dirpath + "\\" + filename;
	log_fileptr = fopen(filepath.c_str(), "wb");
}

void write_log(const char* fmt, ...)
{
	if (!log_fileptr) {
		return;
	}

	va_list ap;
	va_start(ap, fmt);
	int len = _vscprintf_p(fmt, ap) + 1;
	char* buf = new char[len];
	_vsprintf_p(buf, len, fmt, ap);
	va_end(ap);
	std::string str(buf);
	delete[] buf;

	char temp[256] = { 0 };
	SYSTEMTIME st= { 0 };
	GetLocalTime(&st);
	sprintf_s(temp, "%04d-%02d-%02d %02d:%02d:%02d.%03d ",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	str.insert(0, temp);
	fprintf(log_fileptr, "%s\n", str.c_str());
	fflush(log_fileptr);

	//"�ϴ����¼�¼��־��0�����ϴ���1��������������־��2�����ʧ�ܰ���������־��3������������־��",
	if (cmdline_option.upload_update_record_flag > 1) {
		if (log_filetext.size() < (size_t)cmdline_option.upload_update_log_max_size) {
			replace_with_linux_path_symbol(str);
			log_filetext.append(str).append("\n");
		}
	}
}

void write_log_type(const char* type, const std::string& str)
{
	if (!log_fileptr) {
		return;
	}

	char temp[256] = { 0 };
	SYSTEMTIME st= { 0 };
	GetLocalTime(&st);
	sprintf_s(temp, "%04d-%02d-%02d %02d:%02d:%02d.%03d [%s] ",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,
		st.wSecond, st.wMilliseconds, type);

	std::string buf(temp);
	buf.append(str);
	fprintf(log_fileptr, "%s\n", buf.c_str());
	fflush(log_fileptr);

	//"�ϴ����¼�¼��־��0�����ϴ���1��������������־��2�����ʧ�ܰ���������־��3������������־��",
	if (cmdline_option.upload_update_record_flag > 1) {
		if (log_filetext.size() < (size_t)cmdline_option.upload_update_log_max_size) {
			replace_with_linux_path_symbol(buf);
			log_filetext.append(buf).append("\n");
		}
	}
}

void write_log_type_fmt(const char* type, const char* fmt, ...)
{
	if (!log_fileptr) {
		return;
	}

	va_list ap;
	va_start(ap, fmt);
	int len = _vscprintf_p(fmt, ap) + 1;
	char* buf = new char[len];
	_vsprintf_p(buf, len, fmt, ap);
	va_end(ap);
	std::string str(buf);
	delete[] buf;

	char temp[256] = { 0 };
	SYSTEMTIME st= { 0 };
	GetLocalTime(&st);
	sprintf_s(temp, "%04d-%02d-%02d %02d:%02d:%02d.%03d [%s] ",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, type);
	str.insert(0, temp);
	fprintf(log_fileptr, "%s\n", str.c_str());
	fflush(log_fileptr);

	//"�ϴ����¼�¼��־��0�����ϴ���1��������������־��2�����ʧ�ܰ���������־��3������������־��",
	if (cmdline_option.upload_update_record_flag > 1) {
		if (log_filetext.size() < (size_t)cmdline_option.upload_update_log_max_size) {
			replace_with_linux_path_symbol(str);
			log_filetext.append(str).append("\n");
		}
	}
}

void close_log_file()
{
	if (log_fileptr) {
		fclose(log_fileptr);
	}
}

std::string format_str(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int len = _vscprintf_p(fmt, ap) + 1;
	char* buf = new char[len];
	_vsprintf_p(buf, len, fmt, ap);
	va_end(ap);
	std::string str(buf);
	delete[] buf;
	return str;
}

void set_last_error(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int len = _vscprintf_p(fmt, ap) + 1;
	char* buf = new char[len];
	_vsprintf_p(buf, len, fmt, ap);
	va_end(ap);
	last_error = buf;
	delete[] buf;
}

void set_last_error(const std::string& str)
{
	last_error = str;
}

std::string get_last_error()
{
	return last_error;
}

char* wideCharToMultiByte(const wchar_t* str, uint32_t code_page)
{
	int size = WideCharToMultiByte(code_page, 0, str, -1, NULL, 0, NULL, FALSE);
	char* buffer = new char[size];
	WideCharToMultiByte(code_page, 0, str, -1, buffer, size, NULL, FALSE);
	return buffer;
}

wchar_t* multiByteToWideChar(const char* str, uint32_t code_page)
{
	int size = MultiByteToWideChar(code_page, 0, str, -1, NULL, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(code_page, 0, str, -1, buffer, size);
	return buffer;
}

std::string utf8_to_ansi(const std::string& utf8)
{
	std::string ret;
	if (utf8.empty()) {
		return ret;
	}

	wchar_t* wstr = multiByteToWideChar(utf8.c_str(), CP_UTF8);
	char* str = wideCharToMultiByte(wstr, CP_ACP);
	ret = str;
	delete[] wstr;
	delete[] str;
	return ret;
}

std::string ansi_to_utf8(const std::string& ansi)
{
	std::string ret;
	if (ansi.empty()) {
		return ret;
	}

	wchar_t* wstr = multiByteToWideChar(ansi.c_str(), CP_ACP);
	char* str = wideCharToMultiByte(wstr, CP_UTF8);
	ret = str;
	delete[] wstr;
	delete[] str;
	return ret;
}

bool is_all_digits(const char *str) {
	while (*str) {
		if (!isdigit((unsigned char)*str)) {
			return false;
		}
		str++;
	}
	return true;
}

void traversal_directory(const std::string& path, std::list<std::string>& files, bool recursion,
						filter_file_cb_t filter_file_cb, void* args)
{
	HANDLE hFind;
	WIN32_FIND_DATA ffd;
	std::string fullPath = path + "\\*";

	hFind = FindFirstFile(fullPath.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// ����"."��".."
			if (strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0)
			{
				std::string newPath = path + "\\" + ffd.cFileName;

				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					// �����Ŀ¼��������Ҫ������Ŀ¼����ݹ����
					files.push_back(newPath);
					if (recursion) {
						traversal_directory(newPath, files, recursion, filter_file_cb, args);
					}
				}
				else {
					if (filter_file_cb) {
						if (!filter_file_cb(newPath, args)) {
							files.push_back(newPath);
						}
					}
					else{
						files.push_back(newPath);
					}
				}
			}
		} while (FindNextFile(hFind, &ffd));

		FindClose(hFind);
	}
}

//���� return true
//������ return false
static bool filter_file_suffix_cb(const std::string& filepath, void* args)
{
	std::list<std::string>* filter_file = static_cast<std::list<std::string>*>(args);
	if (filter_file->empty()) {
		return false;
	}

	std::string suffix_name;
	size_t npos = filepath.rfind(".");
	if (npos == std::string::npos) {
		return false;
	}

	suffix_name = filepath.substr(npos);

	for (std::list<std::string>::iterator iter = filter_file->begin(); iter != filter_file->end(); ++iter) {
		if (!_stricmp((*iter).c_str(), suffix_name.c_str())) {
			return true;
		}
	}
	return false;
}

static std::string get_filename_by_filepath(const std::string& filepath, bool is_src_path) {
	std::string ret;
	if (is_src_path) {
		size_t npos = filepath.find_first_of(cmdline_option.source_path);
		if (npos != std::string::npos) {
			ret = filepath.substr(npos + cmdline_option.source_path.length() + 1);
		}
	}
	else {
		size_t npos = filepath.find_first_of(cmdline_option.destination_path);
		if (npos != std::string::npos) {
			ret = filepath.substr(npos + cmdline_option.destination_path.length() + 1);
		}
	}
	return ret;
}

static bool filter_file_name_cb(const std::string& filepath, void* args)
{
	std::list<std::string>* filter_file = static_cast<std::list<std::string>*>(args);
	if (filter_file->empty()) {
		return !false;
	}
	std::string fn0 = get_filename_by_filepath(filepath, false);
	for (std::list<std::string>::iterator iter = filter_file->begin(); iter != filter_file->end(); ++iter) {
		std::string fn1 = get_filename_by_filepath(*iter, true);
		if (_stricmp(fn0.c_str(), fn1.c_str()) == 0) {
			//printf("copy %s->%s\n", fn0.c_str(), fn1.c_str());
			return !true;
		}
		//printf("not copy %s->%s\n", fn0.c_str(), fn1.c_str());
	}
	return !false;
}

int miniz_compress_folder(const std::string& folder_path, const std::string& zip_filepath, 
	const std::list<std::string>& filter_file_name, progress_cb_t progress_cb, void* args) {
	if (_access(folder_path.c_str(), 0) == -1) {
		LogError("ѹ���ļ���%s·����Ч��", folder_path.c_str());
		return -1;
	}

	mz_zip_archive zip_archive = { 0 };
	std::string filepath = ansi_to_utf8(zip_filepath);
	if (!mz_zip_writer_init_file(&zip_archive, filepath.c_str(), 0)) {
		LogError("zip�ļ�%s·����Ч��", filepath.c_str());
		return -2;
	}

	std::list<std::string> files;
	get_folder_files(folder_path, files, true, &filter_file_name_cb, (void*)&filter_file_name);

	int error_code = 0;
	for (std::list<std::string>::iterator iter = files.begin(); iter != files.end(); ++iter) {
		std::string filepath = *iter;
		do {
			if (progress_cb) {
				progress_cb(filepath, args);
			}
			
			//�����־
			/*{
				size_t npos = filepath.rfind("\\");
				if (npos != std::string::npos) {
					filename = filepath.substr(npos + 1);
					LogInfo("����ѹ��%s��", filename.c_str());
				}
			}*/
			LogInfo("����ѹ��%s��", filepath.c_str());
			std::string relpath = filepath.substr(folder_path.length() + 1);
			relpath = ansi_to_utf8(relpath);

			FILE* file = nullptr;
			errno_t err =  fopen_s(&file, filepath.c_str(), "rb");
			if (!file) {
				mz_zip_writer_end(&zip_archive);
				LogError("��ȡѹ���ļ�%sʧ�ܣ�����ԭ��%s��", filepath.c_str(), strerror(err));
				error_code = -3;
				break;
			}

			_fseeki64(file, 0, SEEK_END);
			long long file_size = _ftelli64(file);
			_fseeki64(file, 0, SEEK_SET);
			//����1GB��ѹ��
			if (file_size > 1024 * 1024 * 1024) {
				fclose(file);
				continue;
			}

			void* file_data = malloc(static_cast<size_t>(file_size));
			if (!file_data) {
				fclose(file);
				mz_zip_writer_end(&zip_archive);
				LogError("�ڴ治�㣬�޷�����ѹ���ļ�%s��", filepath.c_str());
				error_code = -4;
				break;
			}
			fread(file_data, 1, static_cast<size_t>(file_size), file);
			fclose(file);

			mz_bool add_result = mz_zip_writer_add_mem(&zip_archive, relpath.c_str(), file_data,
				static_cast<size_t>(file_size), MZ_BEST_COMPRESSION);
			free(file_data);
			if (!add_result) {
				mz_zip_writer_end(&zip_archive);
				LogError("��ѹ���ļ�%s�ϲ����ڴ�ʧ�ܡ�", filepath.c_str());
				error_code = -5;
				break;
			}
		} while (false);

		//�����־
		/*{
			if (!filename.empty()) {
				LogInfo("ѹ��%s%s��", filename.c_str(), (error_code == 0) ? "�ɹ�" : "ʧ��");
			}
		}*/
		//LogInfo("ѹ��%s%s��", filepath.c_str(), (error_code == 0) ? "�ɹ�" : "ʧ��");
		if (error_code == 0) {
			LogInfo("ѹ��%s�ɹ���", filepath.c_str());
		}
		else {
			LogError("ѹ��%sʧ�ܣ�������룺%d��", filepath.c_str(), error_code);
		}

		if (error_code != 0) {
			break;
		}
	}

	mz_zip_writer_finalize_archive(&zip_archive);
	mz_zip_writer_end(&zip_archive);
	return error_code;
}


#if ENABLE_LIBZIP
/*
* @retval 0 �ɹ�
* @retval -1 �ļ�·����Ч
* @retval -2 ��zip�ļ�ʧ��
* @retval -3 ����zip�ļ�Դʧ��
* @retval -4 ���zipĿ¼Դʧ��
* @retval -5 ���zip�ļ�Դʧ��
* @retval -6 �����ļ�ѹ������ʧ��
* @retval -7 �����ļ�ʧ��
* @retval -8 �����ļ�����ʧ��
* @retval -9 �ر�zip�ļ�ʧ��
*/

static std::string get_libzip_compress_error(int error_code)
{
	std::string str;
	switch (error_code) {
	case 0:
		str = "�ɹ�";
		break;
	case -1:
		str = "�ļ�·����Ч";
		break;
	case -2:
		str = "��zip�ļ�ʧ��";
		break;
	case -3:
		str = "����zip�ļ�Դʧ��";
		break;
	case -4:
		str = "���zipĿ¼Դʧ��";
		break;
	case -5:
		str = "���zip�ļ�Դʧ��";
		break;
	case -6:
		str = "�����ļ�ѹ������ʧ��";
		break;
	case -7:
		str = "�����ļ�ʧ��";
		break;
	case -8:
		str = "�����ļ�����ʧ��";
		break;
	case -9:
		str = "�ر�zip�ļ�ʧ��";
		break;
	default:
		str = "δ֪����";
		break;
	}

	return str;
}


int libzip_compress(const std::string& compress_path, 
	const std::string& zip_path, 
	const std::string& password,
	const std::list<std::string>& filter_file_suffix,
	progress_cb_t progress_cb, 
	void* args) {
	int result = 0;
	zip_t* archive = nullptr;
	do
	{
		DWORD attrs = GetFileAttributes(compress_path.c_str());
		if (attrs == INVALID_FILE_ATTRIBUTES) {
			result = -1;
			break;
		}

		if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
			std::string parent_path = compress_path;
			std::replace_if(parent_path.begin(), parent_path.end(), [](char ch) { return ch == '\\'; }, '/');
			if (parent_path.back() == '/') {
				parent_path.pop_back();
			}

			size_t npos = parent_path.find_last_of('/');
			if (npos == std::string::npos) {
				result = -1;
				break;
			}
			parent_path = parent_path.substr(0, npos);

			int error = 0;
			std::string utf8zip_path = ansi_to_utf8(zip_path);
			archive = zip_open(utf8zip_path.c_str(), ZIP_CREATE, &error);
			if (!archive) {
				result = -2;
				break;
			}

			std::list<std::string> files;
			files.push_back(compress_path);
			traversal_directory(compress_path, files, true, &filter_file_cb, (void*)&filter_file_suffix);
			bool success = true;
			for (std::list<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
				std::replace_if(it->begin(), it->end(), [](char ch) { return ch == '\\'; }, '/');
				DWORD attrs = GetFileAttributes(it->c_str());
				zip_int64_t file_index = 0;
				std::string relative_path = ansi_to_utf8(it->substr(parent_path.length() + 1));

				if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
					if (relative_path.back() != '/') {
						relative_path += "/";
					}

					if ((file_index = zip_dir_add(archive, relative_path.c_str(), ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8)) < 0) {
						result = -4;
						success = false;
						break;
					}
				}
				else {
					std::string filepath = ansi_to_utf8(*it);
					zip_source_t* source = zip_source_file(archive, filepath.c_str(), 0, 0);
					if (!source) {
						result = -3;
						success = false;
						break;
					}

					if ((file_index = zip_name_locate(archive, relative_path.c_str(), 0)) >= 0) {
						zip_delete(archive, file_index);
					}

					LogInfo("����ѹ��%s����", it->c_str());
					if ((file_index = zip_file_add(archive, relative_path.c_str(), source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8)) < 0) {
						LogError("ѹ��%sʧ�ܡ�", it->c_str());
						result = -5;
						zip_source_free(source);
						success = false;
						break;
					}
					LogInfo("ѹ��%s�ɹ���", it->c_str());

					if (zip_set_file_compression(archive, file_index, ZIP_CM_DEFLATE, 6) != 0) {
						result = -6;
						success = false;
						break;
					}

					if (!password.empty() && zip_file_set_encryption(archive, file_index, ZIP_EM_AES_256, password.c_str()) != 0) {
						result = -7;
						success = false;
						break;
					}

					if (progress_cb)
						progress_cb(*it, args);
				}

				if (zip_file_set_external_attributes(archive, file_index, 0, ZIP_OPSYS_WINDOWS_NTFS, attrs) != 0) {
					result = -8;
					success = false;
					break;
				}
			}

			if (!success) {
				break;
			}
		}
		else {
			int error = 0;
			std::string utf8dst_filepath = ansi_to_utf8(zip_path);
			archive = zip_open(utf8dst_filepath.c_str(), ZIP_CREATE, &error);
			if (!archive) {
				result = -2;
				break;
			}

			std::string utf8src_filepath = ansi_to_utf8(compress_path);
			zip_source_t* source = zip_source_file(archive, utf8src_filepath.c_str(), 0, 0);
			if (!source) {
				result = -3;
				break;
			}

			zip_uint64_t file_index = 0;
			size_t npos = utf8src_filepath.find_last_of('/');
			if (npos == std::string::npos) {
				result = -1;
				break;
			}

			std::string relative_path = utf8src_filepath.substr(npos + 1);
			if ((file_index = zip_file_add(archive, relative_path.c_str(), source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8)) < 0) {
				result = -5;
				zip_source_free(source);
				break;
			}

			if (zip_set_file_compression(archive, file_index, ZIP_CM_DEFLATE, 6) != 0) {
				result = -6;
				break;
			}

			if (!password.empty() && zip_file_set_encryption(archive, file_index, ZIP_EM_AES_256, password.c_str()) != 0) {
				result = -7;
				break;
			}

			if (zip_file_set_external_attributes(archive, file_index, 0, ZIP_OPSYS_WINDOWS_NTFS, attrs) != 0) {
				result = -8;
				break;
			}
		}

	} while (false);
	if (archive && zip_close(archive) != 0) {
		result = -9;
	}

	if (result != 0) {
		LogError(get_zip_compress_error(result));
	}
	return result;
}
#endif

int miniz_decompress_file(const std::string& zip_filepath, const std::string& extract_path)
{
	mz_zip_archive zip_archive = { 0 };
	std::string filepath = ansi_to_utf8(zip_filepath);
	if (!mz_zip_reader_init_file(&zip_archive, filepath.c_str(), 0)) {
		LogError("zip�ļ�%s��Ч��", zip_filepath.c_str());
		return -1;
	}

	int error_code = 0;
	mz_uint num_files = mz_zip_reader_get_num_files(&zip_archive);
	for (mz_uint i = 0; i < num_files; i++) {
		mz_zip_archive_file_stat file_stat;
		std::string file_path;
		do  {
			file_path.clear();
			if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
				LogError("��ȡ��ѹ�ļ���Ϣʧ�ܡ�");
				error_code = -4;
				break;
			}

			if (mz_zip_reader_is_file_a_directory(&zip_archive, i)) {
				continue;
			}

			// ������ļ��У�������Ӧ��Ŀ¼
			std::string file_name = utf8_to_ansi(file_stat.m_filename);
			file_path = extract_path + "/" + file_name;
			for (size_t i = 0; i < file_path.length(); ++i) {
				if (file_path[i] == '/') {
					file_path[i] = '\\';
				}
			}

			size_t npos = file_path.rfind("\\");
			if (npos != std::string::npos) {
				std::string dir_path = file_path.substr(0, npos);
				if (!create_path(dir_path)) {
					LogError("������ȡ�ļ���%sʧ�ܡ�", dir_path.c_str());
					error_code = -2;
					break;
				}
			}

			LogInfo("���ڽ�ѹ��%s��", file_path.c_str());
			// ������ļ�����ѹ����Ŀ��·��
			void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, (size_t*)&file_stat.m_uncomp_size, 0);
			if (!p) {
				LogError("��ȡ��ѹ�ļ�%s���ڴ�ʧ�ܡ�", file_path.c_str());
				error_code = -5;
				break;
			}

			bool is_write_try_again = true;
			//DWORD old_attrs;
		WRITE_TRY_AGAIN:
			FILE* file = nullptr;
			errno_t err = fopen_s(&file, file_path.c_str(), "wb");
			if (!file) {
				if (err == EACCES && is_write_try_again) {
					DWORD attrs = GetFileAttributes(file_path.c_str());
					if (attrs != INVALID_FILE_ATTRIBUTES) {
						DWORD set = attrs;
						if (attrs & FILE_ATTRIBUTE_HIDDEN) {
							set &= ~FILE_ATTRIBUTE_HIDDEN;
						}

						if (attrs & FILE_ATTRIBUTE_READONLY) {
							set &= ~FILE_ATTRIBUTE_READONLY;
						}
						SetFileAttributes(file_path.c_str(), set);
					}
					is_write_try_again = false;
					goto WRITE_TRY_AGAIN;
				}

				mz_free(p);
				LogError("д���ѹ�ļ�%sʧ�ܣ�����ԭ��%s��", file_path.c_str(), strerror(err));
				error_code = -3;
				break;
			}

			size_t write_ret = fwrite(p, 1, (size_t)file_stat.m_uncomp_size, file);
			if (write_ret != (size_t)file_stat.m_uncomp_size) {
				mz_free(p);
				fclose(file);
				LogError("д���ѹ�ļ�%sʧ�ܣ�У���С��һ�¡�", file_path.c_str());
				error_code = -3;
				break;
			}
			fclose(file);
			mz_free(p);
		} while (false);

		if  (!file_path.empty()) {
			if (error_code == 0) {
				LogInfo("��ѹ��%s�ɹ���", file_path.c_str());
			}
			else {
				LogError("��ѹ��%sʧ�ܣ�������룺%d��", file_path.c_str(), error_code);
			}
		}

		if (error_code != 0) {
			break;
		}
	}
	mz_zip_reader_end(&zip_archive);
	return error_code;
}

#if ENABLE_LIBZIP
/*
* @retval 0 �ɹ�
* @retval -1 ��zip�ļ�ʧ��
* @retval -2 ��ѹ��·����Ч
* @retval -3 ���ý�ѹ������ʧ��
* @retval -4 ��ȡzip�ļ���ʧ��
* @retval -5 ����·��ʧ��
* @retval -6 û����������
* @retval -7 �������벻��ȷ
* @retval -8 ��zip�ļ�ʧ��
* @retval -9 д���ļ�ʧ��
* @retval -10 ��ȡzip�ļ�����ʧ��
* @retval -11 �ر�zip�ļ�ʧ��
*/
static std::string get_libzip_decompress_error(int error_code)
{
	std::string str;
	switch (error_code) {
	case 0:
		str = "�ɹ�";
		break;
	case -1:
		str = "��zip�ļ�ʧ��";
		break;
	case -2:
		str = "��ѹ��·����Ч";
		break;
	case -3:
		str = "���ý�ѹ������ʧ��";
		break;
	case -4:
		str = "��ȡzip�ļ���ʧ��";
		break;
	case -5:
		str = "����·��ʧ��";
		break;
	case -6:
		str = "û����������";
		break;
	case -7:
		str = "�������벻��ȷ";
		break;
	case -8:
		str = "��zip�ļ�ʧ��";
		break;
	case -9:
		str = "д���ļ�ʧ��";
		break;
	case -10:
		str = "��ȡzip�ļ�����ʧ��";
		break;
	case -11:
		str = "�ر�zip�ļ�ʧ��";
		break;
	default:
		str = "δ֪����";
		break;
	}

	return str;
}

int libzip_decompress(const std::string& zip_path, const std::string& decompress_path, const std::string& password) {
	int result = 0;
	zip_t* archive = nullptr;
	do
	{
		DWORD attrs = GetFileAttributes(decompress_path.c_str());
		if (attrs == INVALID_FILE_ATTRIBUTES) {
			if (!create_path(decompress_path.c_str())) {
				result = -2;
				break;
			}
		}
		else {
			if ((attrs & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				result = -2;
				break;
			}
		}

		int error = 0;
		std::string utf8zip_path = ansi_to_utf8(zip_path);
		archive = zip_open(utf8zip_path.c_str(), 0, &error);
		if (!archive) {
			result = -1;
			break;
		}

		if (!password.empty() && (zip_set_default_password(archive, password.c_str()) != 0)) {
			result = -3;
			break;
		}

		bool success = true;
		zip_int64_t num_entries = zip_get_num_entries(archive, 0);
		for (zip_int64_t i = 0; i < num_entries; ++i) {
			const char* utf8_name = zip_get_name(archive, i, 0);
			if (!utf8_name) {
				result = -4;
				success = false;
				break;
			}

			std::string ansi_name = utf8_to_ansi(utf8_name);
			std::string file_path = decompress_path;
			if (file_path.back() != '/') {
				file_path += "/";
				file_path += ansi_name;
			}
			else {
				file_path += ansi_name;
			}

			if (ansi_name[ansi_name.length() - 1] == '/') {
				if (!create_path(file_path)) {
					result = -5;
					success = false;
					break;
				}
				continue;
			}

			size_t last_slash = file_path.find_last_of("/");
			if (last_slash != std::string::npos) {
				std::string parent_dir = file_path.substr(0, last_slash);
				if (!create_path(parent_dir)) {
					result = -5;
					success = false;
					break;
				}
			}

			zip_file_t* file = zip_fopen_index(archive, i, 0);
			if (!file) {
				zip_error_t* zerr = zip_get_error(archive);
				int zcode = zip_error_code_zip(zerr);
				if (zcode == ZIP_ER_NOPASSWD) {
					result = -6;
					//not input password;
				}
				else if (zcode == ZIP_ER_WRONGPASSWD) {
					//password incorrect
					result = -7;
				}
				else {
					result = -8;
				}
				success = false;
				break;
			}

			{
				DWORD attrs = GetFileAttributes(file_path.c_str());
				if (attrs != INVALID_FILE_ATTRIBUTES) {
					SetFileAttributes(file_path.c_str(), FILE_ATTRIBUTE_NORMAL);
				}
			}

			LogInfo("���ڽ�ѹ%s����", file_path.c_str());
			std::ofstream out(file_path, std::ios::binary | std::ios::trunc);
			if (!out.is_open()) {
				LogError("��ѹ%sʧ�ܡ�", file_path.c_str());
				result = -9;
				success = false;
				zip_fclose(file);
				break;
			}

			char buffer[8192] = { 0 };
			zip_int64_t size = 0;
			while ((size = zip_fread(file, buffer, sizeof(buffer))) > 0) {
				out.write(buffer, size);
			}

			if (size < 0) {
				LogError("��ѹ%sʧ�ܡ�", file_path.c_str());
				result = -9;
				success = false;
				zip_fclose(file);
				break;
			}
			out.close();
			LogInfo("��ѹ%s�ɹ���", file_path.c_str());

			zip_uint8_t opsys = 0;
			zip_uint32_t attrs = 0;
			if (zip_file_get_external_attributes(archive, i, 0, &opsys, &attrs) < 0) {
				result = -10;
				success = false;
				zip_fclose(file);
				break;
			}
			zip_fclose(file);
			SetFileAttributes(file_path.c_str(), attrs);
		}

		if (!success) {
			break;
		}
		result = 0;
	} while (false);
	if (archive && zip_close(archive) != 0) {
		result = -11;
	}

	if (result !=0 ) {
		LogError(get_zip_decompress_error(result));
	}
	return result;
}
#endif

std::string get_current_path()
{
	size_t buf_size = 8192;
	char* buf_data = nullptr;
retry_0:
	buf_data = new char[buf_size];
	memset(buf_data, 0, buf_size);
	if (GetModuleFileName(nullptr, buf_data, buf_size) == buf_size && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		delete[] buf_data;
		buf_size *= 2;
		goto retry_0;
	}

	std::string file_path(buf_data);
	delete[] buf_data;

	size_t npos = file_path.rfind("\\");
	if (npos != std::string::npos) {
		file_path = file_path.substr(0, npos);
	}
	else 
	{
retry_1:
		buf_data = new char[buf_size];
		memset(buf_data, 0, buf_size);
		DWORD need_bytes = GetCurrentDirectory(buf_size, buf_data);
		if (buf_size < need_bytes) {
			delete[] buf_data;
			buf_size *= 2;
			goto retry_1;
		}
		file_path = buf_data;
		delete[] buf_data;
	}
	return file_path;
}

std::string get_base_name(const std::string& file_name, bool is_special)
{
	std::string base_name;
	size_t npos = file_name.find('[');
	if (npos != std::string::npos && is_special)
	{
		base_name = file_name.substr(0, npos);
	}
	else
	{
		//�����ض��ļ���ȥ��.exe��׺,�õ��ļ���app
		npos = file_name.rfind('.');
		if (npos != std::string::npos)
		{
			base_name = file_name.substr(0, npos);
		}
	}
	return base_name;
}

std::string get_file_name(const std::string& file_path) {
	size_t npos = file_path.find_last_of('\\');
	if (npos != std::string::npos) {
		return file_path.substr(npos + 1);
	}
	else {
		npos = file_path.find_last_of('/');
		if (npos != std::string::npos) {
			return file_path.substr(npos + 1);
		}
	}
	return std::string();
}

bool is_occupied(const std::string& name)
{
	bool result = false;
	int handle = _sopen(name.c_str(), _O_RDWR, _SH_DENYRW, _S_IREAD | _S_IWRITE);
	if (-1 == handle) {
		result = true;
	}
	else {
		_close(handle);
	}
	return result;
}

int compare_file(const std::string& file_path1, const std::string& file_path2) {
	int error_code = 0;
	FILE* file1 = nullptr, *file2 = nullptr;
	do 
	{
		fopen_s(&file1, file_path1.c_str(), "rb");
		if (!file1) {
			error_code = -1;
			break;
		}

		fopen_s(&file2, file_path2.c_str(), "rb");
		if (!file2) {
			error_code = -1;
			break;
		}

		_fseeki64(file1, SEEK_SET, SEEK_END);
		long long size1 = _ftelli64(file1);
		_fseeki64(file1, SEEK_SET, SEEK_SET);

		_fseeki64(file2, SEEK_SET, SEEK_END);
		long long size2 = _ftelli64(file2);
		_fseeki64(file2, SEEK_SET, SEEK_SET);

		if (size1 != size2) {
			error_code = 1;
			break;
		}

		//����1GBֱ�Ӳ����жԱ�����
		if (size1 / 1024 / 1024 > 1024) {
			error_code = 1;
			break;
		}

		if (size2 / 1024 / 1024 > 1024) {
			error_code = 1;
			break;
		}

		bool success = true;
		char data1[4096] = { 0 }, data2[4096] = { 0 };
		for (long long i = 0; i < size1; i += 4096) {
			size_t read_once = static_cast<size_t>((size1 - i > 4096) ? 4096 : size1 - i);
			size_t read_size1 = fread(data1, 1, read_once, file1);
			if (read_size1 != read_once) {
				success = false;
				break;
			}

			size_t read_size2 = fread(data2, 1, read_once, file2);
			if (read_size2 != read_once) {
				success = false;
				break;
			}

			if (memcmp(data1, data2, read_size1) != 0) {
				success = false;
				break;
			}
		}

		if (!success) {
			error_code = 1;
			break;
		}
		error_code = 0;
	} while (false);

	if (file1) {
		fclose(file1);
	}

	if (file2) {
		fclose(file2);
	}
	return error_code;
}

bool copy_file(const std::string& source_file, const std::string& dest_file)
{
	bool result = false;
	FILE* file1 = nullptr, *file2 = nullptr;
	do 
	{
		fopen_s(&file1, source_file.c_str(), "rb");
		if (!file1) {
			break;
		}

		fopen_s(&file2, dest_file.c_str(), "wb");
		if (!file2) {
			break;
		}

		_fseeki64(file1, SEEK_SET, SEEK_END);
		long long size1 = _ftelli64(file1);
		_fseeki64(file1, SEEK_SET, SEEK_SET);

		bool success = true;
		char data[8192] = { 0 };
		for (long long i = 0; i < size1; i += 8192) {
			size_t once = static_cast<size_t>((size1 - i > 8192) ? 8192 : size1 - i);
			size_t size = fread(data, 1, once, file1);
			if (size != once) {
				success = false;
				break;
			}

			size = fwrite(data, 1, once, file2);
			if (size != once) {
				success = false;
				break;
			}
		}

		if (!success) {
			break;
		}

		result = true;
	} while (false);

	if (file1) {
		fclose(file1);
	}

	if (file2) {
		fclose(file2);
	}
	return result;
}

size_t get_folder_file_number(const std::string& dir_path, filter_file_cb_t filter_cb, void* args)
{
	std::list<std::string> files;
	get_folder_files(dir_path, files, true, filter_cb, args);
	return files.size();
}

bool copy_folder(const std::string& sourceFolder, const std::string& destFolder,
				 filter_file_cb_t filter_file_cb, void* args)
{
	if (!CreateDirectory(destFolder.c_str(), nullptr)) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			LogError("�����ļ���%sʧ�ܣ��������%lu��", destFolder.c_str(), GetLastError());
			return false;
		}
	}

	std::string searchPath = sourceFolder + "\\*.*";
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		LogError("���ҵ�һ���ļ�ʧ�ܣ��������%lu��", GetLastError());
		return false;
	}

	do {
		std::string sourceFile = sourceFolder + "\\" + findData.cFileName;
		std::string destFile = destFolder + "\\" + findData.cFileName;

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
				if (filter_file_cb && filter_file_cb(sourceFile, args)) {
					//printf("not copy dir %s->%s\n", sourceFile.c_str(), destFile.c_str());
					continue;
				}
				//printf("copy dir %s->%s\n", sourceFile.c_str(), destFile.c_str());

				if (!copy_folder(sourceFile, destFile, filter_file_cb, args)) {
					FindClose(hFind);
					return false;
				}
			}
		}
		else {
			if (filter_file_cb && filter_file_cb(sourceFile, args)) {
				//printf("not copy file %s->%s\n", sourceFile.c_str(), destFile.c_str());
				continue;
			}
			//printf("copy file %s->%s\n", sourceFile.c_str(), destFile.c_str());

			bool is_first_esv = true, is_first_ead = true;
RETRY_COPY_FILE:
			std::string old_dest_file;
			bool find_sensitive_dll = false;
			if (sensitive_dll_set.size()) {
				//�ҵ�����DLL
				std::string file_name = get_file_name(destFile);
				for (std::set<std::string>::iterator iter = sensitive_dll_set.begin();
					iter != sensitive_dll_set.end(); ++iter) {
						if (!_stricmp((*iter).c_str(), file_name.c_str())) {
							size_t npos = destFile.find_last_of('\\');
							if (npos != std::string::npos) {
								find_sensitive_dll = true;
								old_dest_file = destFile;
								destFile.replace(npos + 1, destFile.length() - npos + 1, *iter + ".log");
							}
							break;
						}
				}
				/*std::set<std::string>::iterator iter = sensitive_dll_set.find(file_name);
				if (iter != sensitive_dll_set.end()) {
					size_t npos = destFile.find_last_of('\\');
					if (npos != std::string::npos) {
						find_sensitive_dll = true;
						old_dest_file = destFile;
						destFile.replace(npos + 1, destFile.length() - npos + 1, *iter + ".log");
					}
				}*/
			}

			BOOL success = CopyFile(sourceFile.c_str(), destFile.c_str(), FALSE);
			if (success && find_sensitive_dll) {
				if (!MoveFileEx(destFile.c_str(), old_dest_file.c_str(), MOVEFILE_REPLACE_EXISTING)) {
					LogError("MoveFileEx����ʧ�ܣ�������룺%lu", GetLastError());
					CloseHandle(hFind);
					return false;
				}
				destFile = old_dest_file;
			}

			DWORD error = 0;
			if (success) {
				LogInfo("����%s��%s�ɹ���", sourceFile.c_str(), destFile.c_str());
			}
			else {
				LogError("����%s��%sʧ�ܣ��������%lu��", sourceFile.c_str(), destFile.c_str(), error = GetLastError());
			}
			
			if (success == FALSE) {
				if (error == ERROR_SHARING_VIOLATION) {
					if (is_first_esv) {
						is_first_esv = false;
						kill_occupied_file_process(destFile);
						LogWarn("�ļ�%s��ռ���У����ڽ������ԡ�", destFile.c_str());
						Sleep(1000);
						goto RETRY_COPY_FILE;
					}

					//�����ռ�ã��Ա��ļ��Ƿ���ȫһ��
					int ret_compare = compare_file(sourceFile, destFile);
					if (ret_compare != 0) {
						FindClose(hFind);
						return false;
					}
					
					LogWarn("%s��%s������ͬ���������ơ�", sourceFile.c_str(), destFile.c_str());
					//��ֱͬ�������������κδ���
				}
				else if (error == ERROR_ACCESS_DENIED) {
					if (is_first_ead) {
						is_first_ead = false;
						SetFileAttributes(destFile.c_str(), FILE_ATTRIBUTE_NORMAL);
						kill_occupied_file_process(destFile);
						LogWarn("���޸��ļ�%s���ԣ����ڽ������Բ�����", destFile.c_str());
						Sleep(1000);
						goto RETRY_COPY_FILE;
					}

					int ret_compare = compare_file(sourceFile, destFile);
					if (ret_compare != 0) {
						LogError("�����ļ�%s���ܾ������ܴ��ڱ�������������ء�", sourceFile.c_str());
						FindClose(hFind);
						return false;
					}

					LogWarn("%s��%s������ͬ���������ơ�", sourceFile.c_str(), destFile.c_str());
					//��ֱͬ�������������κδ���
				}
				else {
					FindClose(hFind);
					return false;
				}
			}
		}
	} while (FindNextFile(hFind, &findData) != 0);
	FindClose(hFind);
	return true;
}

bool remove_folder(const std::string& path) 
{
	if (!RemoveDirectory(path.c_str())) {
		// ����ļ��в�Ϊ�գ���ݹ�ɾ���ļ������ļ���
		if (GetLastError() != ERROR_DIR_NOT_EMPTY) {
			// ���ַ�"Ŀ¼��Ϊ��"�Ĵ��󣬷���ʧ��
			return false;
		}

		// �����ļ����е������ļ������ļ���
		WIN32_FIND_DATA findFileData;
		std::string searchPath = path + "\\*";
		HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);
		if (hFind == INVALID_HANDLE_VALUE) {
			// ����ʧ�ܣ��������ļ��в����ڻ���������
			return false;
		}

		do {
			// ������ǰĿ¼��.�����ϼ�Ŀ¼��..��
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp(findFileData.cFileName, ".") != 0 &&
					strcmp(findFileData.cFileName, "..") != 0) {
						std::string subPath = path + "\\" + findFileData.cFileName;
						// �ݹ�ɾ�����ļ���
						if (!remove_folder(subPath)) {
							FindClose(hFind);
							return false;
						}
				}

			} else {
				// ɾ���ļ�
				bool is_first_esv = true, is_first_ead = true;
				std::string filePath = path + "\\" + findFileData.cFileName;
			RETRY_DELETE_FILE:
				BOOL success = DeleteFile(filePath.c_str());
				if (success) {
					LogInfo("ɾ��%s�ɹ���", filePath.c_str());
				}
				else{
					DWORD error = GetLastError();
					if (error == ERROR_SHARING_VIOLATION) {
						if (is_first_esv) {
							is_first_esv = false;
							kill_occupied_file_process(filePath);
							Sleep(1000);
							goto RETRY_DELETE_FILE;
						}
						success = true;
						LogWarn("ɾ��%sʧ�ܣ����ļ�����һ������ռ���У��Ժ����ֶ�ɾ����", filePath.c_str());
					}
					else if (error == ERROR_ACCESS_DENIED) {
						if (is_first_ead) {
							is_first_ead = false;
							SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_NORMAL);
							kill_occupied_file_process(filePath);
							Sleep(1000);
							goto RETRY_DELETE_FILE;
						}
						success = true;
						LogWarn("ɾ��%sʧ�ܣ��ܾ����ʡ�", filePath.c_str());
					}
					else {
						LogError("ɾ��%sʧ�ܣ��������%lu��", filePath.c_str(), error);
					}
				}
				
				if (!success) {
					FindClose(hFind);
					return false;
				}
			}
		} while (FindNextFile(hFind, &findFileData));

		FindClose(hFind);

		// �ٴγ���ɾ���ļ��У������Ӧ���ǿյ�
		if (!RemoveDirectory(path.c_str())) {
			return false;
		}
	}
	return true;
}

void find_file(const char* dir, const char* suffix, std::list<std::string>& files, bool recursion)
{
	WIN32_FIND_DATA wfd = { 0 };
	HANDLE handle = NULL;
	std::string temp = std::string(dir) + "\\*";
	if ((handle = FindFirstFile(temp.c_str(), &wfd)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!recursion)
					continue;

				if (!strcmp(wfd.cFileName, ".") || !strcmp(wfd.cFileName, ".."))
					continue;
				temp = std::string(dir) + "\\" + std::string(wfd.cFileName);
				find_file(temp.c_str(), suffix, files, recursion);
			}
			else
			{
				temp = std::string(dir) + "\\" + std::string(wfd.cFileName);
				char* ptr = strrchr(const_cast<char*>(temp.c_str()), '.');
				if (ptr && !strcmp(ptr, suffix))
					files.push_back(temp);
			}
		} while (FindNextFile(handle, &wfd));
		FindClose(handle);
	}
	return;
}

bool create_shortcut(const std::string& file_path, bool recursion_replace)
{
	bool result = false, success = true;
	static_cast<void>(CoInitialize(NULL));
	do
	{
		//��ȡ����·��
		LPITEMIDLIST item_list = nullptr;
		if (SHGetSpecialFolderLocation(NULL,
			CSIDL_DESKTOPDIRECTORY,
			&item_list) != S_OK) {
				break;
		}

		char desktop_path[4096] = { 0 };
		if (!SHGetPathFromIDList(item_list, desktop_path)) {
			CoTaskMemFree(item_list);
			break;
		}
		CoTaskMemFree(item_list);

		//��ѯ��������,�Ƿ��Ѵ���,��һ���汾
		std::list<std::string> file_list;
		find_file(desktop_path, ".lnk", file_list, recursion_replace);

		std::string file_name;
		size_t npos = file_path.rfind("\\");
		if (npos != std::string::npos) {
			file_name = file_path.substr(npos + 1);
		}
		else {
			file_name = "δ֪.exe";
		}
		std::string base_name = get_base_name(file_name);
		//std::string base_name = file_name;
		//size_t npos = base_name.find('[');
		//if (npos != std::string::npos) {
		//	//�ض��ļ���,ȥ��[x.x.x.x]�汾��
		//	base_name = base_name.substr(0, npos);
		//}
		//else {
		//	npos = base_name.rfind('.');
		//	if (npos != std::string::npos) {
		//		base_name = base_name.substr(0, npos);
		//	}
		//	//�Ƴ�" - ����"��" - ���� (1)"..." - ���� (N)"
		//	//utility::remove_filename_copy_str(base_name);
		//}

		std::list<std::string> name_list;
		bool find_old = false;
		//�ж��Ƿ���ڶ���ɿ�ݷ�ʽ,������ڽ�ȫ���滻
		for (std::list<std::string>::iterator iter = file_list.begin(); iter != file_list.end(); ++iter) {
			std::string dst_name;
			std::string x = *iter;
			if (x.rfind('\\') != std::string::npos) {
				dst_name = x.substr(x.rfind('\\') + 1);
				if (dst_name.find('[') != std::string::npos) {
					dst_name = dst_name.substr(0, dst_name.find('['));
				}
				else {
					//�������ض��ļ�����ʽ,ȥ��.lnk��׺��
					size_t npos = dst_name.rfind('.');
					if (npos != std::string::npos) {
						dst_name = dst_name.substr(0, npos);
					}

					//utility::remove_filename_copy_str(dst_name);
				}
			}

			//��������ж�
			if (base_name.find(dst_name) != std::string::npos ||
				dst_name.find(base_name) != std::string::npos ||
				dst_name == base_name) {
					find_old = true;
					name_list.push_back(x);
			}
		}

		std::list<std::string> lnk_list;
		if (find_old) {
			for (std::list<std::string>::iterator iter = name_list.begin(); iter != name_list.end(); ++iter) {
				std::string x = *iter;
				//ɾ���ɿ�ݷ�ʽ
				DeleteFile(x.c_str());
				SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

				//��ȡ�ɿ�ݷ�ʽ��·��
				std::string path = x;
				size_t npos = path.rfind('\\');
				if (npos != std::string::npos) {
					path = path.substr(0, npos);
					//����¿�ݷ�ʽ��
					std::string lnk_name = path + std::string("\\") + base_name + std::string(".lnk");
					lnk_list.push_back(lnk_name);
				}
			}
		}
		else {
			std::string lnk_name = desktop_path + std::string("\\") + base_name + std::string(".lnk");
			lnk_list.push_back(lnk_name);
		}

		for (std::list<std::string>::iterator iter0 = lnk_list.begin(); iter0 != lnk_list.end(); ++iter0) {
			std::string x = *iter0;
			IShellLinkA* link = nullptr;
			HRESULT ret = S_FALSE;
			if ((ret = CoCreateInstance(CLSID_ShellLink,
				NULL, CLSCTX_INPROC_SERVER,
				IID_IShellLink,
				(void**)&link)) != S_OK) {
					success = false;
					break;
			}

			std::string working_directory;
			size_t npos = file_path.rfind('\\');
			if (npos != std::string::npos) {
				working_directory = file_path.substr(0, npos);
			}
			else{
				working_directory = get_current_path();	
			}
			link->SetWorkingDirectory(working_directory.c_str());
			link->SetPath(file_path.c_str());
			link->SetIconLocation(file_path.c_str(), 0);

			IPersistFile* ipf = nullptr;
			if (link->QueryInterface(IID_IPersistFile, (void**)&ipf) != S_OK) {
				success = false;
				if (ipf) {
					ipf->Release();
				}
				break;
			}

			wchar_t* wstr = multiByteToWideChar(x.c_str(), CP_ACP);
			ipf->Save(wstr, TRUE);
			ipf->Release();
			delete[] wstr;
		}

		if (!success) {
			break;
		}
		result = true;
	} while (false);
	CoUninitialize();
	return result;
}

bool get_all_processes(std::map<DWORD, std::string>& processes)
{
	bool result = false;
	do
	{
		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(pe32);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE)
		{
			break;
		}

		BOOL next = Process32First(snapshot, &pe32);
		while (next)
		{
			processes.insert(std::make_pair(pe32.th32ProcessID, std::string(pe32.szExeFile)));
			next = Process32Next(snapshot, &pe32);
		}
		CloseHandle(snapshot);
		result = true;
	} while (false);
	return result;
}

static bool stdmap_contains(const std::map<std::string, std::string>& t, const std::string& v)
{
	return t.find(v) != t.end();
}

static bool to_bool(const std::string& str) { return _stricmp(str.c_str(), "true") == 0; }

cmdline_option_t parse_command_line_option(const std::string& cmdline)
{
	std::map<std::string, std::string> ret_map;
	std::vector<std::string> key_list = get_cmdline_option_arr();
	for (std::vector<std::string>::const_iterator iter = key_list.begin();
		iter != key_list.end(); ++iter) {
			size_t start_npos = cmdline.find(*iter);
			if (start_npos != std::string::npos) {
				start_npos += 1;
				size_t tmp_npos = std::string::npos;
TRAY_REFIND:
				size_t end_npos = cmdline.find("-", tmp_npos == std::string::npos ? start_npos : tmp_npos);
				if (end_npos != std::string::npos) {
					std::string temp = cmdline.substr(end_npos);
					bool find = false;
					for (size_t i = 0; i < key_list.size(); ++i) {
						if (strncmp(temp.c_str(), key_list[i].c_str(), key_list[i].length()) == 0) {
							find = true;
							break;
						}
					}

					if (!find) {
						tmp_npos = end_npos + 1;
						goto TRAY_REFIND;
					}
				}
				
				std::string kv = cmdline.substr(start_npos, end_npos - start_npos);
				size_t first_npos = kv.find_first_of("\"");
				size_t last_npos = kv.find_last_of("\"");
				bool exist_double_marks = (first_npos != std::string::npos && last_npos != std::string::npos);

				std::string str = kv;
				if (exist_double_marks) {
					str.erase(first_npos, 1);
					str.erase(last_npos - 1, 1);
				}
				/*else {
					bool find_space = false;
					for (size_t i = 0;i < kv.length(); ++i) {
						if (kv[i] == ' ' && !find_space) {
							str.insert(str.begin() + str.length(), kv[i]);
							find_space = true;
						}
						else if (kv[i] != ' ') {
							str.insert(str.begin() + str.length(), kv[i]);
						}
					}
				}*/

				size_t pos = str.find_first_of(" ");
				if (pos != std::string::npos) {
					std::string key = str.substr(0, pos);
					std::string value = str.substr(pos + 1);
					trim_str(key);
					trim_str(value);
					ret_map.insert(std::make_pair(key, value));
				}
			}
	}

	cmdline_option_t cmdline_option;
	cmdline_option.kv = ret_map;

	std::string option = "is_backup_file";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_backup_file = to_bool(ret_map[option]);
	}

	option = "is_create_shortcut";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_create_shortcut = to_bool(ret_map[option]);
	}

	option = "is_delete_update_file";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_delete_update_file = to_bool(ret_map[option]);
	}

	option = "is_hide_window";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_hide_window = to_bool(ret_map[option]);
	}

	option = "is_recursion_replace_shortcut";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_recursion_replace_shortcut = to_bool(ret_map[option]);
	}

	option = "is_show_topmost";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_show_topmost = to_bool(ret_map[option]);
	}

	option = "old_version_process_id";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.old_version_process_id = atol(ret_map[option].c_str());
	}

	option = "old_version_process_name";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.old_version_process_name = ret_map[option];
	}

	option = "start_process_command_line";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.start_process_command_line = ret_map[option];
	}

	option = "source_path";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.source_path = ret_map[option];
		replace_with_windows_path_symbol(cmdline_option.source_path);
	}

	option = "destination_path";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.destination_path = ret_map[option];
		replace_with_windows_path_symbol(cmdline_option.destination_path);
	}

	option = "window_height";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.window_height = atoi(ret_map[option].c_str());
	}

	option = "window_title";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.window_title = ret_map[option];
	}

	option = "window_width";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.window_width = atoi(ret_map[option].c_str());
	}

	option = "is_compress_backup_file";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_compress_backup_file = to_bool(ret_map[option]);
	}

	option = "sensitive_dll";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.sensitive_dll = ret_map[option];
	}

	option = "latest_version";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.latest_version = ret_map[option];
	}

	option = "is_only_start_process";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_only_start_process = to_bool(ret_map[option]);
	}

	option = "is_prompt_if_error";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_prompt_if_error = to_bool(ret_map[option]);
	}

	option = "start_process_file_path";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.start_process_file_path = ret_map[option];
		replace_with_windows_path_symbol(cmdline_option.start_process_file_path);
	}

	option = "start_process_working_directory";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.start_process_working_directory = ret_map[option];
		replace_with_windows_path_symbol(cmdline_option.start_process_working_directory);
	}

	option = "server_address";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.server_address = ret_map[option];
	}

	option = "server_port";
	if (stdmap_contains(ret_map, option)) {
		int port = atoi(ret_map[option].c_str());
		cmdline_option.server_port = port == 0 ? 60682 : port;
	}

	option = "upload_update_record_flag";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.upload_update_record_flag = atoi(ret_map[option].c_str());
	}

	option = "window_style";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.window_style = atoi(ret_map[option].c_str());
	}

	option = "is_delete_old_version_backup_file";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_delete_old_version_backup_file = to_bool(ret_map[option]);
	}

	option = "is_wait_old_version_exit";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.is_wait_old_version_exit = to_bool(ret_map[option]);
	}

	option = "wait_old_version_exit_timeout";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.wait_old_version_exit_timeout = atoi(ret_map[option].c_str());
	}

	option = "kill_old_version_process_flag";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.kill_old_version_process_flag = atoi(ret_map[option].c_str());
	}

	option = "upload_update_log_max_size";
	if (stdmap_contains(ret_map, option)) {
		cmdline_option.upload_update_log_max_size = atoi(ret_map[option].c_str()) * 1024 * 1024;
		if (cmdline_option.upload_update_log_max_size == 0) {
			cmdline_option.upload_update_log_max_size = 5 * 1024 * 1024;
		}
	}

	return cmdline_option;
}

bool check_path_is_valid(const std::string& source_path, const std::string& destination_path)
{
	LogInfo("���ڼ��·���Ƿ���Ч�������ĵȴ�����");
	bool result = false;
	do 
	{
		//���Դ·��
		int ret = _access(source_path.c_str(), 0);
		if (ret == 0) {
			LogInfo("Դ·��%s��Ч��", source_path.c_str());
		}
		else {
			LogError("Դ·��%s��Ч��", source_path.c_str());
			break;
		}

		//���Ŀ��·��
		ret = _access(destination_path.c_str(), 0);
		if (ret == 0) {
			LogInfo("Ŀ��·��%s��Ч��", destination_path.c_str());
		}
		else {
			LogError("Ŀ��·��%s��Ч��", destination_path.c_str());
			break;
		}

		LogInfo("���·���Ƿ���Ч��ɣ�");
		result = true;
	} while (false);
	return result;
}

bool wait_process_exit(DWORD process_id, const std::string& process_name, int kill_flag, size_t timeout)
{
	LogInfo("�ȴ������Ƿ��˳��У������ĵȴ�����");
	bool result = false;
	do 
	{
		size_t start = GetTickCount();
		while(true) {
			std::map<DWORD, std::string> pid_map;
			get_all_processes(pid_map);
			bool find = false;
			for (std::map<DWORD, std::string>::iterator iter = pid_map.begin(); iter != pid_map.end(); ++iter) {
				if (kill_flag == 0 && process_id != 0) {
					if (iter->first == process_id) {
						LogWarn("�ȴ�%s�˳��С���", process_name.c_str());
						find = true;
						break;
					}
				}
				else {
					if (iter->second.substr(0, process_name.length()) == process_name) {
						LogWarn("�ȴ�%s�˳��С���", process_name.c_str());
						find = true;
						break;
					}
				}
			}

			if (!find) {
				LogInfo("����%s���˳���", process_name.c_str());
				break;
			}

			if (GetTickCount() - start > timeout) {
				LogInfo("����%sδ�˳���", process_name.c_str());
				break;
			}
			Sleep(100);
		}

		result = true;
	} while (false);
	LogInfo("�ȴ������Ƿ��˳���ɣ�");
	return result;
}

bool check_process_is_exit(DWORD process_id, const std::string& process_name, int kill_flag, size_t timeout)
{
	LogInfo("�������Ƿ��˳��У������ĵȴ�����");
	bool result = false, success = true, is_first_check = true;

	do 
	{
		size_t start_time = GetTickCount();
		while(true) {
			bool is_app_run = false;
			std::map<DWORD, std::string> pid_map;
			get_all_processes(pid_map);

			std::list<DWORD> pid_list;
			for (std::map<DWORD, std::string>::iterator iter = pid_map.begin(); iter != pid_map.end(); ++iter) {
				if (kill_flag == 0 && process_id !=0) {
					if (iter->first == process_id) {
						pid_list.push_back(iter->first);
						LogWarn("�ȴ�%s�˳��С���", process_name.c_str());
						is_app_run = true;
						break;
					}
				}
				else {
					if (iter->second.substr(0, process_name.length()) == process_name) {
						pid_list.push_back(iter->first);
						LogWarn("�ȴ�%s�˳��С���", process_name.c_str());
						is_app_run = true;
					}
				}
			}

			if (is_first_check && !is_app_run) {
				LogInfo("��������ȫ�˳���");
				break;
			}

			is_first_check = false;

			if (!is_app_run) {
				break;
			}

			int exit_num = 0;
			for (std::list<DWORD>::iterator iter = pid_list.begin(); iter != pid_list.end(); ++iter) {
				if (terminate_process(*iter)) {
					LogInfo("��ֹ����%s�ɹ�����PIDΪ%lu��", process_name.c_str(), *iter);
					++exit_num;
				}
				else {
					LogError("��ֹ����%sʧ�ܣ��������%lu��", process_name.c_str(), GetLastError());
				}
			}

			if (pid_list.size() == exit_num) {
				break;
			}

			if (GetTickCount() - start_time > timeout) {
				success = false;
				LogError("�������Ƿ��˳���ʱ��");
				break;
			}
			Sleep(100);
		}

		if (!success) {
			break;
		}

		LogInfo("�������Ƿ��˳���ɣ�");
		result = true;
	} while (false);
	return result;
}

bool check_file_is_occupy(const std::string& destination_path)
{
	bool result = true, success = true, wait = false;
	LogInfo("����ļ��Ƿ�ռ���У������ĵȴ�����");
	std::list<std::string> files;
	std::vector<DWORD> closed_pids;
	traversal_directory(destination_path, files, false);
	for (std::list<std::string>::iterator iter = files.begin(); iter != files.end();) {
		DWORD attrs = GetFileAttributes(iter->c_str());
		if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
			++iter;
			continue;
		}

		std::map<DWORD, std::string> pids = get_occupied_file_pids(*iter);
		std::string filepath = *iter;
		if (pids.empty()) {
			LogInfo("%sδռ�á�", filepath.c_str());
		}
		else {
			LogWarn("%sռ���С�", filepath.c_str());
			for (std::map<DWORD, std::string>::iterator it = pids.begin(); it != pids.end(); ++it) {
				std::vector<DWORD>::iterator pid_it =  std::find(closed_pids.begin(), closed_pids.end(), it->first);
				if (pid_it != closed_pids.end()) {
					wait = true;
					LogWarn("ռ�ý���%s�ѹرյȴ�ϵͳ������Դ�С���", it->second.c_str());
					break;
				}

				success = terminate_process(it->first);
				if (success) {
					LogInfo("�ر�ռ�ý���%s�ɹ�����PID��%lu��", it->second.c_str(), it->first);
					closed_pids.push_back(it->first);
				}
				else {
					LogError("�ر�ռ�ý���%sʧ�ܣ���PID��%lu��������룺%lu��", it->second.c_str(), 
						it->first, GetLastError());
				}
				
				if (!success) {
					result = false;
					break;
				}
			}

			if (!success) {
				break;
			}
		}

		if (wait) {
			wait = false;
			Sleep(100);
			continue;
		}

		++iter;
	}

	if (result) {
		LogInfo("����ļ��Ƿ�ռ����ɣ�");
	}
	return result;
}

//bool backup_file(const std::string& source_path, const std::string& destination_path,
//				 const std::string& backup_file_filter_suffix,
//				 bool is_compress_backup_file)
//{
//	bool result = false;
//	do 
//	{
//		char localappdata_path[8192] = { 0 };
//		if (SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localappdata_path) != S_OK) {
//			LogInfo("�����ļ�ʧ�ܣ���ȡLOCALAPPDATAĿ¼ʧ�ܡ�");
//			break;
//		}
//
//		format_str("%s\\imp_updater_backup\\%s", localappdata_path, "");
//
//		//�����ļ�
//		LogInfo("���ڱ����ļ��������ĵȴ�����");
//		size_t source_path_npos = source_path.rfind("\\");
//		if (source_path_npos == std::string::npos) {
//			LogInfo("�����ļ�ʧ�ܣ���ȡԴ·���ϼ�Ŀ¼ʧ�ܡ�");
//			break;
//		}
//
//		std::list<std::string> filter_file_suffix;
//		if (!backup_file_filter_suffix.empty()) {
//			filter_file_suffix = split_by_semicolon(backup_file_filter_suffix, ';');
//		}
//
//		if (is_compress_backup_file) {
//			std::string zip_filepath = source_path.substr(0, source_path_npos) + "\\backup.zip";
//			int ret = zip_compress_folder(destination_path.c_str(), zip_filepath.c_str(), filter_file_suffix);
//
//			if (ret != 0)
//			{
//				std::string error_string;
//				switch (ret) {
//				case -1:
//					error_string = format_str("�����ļ�ʧ�ܣ�ѹ���ļ���·����Ч��");
//					break;
//				case -2:
//					error_string = format_str("�����ļ�ʧ�ܣ�zip�ļ�·����Ч��");
//					break;
//				case -3:
//					error_string = format_str("�����ļ�ʧ�ܣ���ȡѹ���ļ���Ч��");
//					break;
//				case -4:
//					error_string = format_str("�����ļ�ʧ�ܣ��ڴ治�㣬�޷�����ѹ����");
//					break;
//				case -5:
//					error_string = format_str("�����ļ�ʧ�ܣ�ѹ���ļ��ϲ����ڴ�ʧ�ܣ�");
//					break;
//				default:
//					error_string = format_str("�����ļ�ʧ�ܣ�δ֪����");
//					break;
//				}
//				LogInfo(error_string);
//				break;
//			}
//		}
//		else {
//			std::string bakcup_filepath = source_path.substr(0, source_path_npos) + "\\backup";
//			if (!copy_folder(destination_path, bakcup_filepath, filter_file_cb, (void*)&filter_file_suffix)) {
//				break;
//			}
//		}
//
//		result = true;
//	} while (false);
//
//	if (result) {
//		LogInfo("�����ļ���ɡ�");
//	}
//	return result;
//}

bool backup_file(const std::string& process_name,
				 const std::string& latest_version,
				 const std::string& source_path,
				 const std::string& destination_path,
				 bool is_compress_backup_file,
				 progress_cb_t progress_cb,
				 void* args)
{
	bool result = false;
	do 
	{
		char localappdata_path[8192] = { 0 };
		if (SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localappdata_path) != S_OK) {
			LogError("�����ļ�ʧ�ܣ���ȡLOCALAPPDATAĿ¼ʧ�ܡ�");
			break;
		}

		std::string file_path;
		size_t npos = process_name.find_last_of('.');
		if (npos != std::string::npos) {
			file_path = format_str("%s\\imp_updater_backup\\%s\\%s", localappdata_path, 
				process_name.substr(0, npos).c_str(),
				latest_version.c_str());
		}
		else
		{
			file_path = format_str("%s\\imp_updater_backup\\%s\\%s", localappdata_path, 
				process_name.c_str(),
				latest_version.c_str());
		}

		if (!create_path(file_path)) {
			LogError("�����ļ�ʧ�ܣ�����·��%sʧ�ܡ�", file_path.c_str());
			break;
		}

		//�����ļ�
		LogInfo("���ڱ����ļ��������ĵȴ�����");

		std::list<std::string> filter_file_name;
		get_folder_files(source_path, filter_file_name, true);

		if (is_compress_backup_file) {
			std::string zip_filepath = file_path + "\\backup.zip";
#if ENABLE_LIBZIP
			if (zip_compress(destination_path, zip_filepath, std::string(), filter_file_suffix, progress_cb, args) != 0) {
				break;
			}
#else
			int ret = miniz_compress_folder(destination_path.c_str(), zip_filepath.c_str(), filter_file_name, progress_cb, args);

			if (ret != 0)
			{
				std::string error_string;
				switch (ret) {
				case -1:
					error_string = format_str("�����ļ�ʧ�ܣ�ѹ���ļ���·����Ч��");
					break;
				case -2:
					error_string = format_str("�����ļ�ʧ�ܣ�zip�ļ�·����Ч��");
					break;
				case -3:
					error_string = format_str("�����ļ�ʧ�ܣ���ȡѹ���ļ���Ч��");
					break;
				case -4:
					error_string = format_str("�����ļ�ʧ�ܣ��ڴ治�㣬�޷�����ѹ����");
					break;
				case -5:
					error_string = format_str("�����ļ�ʧ�ܣ�ѹ���ļ��ϲ����ڴ�ʧ�ܣ�");
					break;
				default:
					error_string = format_str("�����ļ�ʧ�ܣ�δ֪����");
					break;
				}
				LogError(error_string);
				break;
			}
#endif
		}
		else {
			std::string bakcup_filepath = file_path + "\\backup";
			if (!copy_folder(destination_path, bakcup_filepath, progress_cb, args)) {
				break;
			}
		}

		result = true;
	} while (false);

	if (result) {
		LogInfo("�����ļ���ɡ�");
	}
	return result;
}

bool delete_old_version_backup_file(const std::string& process_name, const std::string& latest_version)
{
	bool result = false, success = true;
	do 
	{
		char localappdata_path[8192] = { 0 };
		if (SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localappdata_path) != S_OK) {
			LogError("ɾ���ɰ汾�����ļ�ʧ�ܣ���ȡLOCALAPPDATAĿ¼ʧ�ܡ�");
			break;
		}

		std::string dir_path;
		size_t npos = process_name.find_last_of('.');
		if (npos != std::string::npos) {
			dir_path = format_str("%s\\imp_updater_backup\\%s", localappdata_path, process_name.substr(0, npos).c_str());
		}
		else {
			dir_path = format_str("%s\\imp_updater_backup\\%s", localappdata_path, process_name.c_str());
		}

		std::list<std::string> dirs;
		traversal_directory(dir_path, dirs, false);

		for (std::list<std::string>::const_iterator cit = dirs.begin(); cit != dirs.end(); ++cit) {
			size_t npos = cit->find_last_of('\\');
			if (npos == std::string::npos) {
				continue;
			}

			std::string str = cit->substr(npos + 1);
			std::vector<std::string> ver0 = split_by_delimiter<std::vector<std::string>>(latest_version, '.');
			std::vector<std::string> ver1 = split_by_delimiter<std::vector<std::string>>(str, '.');
			size_t max_size = std::max<size_t>(ver0.size(), ver1.size());
			while (ver0.size() < max_size) {
				ver0.push_back("0");
			}

			while( ver1.size() < max_size) {
				ver1.push_back("0");
			}

			int cmp = 0;
			for (size_t i = 0; i < ver0.size(); ++i) {
				if (std::atoi(ver0[i].c_str()) > std::atoi(ver1[i].c_str())) {
					cmp = 1;
					break;
				}

				if (std::atoi(ver0[i].c_str()) < std::atoi(ver1[i].c_str())) {
					cmp = -1;
					break;
				}
			}

			if (cmp != 1) {
				continue;
			}

			DWORD attrs = GetFileAttributes(cit->c_str());
			if (attrs == INVALID_FILE_ATTRIBUTES) {
				continue;
			}

			if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
				if (!remove_folder(*cit)) {
					success = false;
					break;
				}
			}
		}

		if (!success) {
			break;
		}

		result = true;
	} while (false);
	return result;
}

std::string match_upgrade_app_file_path(const std::string& process_name, const std::string& source_path,
										const std::string& destination_path)
{
	std::string file_path;
	std::list<std::string> file_list;
	find_file(destination_path.c_str(), ".exe", file_list, true);	
	for (std::list<std::string>::iterator iter = file_list.begin(); iter != file_list.end(); ++iter) {
		if ((*iter).find(process_name) != std::string::npos) {
			file_path = *iter;
			break;
		}
	}

	if (file_path.empty() && !file_list.empty()) {
		//���û��ͨ��process_nameƥ���ϣ���ô��ȥƥ��ѹ����������
		size_t npos = source_path.rfind("\\");
		if (npos != std::string::npos) {
			std::string dir_name = source_path.substr(npos + 1);
			for (std::list<std::string>::iterator iter = file_list.begin(); iter != file_list.end(); ++iter) {
				if ((*iter).find(dir_name) != std::string::npos) {
					file_path = *iter;
					LogWarn("����ѹ�����ڲ���û���ҵ����exe����ʹ��ѹ��������ƥ��ɹ���");
					break;
				}
			}
		}

		//���ͨ��ѹ������û���ҵ�����ô��Ĭ�ϵ�һ��Ϊ������exe����
		if (file_path.empty()) {
			file_path = file_list.front();
			LogWarn("����ѹ�����ڲ���û���ҵ����exe����Ĭ��ƥ���һ��exe����");
		}
	}

	if (file_path.empty()) {
		LogError("����ѹ�����ڲ���û���ҵ��κ�exe����");
	}
	return file_path;
}

bool rollback_file(const std::string& process_name, const std::string& latest_version,
				   const std::string& destination_path,
				   bool is_compress_backup_file)
{
	LogInfo("���ڽ��лع��ļ��������ĵȴ�����");
	bool result = false;
	do 
	{
		char localappdata_path[8192] = { 0 };
		if (SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localappdata_path) != S_OK) {
			LogError("�ع��ļ�ʧ�ܣ���ȡLOCALAPPDATAĿ¼ʧ�ܡ�");
			break;
		}

		std::string file_path;
		size_t npos = process_name.find_last_of('.');
		if (npos != std::string::npos) {
			file_path = format_str("%s\\imp_updater_backup\\%s\\%s", localappdata_path, 
				process_name.substr(0, npos).c_str(),
				latest_version.c_str());
		}
		else
		{
			file_path = format_str("%s\\imp_updater_backup\\%s\\%s", localappdata_path, 
				process_name.c_str(),
				latest_version.c_str());
		}

		if (!create_path(file_path)) {
			LogError("�ع��ļ�ʧ�ܣ�����·��%sʧ�ܡ�", file_path.c_str());
			break;
		}

		std::string extract_path = file_path;
		std::string rollback_filepath = extract_path + "\\backup.zip";
		extract_path += "\\backup";

		if (is_compress_backup_file) {
#if ENABLE_LIBZIP
			if (libzip_decompress(rollback_filepath, extract_path) != 0) {
				break;
			}
#else
			int ret = miniz_decompress_file(rollback_filepath.c_str(), extract_path.c_str());
			if (ret != 0)
			{
				std::string error_string;
				switch (ret) {
				case -1:
					error_string = "�ع��ļ�ʧ�ܣ�zip�ļ���Ч��";
					break;
				case -2:
					error_string = "�ع��ļ�ʧ�ܣ�������ȡ�ļ���ʧ�ܣ�";
					break;
				case -3:
					error_string = "�ع��ļ�ʧ�ܣ�д���ѹ�ļ�ʧ�ܣ�";
					break;
				case -4:
					error_string = "�ع��ļ�ʧ�ܣ���ȡ��ѹ�ļ���Ϣʧ�ܣ�";
					break;
				case -5:
					error_string = "�ع��ļ�ʧ�ܣ���ȡ��ѹ�ļ����ڴ�ʧ�ܣ�";
					break;
				default:
					error_string = "�ع��ļ�ʧ�ܣ�δ֪����";
					break;

				}
				LogError(error_string);
				break;
			}
#endif
		}
		
		//ֱ�ӿ�����ȥ
		if (!copy_folder(extract_path, destination_path)) {
			break;
		}

		remove_folder(extract_path);
		result = true;
	} while (false);
	if (result) {
		LogInfo("�ع��ļ��ɹ���");
	}
	return result;
}

bool start_process(const std::string& file_path, const std::string& command_line, const std::string& working_directory)
{
	bool result = false;
	do 
	{
		LogInfo("�����������̣������ĵȴ�����");
		SHELLEXECUTEINFOA sei = { sizeof(SHELLEXECUTEINFOA) };
		sei.lpVerb = "open";
		sei.lpFile = file_path.c_str();
		sei.lpParameters = command_line.empty() ? nullptr : command_line.c_str();
		//sei.lpDirectory = working_directory.empty() ? nullptr : working_directory.c_str();
		std::string dir_path;
		if (working_directory.empty()) {
			dir_path = file_path;
			replace_with_windows_path_symbol(dir_path);
			size_t npos = dir_path.find_last_of('\\');
			if (npos != std::string::npos) {
				dir_path = dir_path.substr(0, npos);
				sei.lpDirectory = dir_path.c_str();
			}
			else {
				sei.lpDirectory = nullptr;
			}
		}
		else {
			sei.lpDirectory = working_directory.c_str();
		}
		sei.nShow = SW_SHOW;
		sei.fMask = SEE_MASK_FLAG_NO_UI;
		BOOL ret = ShellExecuteEx(&sei);
		if (ret) {
			LogInfo("�������̳ɹ���");
		}
		else {
			DWORD error_code = GetLastError();
			if (error_code != ERROR_CANCELLED) {
				LogError("��������ʧ�ܣ�%s", format_error(error_code).c_str());
				break;
			}
		}

		//STARTUPINFOA startInfo = { sizeof(STARTUPINFOA) };
		//PROCESS_INFORMATION procStruct;
		//BOOL ret = CreateProcess(
		//	file_path.c_str(),
		//	command_line.empty() ? nullptr : (char*)command_line.c_str(),
		//	nullptr,
		//	nullptr,
		//	FALSE,
		//	NORMAL_PRIORITY_CLASS,
		//	nullptr,
		//	working_directory.empty() ? nullptr: working_directory.c_str(),
		//	&startInfo,
		//	&procStruct);
		//if (ret != TRUE && GetLastError() == ERROR_ELEVATION_REQUIRED) {
		//	//��Ҫ�õ�����ԱȨ��
		//	SHELLEXECUTEINFOA sei = { sizeof(SHELLEXECUTEINFOA) };
		//	sei.lpVerb = "runas";
		//	sei.lpFile = file_path.c_str();
		//	sei.lpParameters = command_line.empty() ? nullptr : command_line.c_str();
		//	sei.lpDirectory = working_directory.empty() ? nullptr : working_directory.c_str();
		//	sei.nShow = SW_SHOW;
		//	ret = ShellExecuteEx(&sei);
		//	if (ret) {
		//		LogInfo("�������̳ɹ���");
		//	}
		//	else {
		//		DWORD error_code = GetLastError();
		//		if (error_code != ERROR_CANCELLED) {
		//			LogInfo("��������ʧ�ܣ�%s", format_error(error_code).c_str());
		//			break;
		//		}
		//	}
		//}
		//else {
		//	if (ret) {
		//		/*if (procStruct.hProcess) {
		//			CloseHandle(procStruct.hProcess);
		//		}

		//		if (procStruct.hThread) {
		//			CloseHandle(procStruct.hThread);
		//		}*/
		//		LogInfo("�������̳ɹ���");
		//	}
		//	else {
		//		LogInfo("��������ʧ�ܣ�%s", format_error(GetLastError()).c_str());
		//		break;
		//	}
		//}
		result = true;
	} while (false);
	return result;
}

void update_finish(bool ret, bool is_hide_window, bool is_backup_file, bool is_copy_file_error,
				   const std::string& process_name,
				   const std::string& latest_version,
				   const std::string& destination_path, int window_style)
{
	if (ret) {
		if (!is_hide_window && cmdline_option.window_style == 0) {
			for (int i = 3; i >= 0; --i) {
				ChangeStatusText("���³ɹ���%d�������Զ��˳���", i);
				Sleep(1000);
			}
		}
	}
	else {
		ChangeStatusText("����ʧ�ܣ�");

		if (is_backup_file && is_copy_file_error) {
			std::string caption = format_str("����ԭ��%s\n�Ƿ�Ҫ�ع�����һ���汾��", get_last_error().c_str());
			int ret = IDYES;
			if (cmdline_option.is_prompt_if_error) {
				ret = MessageBox(nullptr, caption.c_str(), "����", MB_YESNO | MB_ICONQUESTION);
			}
			if (ret == IDYES) {
				LogInfo("���ڻع�����һ���汾�������ĵȴ�����");
				bool success = rollback_file(process_name, latest_version, destination_path,
					cmdline_option.is_compress_backup_file);
				if (success) {
					LogInfo("�ع�����һ���汾�ɹ���");
				}
				else {
					LogError("�ع�����һ���汾ʧ�ܣ�%s", get_last_error().c_str());
				}
				
				if (is_hide_window || window_style != 0) {
					if (cmdline_option.is_prompt_if_error) {
						MessageBox(nullptr, get_last_error().c_str(), "����", MB_ICONERROR | MB_OK);
					}
				}
			}
		}
		else {
			if (is_hide_window || window_style != 0) {
				if (cmdline_option.is_prompt_if_error) {
					MessageBox(nullptr, get_last_error().c_str(), "����", MB_ICONERROR | MB_OK);
				}
			}
		}
	}
}

typedef struct _thread_copy_file_args {
	_thread_copy_file_args(){
		current_progress = 0;
		current_size = 0;
		total_size = 0;
		rate = 0;
		filter_cb = nullptr;
	}

	size_t current_progress;
	size_t current_size;
	size_t total_size;
	int rate;
	filter_file_cb_t filter_cb;
	std::list<std::string> filter_args;
} thread_copy_file_args_t;

static bool thread_copy_file_cb(const std::string& filepath, void* args)
{
	thread_copy_file_args_t* ptr = static_cast<thread_copy_file_args_t*>(args);
	if (ptr->filter_cb && ptr->filter_cb(filepath, &ptr->filter_args)) {
		return true;
	}

	if (ptr->total_size > 0) {
		float precent = static_cast<float>(++ptr->current_size) / static_cast<float>(ptr->total_size) *  ptr->rate;
		uInstallProgress = static_cast<int>(ptr->current_progress + precent);
	}
	
	return false;
}

static unsigned long __stdcall update_thread(void* args)
{
	size_t start_time = GetTickCount();
	bool upgrade_result = false, is_copy_file_error = false;
	bIsUpgradeFinish = FALSE;
	do 
	{
		LogInfo("�����в�����");
		for (std::map<std::string, std::string>::iterator iter = cmdline_option.kv.begin();
			iter != cmdline_option.kv.end(); ++iter) {
				LogInfo("ѡ��[%s], ����[%s]��", iter->first.c_str(), iter->second.c_str());
		}

		std::string file_path;
		if (!cmdline_option.is_only_start_process) {
			ChangeStatusText("���ڸ���%s�������ĵȴ�����", cmdline_option.old_version_process_name.c_str());

			std::list<std::string> sensitive_dll_list = split_by_delimiter<std::list<std::string>>(cmdline_option.sensitive_dll, ';');
			for (std::list<std::string>::iterator iter = sensitive_dll_list.begin();
				iter != sensitive_dll_list.end(); ++iter) {
					sensitive_dll_set.insert(*iter);
			}

			if (find_str_set(sensitive_dll_set, "$no_default", true) == sensitive_dll_set.end()) {
				//��ЩDLL�ᾭ������
				for (int i = 0; often_intercepted_dll_name[i]; ++i) {
					std::set<std::string>::iterator iter = find_str_set(sensitive_dll_set, often_intercepted_dll_name[i], true);
					if (iter == sensitive_dll_set.end()) {
						sensitive_dll_set.insert(often_intercepted_dll_name[i]);
					}
				}
			}
			else {
				sensitive_dll_set.erase("$no_default");
			}
			
			//�ȴ������˳�
			if (cmdline_option.is_wait_old_version_exit) {
				wait_process_exit(cmdline_option.old_version_process_id,
					cmdline_option.old_version_process_name, 
					cmdline_option.kill_old_version_process_flag,
					cmdline_option.wait_old_version_exit_timeout);
			}

			if (!cmdline_option.is_hide_window) {
				PostMessage(hWnd, WM_USER_SHOW_WINDOW, 0, 0);
			}

			//���·���Ƿ���Ч
			if (!check_path_is_valid(cmdline_option.source_path, cmdline_option.destination_path)) {
				break;
			}

			//�������Ƿ��˳�
			if (!check_process_is_exit(cmdline_option.old_version_process_id,
				cmdline_option.old_version_process_name, 
				cmdline_option.kill_old_version_process_flag,
				cmdline_option.wait_old_version_exit_timeout)) {
				break;
			}
			uInstallProgress += 5;

			//����ļ��Ƿ�ռ��
			if (!check_file_is_occupy(cmdline_option.destination_path)) {
				break;
			}
			uInstallProgress += 5;
			
			thread_copy_file_args_t tcfa;
			
			//�����ļ�
			if (cmdline_option.is_backup_file) {
				std::list<std::string> filter_file_name;
				get_folder_files(cmdline_option.source_path, filter_file_name, true, nullptr, nullptr, true);
				tcfa.current_size = 0;
				tcfa.current_progress = uInstallProgress;
				tcfa.total_size = get_folder_file_number(cmdline_option.destination_path, &filter_file_name_cb, &filter_file_name);
				tcfa.rate = 40;
				tcfa.filter_cb = cmdline_option.is_compress_backup_file ? nullptr : &filter_file_name_cb;
				tcfa.filter_args = filter_file_name;
				if (!backup_file(cmdline_option.old_version_process_name,
					cmdline_option.latest_version,
					cmdline_option.source_path,
					cmdline_option.destination_path,
					cmdline_option.is_compress_backup_file,
					&thread_copy_file_cb, &tcfa)) {
					break;
				}
				uInstallProgress = 50;
				tcfa.filter_cb = nullptr;
			}
			else {
				tcfa.rate = 80;
			}

			tcfa.current_size = 0;
			tcfa.current_progress = uInstallProgress;
			tcfa.total_size = get_folder_file_number(cmdline_option.source_path);

			//�����ļ���
			LogInfo("���Ƹ����ļ��У������ĵȴ�����");
			bool success = copy_folder(cmdline_option.source_path, cmdline_option.destination_path, &thread_copy_file_cb, &tcfa);
			if (success) {
				LogInfo("���Ƹ����ļ��ɹ���");
			}
			else {
				LogError("���Ƹ����ļ�ʧ�ܡ�");
				is_copy_file_error = true;
				break;
			}

			uInstallProgress = 90;

			//ɾ�������ļ�
			if (cmdline_option.is_delete_update_file) {
				LogInfo("����ɾ�������ļ��������ĵȴ�����");
				bool success = remove_folder(cmdline_option.source_path);
				if (success) {
					LogInfo("ɾ�������ļ��ɹ���");
				}
				else {
					//���Դ���
					LogWarn("ɾ�������ļ�ʧ�ܣ����ֶ�ɾ����");
				}
			}

			//ɾ���ɰ汾�����ļ�
			if (cmdline_option.is_delete_old_version_backup_file) {
				LogInfo("����ɾ���ɰ汾�����ļ��������ĵȴ�����");
				bool success = delete_old_version_backup_file(cmdline_option.old_version_process_name, cmdline_option.latest_version);
				if (success) {
					LogInfo("ɾ���ɰ汾�����ļ��ɹ���");
				}
				else {
					LogWarn("ɾ���ɰ汾�����ļ�ʧ�ܣ����ֶ�ɾ����");
				}
			}

			uInstallProgress += 5;
			//ƥ��������Ӧ�ó���
			cmdline_option.start_process_file_path = match_upgrade_app_file_path(cmdline_option.old_version_process_name,
				cmdline_option.source_path, cmdline_option.destination_path);

			if (cmdline_option.start_process_file_path.empty()) {
				LogError("�����ļ����ڲ���û���ҵ�������");
				break;
			}

			//��ݷ�ʽ����
			if (cmdline_option.is_create_shortcut) {
				LogInfo("���ڴ�����ݷ�ʽ�������ĵȴ�����");
				success = create_shortcut(cmdline_option.start_process_file_path, cmdline_option.is_recursion_replace_shortcut);
				
				if (success) {
					LogInfo("������ݷ�ʽ�ɹ���");
				}
				else {
					LogError("������ݷ�ʽʧ�ܣ�");
					break;
				}
			}

			uInstallProgress += 4;

			LogInfo("������ɣ�");
		}
		else {
			ChangeStatusText("�������������У������ĵȴ�����");
			//�ȴ������˳�
			if (cmdline_option.is_wait_old_version_exit) {
				wait_process_exit(cmdline_option.old_version_process_id,
					cmdline_option.old_version_process_name,
					cmdline_option.kill_old_version_process_flag,
					cmdline_option.wait_old_version_exit_timeout);

				check_process_is_exit(cmdline_option.old_version_process_id,
					cmdline_option.old_version_process_name,
					cmdline_option.kill_old_version_process_flag,
					cmdline_option.wait_old_version_exit_timeout);
			}
		}
		
		//��������
		if (!start_process(cmdline_option.start_process_file_path, 
			cmdline_option.start_process_command_line,
			cmdline_option.start_process_working_directory)) {
			break;
		}

		uInstallProgress += 1;
		upgrade_result = true;
	} while (false);
	bIsUpgradeFinish = TRUE;

	int elapsed_time = static_cast<int>(static_cast<float>(GetTickCount() - start_time) / 1000);
	if (!cmdline_option.is_only_start_process) {
		LogInfo("������ʱ%d�롣", elapsed_time);
		update_finish(upgrade_result, cmdline_option.is_hide_window,
			cmdline_option.is_backup_file, is_copy_file_error, 
			cmdline_option.old_version_process_name,
			cmdline_option.latest_version,
			cmdline_option.destination_path,
			cmdline_option.window_style);
	}
	else {
		if (!upgrade_result) {
			if (cmdline_option.is_prompt_if_error) {
				MessageBox(nullptr, get_last_error().c_str(), "����", MB_ICONERROR | MB_OK);
			}
		}
	}

	if (!cmdline_option.is_only_start_process) {
		//���ص�����
		SendMessage(hWnd, WM_USER_HIDE_WINDOW, 0, 0);
		upload_update_record(upgrade_result, elapsed_time);
	}
	
	PostMessage(hWnd, WM_CLOSE, 0, 0);
	return 0;
}

void update_app() 
{
	HANDLE handle = CreateThread(nullptr, 0, update_thread, nullptr, 0, nullptr);
	if (handle != INVALID_HANDLE_VALUE) {
		CloseHandle(handle);
	}
}

bool create_path(const std::string& folder)
{
	std::string folder_builder;
	std::string sub;
	sub.reserve(folder.size());
	for (std::string::const_iterator it = folder.begin(); it != folder.end(); ++it) {
		//cout << *(folder.end()-1) << endl;
		const char c = *it;
		sub.push_back(c);
		if (c == '\\' || it == folder.end() - 1) {
			folder_builder.append(sub);
			if (0 != ::_access(folder_builder.c_str(), 0)) {
				// this folder not exist
				if (0 != ::_mkdir(folder_builder.c_str())) {
					// create failed
					return false;
				}
			}
			sub.clear();
		}
	}
	return true;
}

std::map<DWORD, std::string> get_occupied_file_pids(const std::string& file_path)
{
	std::map<DWORD, std::string> ret;
	do 
	{
		DWORD dwSession = 0;
		WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
		DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);
		if (dwError != ERROR_SUCCESS) {
			break;
		}

		const wchar_t* pszFile = multiByteToWideChar(file_path.c_str(), CP_ACP);
		dwError = RmRegisterResources(dwSession, 1, &pszFile, 0, NULL, 0, NULL);
		if (dwError != ERROR_SUCCESS) {
			delete[] pszFile;
			RmEndSession(dwSession);
			break;
		}
		delete[] pszFile;

		UINT nProcInfoNeeded = 0;
		UINT nProcInfo = 0;
		DWORD dwReason;
		dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, nullptr, &dwReason);
		if (dwError != ERROR_MORE_DATA) {
			RmEndSession(dwSession);
			break;
		}

		nProcInfo = nProcInfoNeeded;
		RM_PROCESS_INFO* rgpi = new RM_PROCESS_INFO[nProcInfo];
		memset(rgpi, 0, sizeof(rgpi) * nProcInfo);
		dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi, &dwReason);
		if (dwError != ERROR_SUCCESS) {
			delete[] rgpi;
			RmEndSession(dwSession);
			break;
		}

		for (UINT i = 0; i < nProcInfo; i++) {
			DWORD pid = rgpi[i].Process.dwProcessId;
			char* str = wideCharToMultiByte(rgpi[i].strAppName, CP_ACP);
			std::string app(str);
			delete[] str;
			ret.insert(std::make_pair(pid, app));
		}

		delete[] rgpi;
		RmEndSession(dwSession);
	} while (false);
	return ret;
}

bool terminate_process(DWORD pid)
{
	bool result = false;
	do 
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, pid);
		if (hProcess == nullptr) {
			LogError("OpenProcess����ʧ�ܣ�������룺%lu��", GetLastError());
			break;
		}

		if (!TerminateProcess(hProcess, 0)) {
			CloseHandle(hProcess);
			LogError("TerminateProcess����ʧ�ܣ�������룺%lu��", GetLastError());
			break;
		}

		DWORD object = WaitForSingleObject(hProcess, 30000);
		if (object != WAIT_OBJECT_0) {
			CloseHandle(hProcess);
			LogError("WaitForSingleObject����ʧ�ܣ�������룺%lu��", GetLastError());
			break;
		}
		CloseHandle(hProcess);
		result = true;
	} while (false);
	return result;
}

std::string get_host_name()
{
	std::string host_name("unknown");
	//bool clear_wsa = false;
	do 
	{
		/*SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		if (s == INVALID_SOCKET && WSAGetLastError() == WSANOTINITIALISED) {
			WSADATA wsaData;
			int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (result != 0) {
				break;
			}
			clear_wsa = true;
		}*/

		char hostname[256] = { 0 };
		if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
			break;
		}
		host_name = hostname;
	} while (false);
	
	/*if (clear_wsa) {
		WSACleanup();
	}*/
	return host_name;
}

wsa_lock::wsa_lock() :socket_(INVALID_SOCKET), init_(false)
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET && WSAGetLastError() == WSANOTINITIALISED) {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
			init_ = true;
		}
	}
}

wsa_lock::~wsa_lock()
{
	if (init_) {
		WSACleanup();
	}
}

static wsa_lock global_wsa_lock;

bool get_ipaddr(const std::string& domain, std::string& addr)
{
	bool result = false;
	do 
	{
		/*WSADATA wsaData;
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0) {
			break;
		}*/

		struct addrinfo hints = { 0 };
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		struct addrinfo* res = nullptr;
		if (getaddrinfo(domain.c_str(), nullptr, &hints, &res) != 0) {
			break;
		}

		for (struct addrinfo* ptr = res; ptr != nullptr; ptr = ptr->ai_next) {
			char ipStr[INET_ADDRSTRLEN] = { 0 };
			struct sockaddr_in* sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in*>(ptr->ai_addr);
			inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), ipStr, INET_ADDRSTRLEN);
			addr = ipStr;
			break;
		}

		freeaddrinfo(res);
		/*WSACleanup();*/
		result = true;
	} while (false);
	return result;
}

bool is_valid_ipaddr(const std::string& ip)
{
	// ������¼ÿ�����ֵ�ֵ
	int segments = 0;
	int segmentValue = 0;
	bool hasDigits = false;

	for (size_t i = 0; i < ip.length(); ++i) {
		char c = ip[i];

		if (std::isdigit(c)) {
			// �ۼ����ֲ��ֵ�ֵ
			segmentValue = segmentValue * 10 + (c - '0');
			if (segmentValue > 255) {
				return false; // ÿ��ֵӦ�� 0-255 ֮��
			}
			hasDigits = true;
		}
		else if (c == '.') {
			// �����ֶη��ţ���鵱ǰ���Ƿ�Ϸ�
			if (!hasDigits) {
				return false; // ȷ������������
			}
			++segments;
			if (segments > 3) {
				return false; // ���ֻ���� 3 ����
			}
			segmentValue = 0; // ���õ�ǰ��ֵ
			hasDigits = false; // �����Ƿ������ֵı�־
		}
		else {
			return false; // �Ƿ��ַ�
		}
	}

	// ������һ����
	return hasDigits && segments == 3;
}

std::vector<std::string> get_localhost_addr() {
	std::vector<std::string> ips;
	ULONG outBufLen = 0;
	DWORD dwRetVal = 0;
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	if (GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
		pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
	}

	if (pAddresses) {
		dwRetVal = GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &outBufLen);
		if (dwRetVal == NO_ERROR) {
			PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
			while (pCurrAddresses != NULL) {
				if (pCurrAddresses->IfType == IF_TYPE_ETHERNET_CSMACD ||
					pCurrAddresses->IfType == IF_TYPE_IEEE80211) {
						std::string adapterName = pCurrAddresses->AdapterName;
						std::wstring friendlyName = pCurrAddresses->FriendlyName;

						if (adapterName.find("VMware") != std::string::npos ||
							adapterName.find("VirtualBox") != std::string::npos ||
							friendlyName.find(L"VMware") != std::wstring::npos ||
							friendlyName.find(L"VirtualBox") != std::wstring::npos) {
							pCurrAddresses = pCurrAddresses->Next;
							continue;
						}

						for (PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress;
							pUnicast != NULL;
							pUnicast = pUnicast->Next) {
								struct sockaddr_in* sa_in = (struct sockaddr_in*)pUnicast->Address.lpSockaddr;
								std::string addr = inet_ntoa(sa_in->sin_addr);
								if (addr.find("169.254") != std::string::npos) {
									continue;
								}
								ips.push_back(addr);
						}
				}
				pCurrAddresses = pCurrAddresses->Next;
			}
		}
		free(pAddresses);
	}

	return ips;
}

bool get_guid(std::string& guid) {
	bool result = false;
	HKEY hKey = nullptr;
	do 
	{
		CHAR szGuid[256] = { 0 };
		DWORD dwSize = sizeof(szGuid);
		DWORD dwType = REG_SZ;

		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography",
			0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS) {
				break;
		}

		if (RegQueryValueExA(hKey, "MachineGuid", NULL, &dwType, (LPBYTE)szGuid, &dwSize) != ERROR_SUCCESS) {
			break;
		}
		guid = szGuid;
		result = true;
	} while (false);

	if (hKey) {
		RegCloseKey(hKey);
	}
	return result;
}

bool get_cpuid(std::string& cpuid) {
	cpuid.clear();
	HRESULT hres;

	// ��ʼ�� COM
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) {
		return false;
	}

	// ���ð�ȫ����
	hres = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL);

	if (FAILED(hres)) {
		CoUninitialize();
		return false;
	}

	IWbemLocator* pLoc = NULL;
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID*)&pLoc);

	if (FAILED(hres)) {
		CoUninitialize();
		return false;
	}

	IWbemServices* pSvc = NULL;
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&pSvc);

	if (FAILED(hres)) {
		pLoc->Release();
		CoUninitialize();
		return false;
	}

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT ProcessorId FROM Win32_Processor"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres)) {
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return false;
	}

	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (0 == uReturn) {
			break;
		}

		VARIANT vtProp;
		hr = pclsObj->Get(L"ProcessorId", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr)) {
			cpuid = _bstr_t(vtProp.bstrVal);
			VariantClear(&vtProp);
		}

		pclsObj->Release();
	}

	pEnumerator->Release();
	pSvc->Release();
	pLoc->Release();
	CoUninitialize();

	return !cpuid.empty();
}

std::string get_uuid()
{
	std::string uuid;
	if (!get_guid(uuid)) {
		if (!get_cpuid(uuid)) {
			uuid = "undefined";
		}
	}
	return uuid;
}

#include "picojson.h"
#include "base64.h"

static size_t curl_write_body_cb(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t total = size * nmemb;
	std::string* body = static_cast<std::string*>(userp);
	body->append(static_cast<char*>(contents), total);
	return total;
}

bool upload_update_record(bool success, int elapsed_time)
{
	bool result = false;
	//curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL* curl = nullptr;
	struct curl_slist* headers = nullptr;
	do 
	{	
		//"�ϴ����¼�¼��־��0�����ϴ���1��������������־��2�����ʧ�ܰ���������־��3������������־��",
		if (cmdline_option.upload_update_record_flag == 0) {
			result = true;
			break;
		}

		std::string update_log;
		if ((cmdline_option.upload_update_record_flag == 2 && !success) ||
			cmdline_option.upload_update_record_flag == 3) {
			update_log = base64_encode(ansi_to_utf8(log_filetext));
		}

		curl = curl_easy_init();
		if (!curl) {
			write_log_type_fmt("error", "curl��ʼ��ʧ�ܡ�");
			break;
		}

		picojson::object obj;
		std::vector<std::string> addr = get_localhost_addr();
		std::vector<picojson::value> ip_address;
		for (size_t i = 0; i < addr.size(); ++i) {
			ip_address.push_back(picojson::value(addr[i].c_str()));
		}

		time_t timestamp = get_time_stamp_ms();
		std::string uuid = ansi_to_utf8(get_uuid());
		std::string app_name = ansi_to_utf8(cmdline_option.old_version_process_name);
		std::string app_version = ansi_to_utf8(cmdline_option.latest_version);
		std::string app_secret = utility::sha256(uuid + app_name + app_version + format_str("%lld", ((timestamp + elapsed_time) >> 1)));
		std::string host_name = ansi_to_utf8(get_host_name());
		obj["method"] = picojson::value("upload_update_record");
		obj["uuid"] = picojson::value(uuid);
		obj["app_name"] = picojson::value(app_name);
		obj["app_version"] = picojson::value(app_version);
		obj["app_secret"] = picojson::value(app_secret);
		obj["ip_address"] = picojson::value(ip_address);
		obj["host_name"] = picojson::value(host_name);
		obj["update_result"] = picojson::value(success);
		obj["update_log"] = picojson::value(update_log);
		obj["elapsed_time"] = picojson::value(static_cast<double>(elapsed_time));
		obj["timestamp"] = picojson::value(static_cast<double>(timestamp));

		std::string json_str = picojson::value(obj).serialize();
		std::string req_body;
		if (!deflate_compress(json_str.c_str(), json_str.size(), req_body)) {
			write_log_type_fmt("error", "deflateѹ��������ʧ�ܡ�");
			break;
		}

		char url[1024] = { 0 };
		sprintf(url, "https://%s:%d/post_request", cmdline_option.server_address.c_str(), cmdline_option.server_port);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req_body.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, req_body.size());

		std::string content_length = format_str("Content-Length: %lu", req_body.length());
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Content-Encoding: deflate");
		headers = curl_slist_append(headers, content_length.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

		std::string resp_body;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curl_write_body_cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp_body);

		// ִ������
		CURLcode code = curl_easy_perform(curl);
		if (code != CURLE_OK) {
			write_log_type_fmt("error", "�ϴ����¼�¼ʧ�ܣ�%s��", curl_easy_strerror(code));
			break;
		}

		long status_code = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

		if (status_code != 200) {
			write_log_type_fmt("error", "�ϴ����¼�¼ʧ�ܣ���������Ӧ�쳣״̬��%d��", status_code);
			break;
		}

		picojson::value value;
		picojson::parse(value, resp_body);
		obj = value.get<picojson::object>();

		int error_code =static_cast<int>(obj["error_code"].get<double>());
		std::string error_string = obj["error_string"].get<std::string>();
		error_string = utf8_to_ansi(error_string);

		if (error_code == 0) {
			write_log_type_fmt("info", "�ϴ����¼�¼�ɹ���");
		}
		else {
			write_log_type_fmt("error", "�ϴ����¼�¼ʧ�ܣ�%s��", error_string.c_str());
			break;
		}

		result = true;
	} while (false);

	if (headers) {
		curl_slist_free_all(headers);
	}

	if (curl) {
		curl_easy_cleanup(curl);
	}

	//curl_global_cleanup();
	return result;
}

void kill_occupied_file_process(const std::string& process)
{
	std::map<DWORD, std::string> pids = get_occupied_file_pids(process);
	for (std::map<DWORD, std::string>::iterator it = pids.begin(); it != pids.end(); ++it) {
		bool ret = terminate_process(it->first);
		if (ret) {
			LogInfo("�ر�ռ��%s�ļ����̳ɹ���ռ�ý��̣�%s��PID��%lu��", process.c_str(), it->second.c_str(), it->first);
		}
		else {
			LogError("�ر�ռ��%s�ļ�����ʧ�ܣ�ռ�ý��̣�%s��PID��%lu��������룺%lu��", 
				process.c_str(), it->second.c_str(), it->first, GetLastError());
		}
	}
}

bool deflate_compress(const char* data, std::size_t size, std::string& compress_data)
{
	z_stream zs = { 0 };

	if (deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
		return false;
	}

	zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data));
	zs.avail_in = size;

	int ret;
	char outbuffer[4096] = { 0 };

	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = ::deflate(&zs, Z_FINISH);

		if (compress_data.size() < zs.total_out) {
			compress_data.append(outbuffer, zs.total_out - compress_data.size());
		}
	} while (ret == Z_OK);

	deflateEnd(&zs);

	return ret == Z_STREAM_END;
}

std::string get_file_exstension(const std::string& file_name)
{
	size_t dotPos = file_name.find_last_of(".");
	if (dotPos != std::string::npos) {
		return file_name.substr(dotPos);
	}
	return "";
}