#include "globals.h"
#include "ConsoleColor.h"

#include <iostream>
#include <Psapi.h>
#include <string>
#include <thread>
#include <TlHelp32.h>
#include "Game/Game.h"
#include "Game/xorstr.h"
using namespace std;

std::uint32_t find_process_by_id(const std::string& name)
{
	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32 proc_entry{};
	proc_entry.dwSize = sizeof proc_entry;

	auto found_process = false;
	if (!!Process32First(snap, &proc_entry)) {
		do {
			if (name == proc_entry.szExeFile) {
				found_process = true;
				break;
			}
		} while (!!Process32Next(snap, &proc_entry));
	}

	CloseHandle(snap);
	return found_process
		? proc_entry.th32ProcessID
		: 0;
}

uintptr_t GetProcessBaseAddress(HANDLE Handle)
{
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(Handle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(Handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				std::string wstrModName = szModName;
				std::string wstrModContain = xorstr_("RainbowSix.exe");
				if (wstrModName.find(wstrModContain) != std::string::npos)
				{
					base_address = (uintptr_t)hMods[i];
					return base_address;
				}
			}
		}
	}

	return false;
}

void update()
{
	std::cout << "\b" << bCaveiraESPStatus;
	return;
}

int main()
{
	SetConsoleTitle("Caveira ESP [unknowncheats.me/forum/members/3073613.html]");

	{
		Sleep(1500);
		pid = find_process_by_id(xorstr_("RainbowSix.exe"));
		if (!pid)
		{
			cout << red << (xorstr_("[-] Could not find RainbowSix.exe"));
			cin.get();
			exit(0);
		}

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
		if (hProcess == INVALID_HANDLE_VALUE || !hProcess)
		{
			cout << red << (xorstr_("[-] Could not get process"));
			cin.get();
			exit(0);
		}

		base_address = GetProcessBaseAddress(hProcess);
		if (!base_address)
		{
			cout << red << (xorstr_("[-] Could not find base address"));
			cin.get();
			exit(0);
		}
	}


	cout << yellow << (xorstr_("[!] RainbowSix.exe find with success! \n"));
	std::cout << blue <<  xorstr_("[F9]") << xorstr_(" Caveira ESP => ") << bCaveiraESPStatus;

	while (true)
	{
		RainbowSix::esp();

		if (GetAsyncKeyState(VK_F9) & 1)
		{
			bCaveiraESPStatus = !bCaveiraESPStatus;
			update();
		}

		if (GetAsyncKeyState(VK_END))
		{
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}