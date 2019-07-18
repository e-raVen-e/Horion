#include "TestCommand.h"



TestCommand::TestCommand() : ICommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
}

bool TestCommand::execute(std::vector<std::string>* args)
{
	HANDLE hMapFile;
	LPCTSTR pBuf;
	HANDLE hFile;

	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return FALSE;

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, L"SeCreateGlobalPrivilege",
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	if (GetLastError() != ERROR_SUCCESS) {
		logF("AdjustTokenPrivileges eroprr %d", GetLastError());
	}

	hFile = CreateFile(TEXT("C:\\Users\\gbaker\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\yeet.txt"), // open One.txt
		GENERIC_ALL,             // open for reading
		0,                        // do not share
		NULL,                     // no security
		OPEN_EXISTING,            // existing file only
		FILE_ATTRIBUTE_NORMAL,    // normal file
		NULL);

	logF("file handle: %llX, %d", hFile, GetLastError());

	hMapFile = CreateFileMappingA(
		hFile,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		0xF,                // maximum object size (low-order DWORD)
		"Global\\yeetboii");                 // name of mapping object

	if (hMapFile == NULL)
	{
		logF(("Could not create file mapping object (%d).\n"),
			GetLastError());
		return 1;
	}
	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		0xF);

	if (pBuf == NULL)
	{
		logF(("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}


	CopyMemory((PVOID)pBuf, "helo", 5);

	__debugbreak();

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);
	CloseHandle(hFile);
	
	return true;
}
