#pragma once
#include<windows.h>
#include<stdio.h>
#define DATA_SIZE 27136


typedef struct
{
	DWORD size;
	PBYTE ptrbuffer;
}FILEINFO;
FILEINFO Openfile(char* ptrFile)
{
	DWORD dwReadTotal = 0;
	DWORD dwReaded = 0;
	FILEINFO fileinfo = { 0 };
	HANDLE hFile = NULL;
	hFile = CreateFileA(ptrFile,
		FILE_SHARE_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		goto END;

	fileinfo.size = GetFileSize(hFile, NULL);
	if (fileinfo.size == 0)
		goto END;

	fileinfo.ptrbuffer = (byte*)HeapAlloc(GetProcessHeap(), 0, fileinfo.size);
	if (fileinfo.ptrbuffer == 0)
		goto END;
	SecureZeroMemory(fileinfo.ptrbuffer, fileinfo.size);

	while (dwReadTotal < fileinfo.size
		&& ReadFile(hFile, fileinfo.ptrbuffer + dwReadTotal, fileinfo.size - dwReadTotal, &dwReaded, NULL))
	{
		dwReadTotal += dwReaded;
	}
END:
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	return fileinfo;
};
void* __cdecl Mymemcpy(void* dest,
	const void* src,
	size_t 	count
)
{
	char* char_dest = (char*)dest;
	char* char_src = (char*)src;

	if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
	{
		/*  non-overlapping buffers */
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest++;
			char_src++;
			count--;
		}
	}
	else
	{
		/* overlaping buffers */
		char_dest = (char*)dest + count - 1;
		char_src = (char*)src + count - 1;

		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest--;
			char_src--;
			count--;
		}
	}

	return dest;
}
BOOL Write2file(PBYTE file, DWORD contentLen, PCHAR path)//д���ļ�������ͨ��
{

	HANDLE pFile;
	PBYTE tmpBuf = nullptr;
	DWORD dwBytesWrite, dwBytesToWrite;

	pFile = CreateFileA(path, GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,        //���Ǵ����ļ�
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (pFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(pFile);
		HeapFree(GetProcessHeap(), 0, file);
		return FALSE;
	}

	dwBytesToWrite = contentLen;
	dwBytesWrite = 0;

	tmpBuf = file;

	do {                                       //ѭ��д�ļ���ȷ���������ļ���д��  

		WriteFile(pFile, tmpBuf, dwBytesToWrite, &dwBytesWrite, NULL);

		dwBytesToWrite -= dwBytesWrite;
		tmpBuf += dwBytesWrite;

	} while (dwBytesToWrite > 0);

	CloseHandle(pFile);
	HeapFree(GetProcessHeap(), 0, file);
	return TRUE;
}