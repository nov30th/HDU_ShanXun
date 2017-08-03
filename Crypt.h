#pragma once

class CCrypt
{
public:
    CCrypt(void);
    ~CCrypt(void);
    CString Encrypt(CString S, WORD Key); // 加密函数
    CString Decrypt(CString S, WORD Key); // 解密函数
};
