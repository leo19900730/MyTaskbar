// ListCtrlEx.cpp : 实现文件
//

#include "../stdafx.h"
//#include "../MyTaskbar.h"
#include "ListCtrlEx.h"

#include "Utility.h"
#include "Setting.h"

#include "../RenameDlg.h"
//#include "MyTaskbarDlg.h"


// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

	CListCtrlEx::CListCtrlEx()
{
	m_bDragging=false;
	m_bRightButton=false;
	//m_pParentWnd=NULL;

}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_WM_DROPFILES()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(NM_CLICK, &CListCtrlEx::OnNMClick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CListCtrlEx::OnLvnItemchanged)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CListCtrlEx::OnLvnBegindrag)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_RENAME, &CListCtrlEx::OnRename)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CListCtrlEx::OnNMRClick)
END_MESSAGE_MAP()



// CListCtrlEx 消息处理程序

void CListCtrlEx::InitListCtrl()
{

	//HWND hwnd=this->GetParent()->GetSafeHwnd();	
	//m_pParentWnd=CWnd::FromHandle(hwnd);


	m_nItem=-1;

	EnableToolTips(TRUE);
	m_toolTip.Create(this);

	/////////////////////////// CListCtrl  //////////////////////////////////
	this->SetExtendedStyle(LVS_EX_SUBITEMIMAGES);

	//设置控件背景色
	//m_ListCtrl.SetBkColor(CLR_NONE); 
	SetTextBkColor(CLR_NONE);
	SetBkColor(GetSysColor(COLOR_3DFACE)); //设置与窗口背景色相同

	this->SetIconSpacing(50,50);//图标间隔

	m_ImgList.Create(GetSystemMetrics(SM_CXICON),GetSystemMetrics(SM_CYICON),ILC_COLOR24,32,32);    
	this->SetImageList(&m_ImgList,LVSIL_NORMAL); 
	this->SetImageList(&m_ImgList,LVSIL_NORMAL);  //图片

	m_vecLinkList.clear();
	if(!GetFileList(m_vecLinkList)) return ;
	UpdateList(m_vecLinkList);
}


//接收拖入的文件
void CListCtrlEx::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	WCHAR szFilePathName[MAX_PATH]= {0};
	WCHAR szFilePath[MAX_PATH]= {0};

	CString sName;

	//得到文件个数      
	UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);   

	for (UINT nIndex=0 ; nIndex< nNumOfFiles; ++nIndex) 
	{   
		//　得到文件名   
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)szFilePathName, _MAX_PATH);   

		//如果是快捷方式
		CString sExname=CUtility::GetInstance()->GetExtendName(szFilePathName);
		if(sExname.MakeUpper()==L"LNK")
		{			
			CUtility::GetInstance()->GetPathForLink(szFilePathName,szFilePath);
			memset(szFilePathName,0,MAX_PATH);
			wcscpy(szFilePathName,szFilePath);		
		}

		//判断获得的路径是否不存在
		if(!CUtility::GetInstance()->PathIsExist(szFilePathName)) 
		{
			MessageBox(L"未能从快捷方式中解析出有效路径！");
			return;
		}

		int nCnt = this->GetItemCount();
		if(m_nItem!=-1) nCnt=m_nItem+nIndex+1;

		LinkInfo linkInfo(szFilePathName,CUtility::GetInstance()->GetPathName(szFilePathName));
		m_vecLinkList.insert(m_vecLinkList.begin()+nCnt,linkInfo);
		UpdateList(m_vecLinkList);

		//将修改的vec保存到文件
		UpdateLnkFile(m_vecLinkList);
	}      

	//完成拖放操作
	DragFinish(hDropInfo);     

	CListCtrl::OnDropFiles(hDropInfo);
}

//开始拖动
void CListCtrlEx::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_nDragIndex = pNMLV->iItem;
	/*TRACE("m_nDragIndex=%d\n",m_nDragIndex);*/

	// Create a drag image 建立拖动时的图片
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image (positive is up and to the left; neg is down and to the right)
	if(this->GetSelectedCount() > 1) //more than one item is selected
		pt.x = nOffset;
	pt.y = nOffset;


	m_pDragImage = this->CreateDragImage(m_nDragIndex, &pt);
	//TRACE("pt.x=%d,pt.y=%d\n",pt.x,pt.y);

	ASSERT(m_pDragImage); //make sure it was created
	//We will call delete later (in LButtonUp) to clean this up

	CBitmap bitmap;
	if(this->GetSelectedCount() > 1) //more than 1 item in list is selected
		//bitmap.LoadBitmap(IDB_BITMAP_MULTI);
		bitmap.LoadBitmap(IDB_BITMAP_MULTI_BOXES);
	else
		bitmap.LoadBitmap(IDB_BITMAP_BOX);
	m_pDragImage->Replace(0, &bitmap, &bitmap);

	//设置拖放图片
	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMLV->ptAction);


	m_bDragging = TRUE;	//进入拖动状态
	m_nDropIndex = -1;	//释放index

	//// Capture all mouse messages
	SetCapture();


	*pResult = 0;
}

void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	//// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT uFlags;
	m_nDropIndex=this->HitTest(point);
	//if(m_nDropIndex==-1) return;

	TRACE("OnMouseMove\n m_nDragIndex=%d,m_nDropIndex=%d\n",m_nDragIndex,m_nDropIndex);



	m_nItem=m_nDropIndex;
	if(m_bDragging)  //是否在拖动状态
	{
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		//Get the item that is below cursor
		if(m_nDropIndex==-1) m_nDropIndex=this->GetItemCount()-1;
		
		SetCursor(LoadCursor(NULL, IDC_ARROW));  //设置光标为箭头
		this->SetItemState(m_nDropIndex, 0, LVIS_DROPHILITED);

		//for(int i=0;i<this->GetItemCount();i++)
		//{
		//	this->SetItemState(m_nDropIndex, 0, LVIS_SELECTED);
		//	this->RedrawItems(m_nDropIndex, m_nDropIndex);
		//}
		//this->SetItemState(m_nDropIndex, LVIS_SELECTED, LVIS_SELECTED);

		this->RedrawItems(m_nDropIndex, m_nDropIndex);	

	}

	m_pDragImage->DragShowNolock(true);


	CListCtrl::OnMouseMove(nFlags, point);
}



void CListCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bDragging)
	{

		CWnd *pWnd=this->GetParent();
		CRect recClient;
		pWnd->GetClientRect(recClient);
		bool bPointInWindow=recClient.PtInRect(point);

		if(m_nDragIndex==m_nDropIndex) goto RESET;

		//if(m_nDragIndex!=-1 && m_nDropIndex!=-1)
		if(m_nDragIndex!=-1 && bPointInWindow)
		{
			//TRACE("m_nDragIndex=%d,m_nDropIndex=%d\n",m_nDragIndex,m_nDropIndex);
			//std::swap(m_vecLinkList[m_nDragIndex],m_vecLinkList[m_nDropIndex]);
			if(m_nDropIndex==-1)
			{
				m_nDropIndex=this->GetItemCount()-1;
			}
			if(m_nDragIndex==m_nDropIndex)  goto RESET;

			//CString temp=m_vecLinkList.at(m_nDragIndex).Path ;
			//交换位置
			LinkInfo linkInfo(m_vecLinkList.at(m_nDragIndex).Path,m_vecLinkList.at(m_nDragIndex).Title);
			m_vecLinkList.erase(m_vecLinkList.begin()+m_nDragIndex);
			m_vecLinkList.insert(m_vecLinkList.begin()+m_nDropIndex,linkInfo);

			UpdateList(m_vecLinkList);
			UpdateLnkFile(m_vecLinkList);

		}
		else if(m_nDragIndex!=-1 && m_nDropIndex==-1 )   //删除
		{
			CString msg=L"是否删除此项目？";

			if(MessageBox(msg,L"警告", MB_YESNO|MB_ICONWARNING )==IDYES)
			{
				m_vecLinkList.erase(m_vecLinkList.begin()+m_nDragIndex);
				UpdateList(m_vecLinkList);
				UpdateLnkFile(m_vecLinkList);
			}
		}

RESET:

		ReleaseCapture ();
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave(GetDesktopWindow());
		m_pDragImage->EndDrag ();
		delete m_pDragImage; //must delete it because it was created at the beginning of the drag

	}


	CListCtrl::OnLButtonUp(nFlags, point);
}



//点击事件
void CListCtrlEx::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//if(!SwapMouseButton(false))  //右键
	//{
	//	int xx=4;
	//}
	//else
	//{
		if(pNMLV->uNewState & LVIS_SELECTED){

			m_nItem=pNMLV->iItem;
			m_bSelectedItem=true;

			//if(m_nItem!= -1)
			//{		
			//	CString sPath=m_vecLinkList[m_nItem];
			//	ShellExecute(NULL, _T("open"),sPath, NULL, NULL, SW_MAXIMIZE);
			//	//int nItem = this->GetNextItem(-1, LVNI_SELECTED);   //获取item的index
			//	this->SetItemState(index, 0,-1); //-1 是取消高亮度显示，0表示取消选择

			//}
		}
	//}



	*pResult = 0;
}


UINT WINAPI CListCtrlEx::OpenThreadFun1(LPVOID lpParam)
{
	CString path=*(CString*)lpParam;
	ASSERT(path!=L"");
	if(path==L"") return 0;
	ShellExecute(NULL, _T("open"),path, NULL, NULL, SW_MAXIMIZE);

	return 0;
}



void CListCtrlEx::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//TODO: 在此添加控件通知处理程序代码

	if(m_bRightButton)
	{
		m_bRightButton=false;
		return;
	}

	
	if (!m_bDragging) 
	{
		if(m_nItem!= -1)
		{		
			CString sPath=m_vecLinkList.at(m_nItem).Path ;

			AfxBeginThread((AFX_THREADPROC)OpenThreadFun1, &sPath); 

			//ShellExecute(NULL, _T("open"),sPath, NULL, NULL, SW_MAXIMIZE);
			//int nItem = this->GetNextItem(-1, LVNI_SELECTED);   //获取item的index
			this->SetItemState(m_nItem, 0,-1); //-1 是取消高亮度显示，0表示取消选择

			HWND hwnd=this->GetParent()->GetSafeHwnd();
			::SetTimer(hwnd,0,200,NULL);
		}
	}

	*pResult = 0;
}


void CListCtrlEx::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	m_bRightButton=true;

	m_nItem=pNMItemActivate->iItem;
	if(m_nItem ==-1) return;

	CPoint pt;  
    GetCursorPos(&pt);//鼠标点的屏幕坐标  
	
	CMenu m_Menu;  
	m_Menu.LoadMenu(IDR_MENU2);//编辑好的菜单资源  
	CMenu *m_SubMenu = m_Menu.GetSubMenu(0);  
	//显示菜单
	m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);  

	*pResult = 0;
}



//////////////////////////////
//初始化ListCtrl



void CListCtrlEx::UpdateList(const vector<LinkInfo> &vecLinkList)
{

	this->DeleteAllItems();
	m_ImgList.Remove(-1);

	vector<LinkInfo>::iterator it;
	int i=0;

	SHFILEINFO sfi;
	for(it=m_vecLinkList.begin();it!=m_vecLinkList.end();it++)
	{
		//sPath=it->Path;
		//strFileName=it->Title;
		SHGetFileInfo(it->Path,0, &sfi,sizeof(sfi),SHGFI_ICON);
		m_ImgList.Add(sfi.hIcon);

		this->InsertItem(i,it->Title,i);
		i++;
		

	}

}


void CListCtrlEx::UpdateLnkFile(vector<LinkInfo> &vecLinkList)
{
	CString sFileListPath;
	CSetting::GetFileListPath(sFileListPath);
	DeleteFile(sFileListPath);

	//将LinkInfo转为CString
	vector<CString> vecLinkInfo;
	int iSize=vecLinkList.size();
	CString sLinkInfo;
	for(int i=0;i<iSize;i++)
	{
		sLinkInfo=vecLinkList.at(i).Path +L"|" +vecLinkList.at(i).Title;
		vecLinkInfo.push_back(sLinkInfo);
	}

	CUtility::GetInstance()->WriteText(vecLinkInfo,sFileListPath);
}


//获取文件
BOOL CListCtrlEx::GetFileList(vector<LinkInfo> &vecList)
{	
	CString sFileListPath;
	vector<CString> vecExportList;

	CSetting::GetFileListPath(sFileListPath);	
	if (CUtility::GetInstance()->PathIsExist(sFileListPath))
	{
		if(CUtility::GetInstance()->ReadTextLine(vecExportList,sFileListPath))
		{
			//删除不存在的路径
			int iSize=vecExportList.size();
			CString path;
			CString title;
			for(int i=0;i<iSize;i++)
			{
				vector<CString> vecLinkInfo;
				CUtility::GetInstance()->StringSplit(vecExportList.at(i),vecLinkInfo);  //分割
				if(vecLinkInfo.size()<2) continue;

				path=vecLinkInfo.at(0);
				title=vecLinkInfo.at(1);

				if(CUtility::GetInstance()->PathIsExist(path))
				{	if(title==L"NULL") title=CUtility::GetInstance()->GetPathName(path);
					LinkInfo linkInfo(path,title);
					vecList.push_back(linkInfo);
				}
			}

		}
		else
		{
			return false;
		}

	}
	else
	{
		CFile file;
		file.Open(sFileListPath,CFile::modeCreate);//建立文件
		file.Close();
		return false;
	}

	UpdateLnkFile(vecList);
	//UpdateList(vecList);

	return true;

}

void CListCtrlEx::OnRename()
{
	// TODO: 在此添加命令处理程序代码
	CString sTitle=m_vecLinkList.at(m_nItem).Title;

	CRenameDlg dlg(&sTitle);
	dlg.DoModal();

	if(m_vecLinkList.at(m_nItem).Title==sTitle || sTitle==L"")
	{
		return;
	}

	m_vecLinkList.at(m_nItem).Title=sTitle;
	UpdateList(m_vecLinkList);
	UpdateLnkFile(m_vecLinkList);
}



