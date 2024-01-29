#include <windows.h>
#include <stdio.h>

static void PrintLastError();

int main()
{
	SYSTEM_INFO si;
	::GetNativeSystemInfo(&si);

	printf("Number of logical processors: %d\n", si.dwNumberOfProcessors);
	printf("Page size %d Bytes\n", si.dwPageSize);
	printf("Processor mask: 0x%p\n", (PVOID)si.dwActiveProcessorMask);
	printf("Minimum process address: 0x%p\n", si.lpMinimumApplicationAddress);
	printf("Maximum process address: 0x%p\n", si.lpMaximumApplicationAddress);

	WCHAR path[MAX_PATH];
	UINT written = ::GetSystemDirectory(path, MAX_PATH);
	printf("System directory written=%d: %ws\n", written, path);

	DWORD size = MAX_COMPUTERNAME_LENGTH;
	WCHAR name[MAX_COMPUTERNAME_LENGTH];
	::GetComputerName(name, &size);
	printf("Computer name=%ws\n", name);

	WCHAR windowsDir[MAX_PATH];
	UINT getWinDirErr = ::GetWindowsDirectory(windowsDir, MAX_PATH);
	if (getWinDirErr == 0) {
		printf("Failed to get Windows directory\n");
		return 1;
	}
	printf("Windows dir=%ws\n", windowsDir);

	LARGE_INTEGER performanceCounter;
	BOOL qPCResult = ::QueryPerformanceCounter(&performanceCounter);
	if (!qPCResult) {
		printf("Failed to get QPC\n");
		return 2;
	}
	printf("Query Performance Counter=%llu\n", performanceCounter.QuadPart);

	DWORD prodInfo;
	BOOL getProdInfoResult = ::GetProductInfo(6, 3, 0, 0, &prodInfo);
	if (!getProdInfoResult) {
		PrintLastError();
		return 3;
	}

	printf("Product info: 0x%p\n", (PVOID)&prodInfo);

	return 0;
}

static void PrintLastError()
{
	DWORD lastError = ::GetLastError();
	LPVOID lpMsgBuf;
	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		lastError,
		0,
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);
	printf("Last error (code=%d): %ws\n", lastError, (LPTSTR)lpMsgBuf);
}
