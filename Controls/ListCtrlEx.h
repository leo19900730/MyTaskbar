#pragma once
#include "..\\Resource.h"
#include <vector>

using std::vector;


///////////////////////////////////////////

struct LinkInfo
{
	CString Path;
	CString Title;

	LinkInfo(CString path=L"",CString title=L"")
	{
		Path=path;
		Title=title;
	}
};


///////////////////////////////////////////

// CListCtrlEx
class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()

private:

	int m_nItem;
	int m_nSubItem;
	CToolTipCtrl m_toolTip;

	CImageList m_ImgList;
	vector<LinkInfo> m_vecLinkList;

	bool m_bSelectedItem;

	CImageList* m_pDragImage;	//拖动图标时的图片
	BOOL m_bDragging;	//是否处于拖动状态
	int	m_nDragIndex;	//拖动项的index
	int m_nDropIndex;	//放置位置项的index

	//CWnd *m_pParentWnd;

	bool m_bRightButton;

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);


	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);


	void InitListCtrl();
	void UpdateList(const vector<LinkInfo> &vecLinkList);
	void UpdateLnkFile(vector<LinkInfo> &vecLinkList);
	BOOL GetFileList(vector<LinkInfo> &vecList);

	afx_msg void OnRename();
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);


	static UINT WINAPI CListCtrlEx::OpenThreadFun1(LPVOID lpParam);
};


