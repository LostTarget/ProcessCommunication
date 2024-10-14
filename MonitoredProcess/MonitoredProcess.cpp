// MonitoredProcess.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "MonitoredProcess.h"
#include "Tools.h"

//窗口控件句柄
HWND msgBoxTitle;
HWND msgBoxContent;
HWND filePath;
HWND fileContent;
HWND processPath;


BOOL CALLBACK DialogProc(
	HWND hwndDlg,  // handle to dialog box				
	UINT uMsg,     // message				
	WPARAM wParam, // first message parameter				
	LPARAM lParam  // second message parameter				
)
{
	TCHAR szBuffer[256];
	HICON hBigIcon;
	HICON hSmallIcon;
	switch (uMsg)
	{
	case WM_SYSCOMMAND: // 使默认的最大化，最小化，关闭按钮生效
		switch (LOWORD(wParam))
		{
		case SC_CLOSE:
			DestroyWindow(hwndDlg); // 这里点击对话框关闭的x后关闭对话框
			break;
		}
		break;
	case  WM_INITDIALOG:
		// 获取对应的窗口句柄
		msgBoxTitle = GetDlgItem(hwndDlg, IDC_EDIT_MSGBOX_TITLE);
		msgBoxContent = GetDlgItem(hwndDlg, IDC_EDIT_MSGBOX_CONTENT);
		filePath = GetDlgItem(hwndDlg, IDC_EDIT_FILE_PATH);
		fileContent = GetDlgItem(hwndDlg, IDC_EDIT_FILE_CONTENT);
		processPath = GetDlgItem(hwndDlg, IDC_EDIT_PROCESS_PATH);

		return TRUE;

	case  WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_FILE_CREATE_TEST: // 测试创建文件
		{
			Tools tools;
			TCHAR path[MAX_PATH]{ 0 }; 
			GetWindowText(filePath, path, MAX_PATH);
			TCHAR content[MAX_PATH]{ 0 }; 
			GetWindowText(fileContent, content, MAX_PATH); 

			LPSTR pathStr = NULL; 

			tools.TCHARToChar(path, &pathStr); 

			HANDLE hFile = CreateFileA(pathStr, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			LPSTR contentStr = NULL; 
			tools.TCHARToChar(content, &contentStr); 
			// 设置文件指针
			SetFilePointer(hFile, 0, 0, FILE_END);

			if (!WriteFile(hFile, contentStr, strlen(contentStr), NULL, NULL))
			{
				printf("写出失败");
			}

			CloseHandle(hFile); 
			free(contentStr); 
			free(pathStr); 
			return TRUE;
		}

		case IDC_BUTTON_OPEN_PROCESS_TEST: // 测试打开进程
		{
			// 创建进程的信息
			STARTUPINFO si = { 0 };
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi{ 0 };
			TCHAR commandLine[MAX_PATH]{ 0 }; 
			GetWindowText(processPath, commandLine, MAX_PATH);

			if (CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				//MessageBox(NULL, TEXT("Succeessed!"), TEXT("Tips"), MB_OK); ?
				// 打开进程
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId); 

				CloseHandle(hProcess); 
			}
			return TRUE;
		}

		case IDC_BUTTON_MSGBOX_TEST: // 测试弹窗
		{
			TCHAR title[MAX_PATH]{ 0 };
			GetWindowText(msgBoxTitle, title, MAX_PATH);
			TCHAR content[MAX_PATH]{ 0 };
			GetWindowText(msgBoxContent, content, MAX_PATH);

			Tools tool;
			LPSTR titleStr = NULL;
			LPSTR contentStr = NULL;
			tool.TCHARToChar(title, &titleStr);
			tool.TCHARToChar(content, &contentStr);

			//测试消息弹窗
			MessageBoxA(hwndDlg, contentStr, titleStr, MB_OK);

			free(titleStr);
			free(contentStr);
			return TRUE;
		}
		}
		break;

	case WM_NOTIFY:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return FALSE;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	DialogBox(hInstance, (TCHAR*)IDD_DIALOG_MAIN, NULL, DialogProc);
    return 0;
}

