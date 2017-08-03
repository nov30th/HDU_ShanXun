#pragma once

class CNov30thAuth
{
public:
    CNov30thAuth(void);
    ~CNov30thAuth(void);
    CString getPIN(CString userName);
    byte* first(byte source[]);
    byte* second(byte source[]);
    char* last(byte source[]);
    CString getSecret(CString passWord);
    void long2char(unsigned long a, byte *b);
};
