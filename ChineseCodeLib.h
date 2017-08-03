#pragma once

class CChineseCodeLib
{
public:
	CChineseCodeLib(void);
	~CChineseCodeLib(void);
	void UTF_8ToUnicode(WCHAR* pOut,char *pText);
	void UnicodeToUTF_8(char* pOut,WCHAR* pText);
	void UnicodeToGB2312(char* pOut,unsigned short uData);
	void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	void GB2312ToUTF_8(CString& pOut,char *pText, int pLen);
	void UTF_8ToGB2312(CString &pOut, char *pText, int pLen);

};
