// WinDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "Win.h"
#include "WinDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WinDlg dialog


WinDlg::WinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(WinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(WinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void WinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WinDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(WinDlg, CDialog)
	//{{AFX_MSG_MAP(WinDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WinDlg message handlers
