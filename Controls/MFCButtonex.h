#pragma once

using namespace Gdiplus;

// MFCButtonex

class MFCButtonex : public CMFCButton
{
	DECLARE_DYNAMIC(MFCButtonex)

public:
	MFCButtonex();
	//MFCButtonex(CString sImgNormal,CString sImgHover);
	virtual ~MFCButtonex();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	///////////////////////////////////
	//void ShowImage(CString sImgNormal,CString sImgHover);
	void ShowImage(HBITMAP hBmp,HBITMAP hBmpHot);


private:
	//Bitmap *m_imgNormal;
	//Bitmap *m_imgHover;
};


