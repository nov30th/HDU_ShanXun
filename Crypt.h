#pragma once

class CCrypt
{
public:
    CCrypt(void);
    ~CCrypt(void);
    CString Encrypt(CString S, WORD Key); // ���ܺ���
    CString Decrypt(CString S, WORD Key); // ���ܺ���
};
