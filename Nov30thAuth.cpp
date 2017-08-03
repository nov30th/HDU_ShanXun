#include "StdAfx.h"
#include "Nov30thAuth.h"
#include "MD5Checksum.h"
#include "VirtualizerSDK.h"

static byte RADIUS[17] = {"chongqingradius1"};

CNov30thAuth::CNov30thAuth(void)
{

}

CNov30thAuth::~CNov30thAuth(void)
{

}

CString CNov30thAuth::getPIN(CString userName)
{
	VIRTUALIZER_START
    byte *userByte = new byte[userName.GetLength()+1];
    sprintf((char*)userByte,"%s",userName);
    //byte userByte[] = userName.
    CTime theTime = CTime::GetCurrentTime();
    unsigned long time = (int) theTime.GetTime();
    time %= 9999;
    byte timeByte[4] = {'8','4','8','d'};
    long2char(time,timeByte);
    byte userAndRadiusWithTime[32];
    CString MD5;
    byte timeAfterFirst[4];
    byte timeAfterSecond[6];
    char timeAfterLast[6];
    static char PIN[10];
    for(int i = 0; i < 4; i++)
        userAndRadiusWithTime[i] = timeByte[i];

    for(int i = 4; i < 16; i++)
        userAndRadiusWithTime[i] = userByte[i - 4];

    for(int i = 16; i < 32; i++)
        userAndRadiusWithTime[i] = RADIUS[i - 16];
    CMD5Checksum md5;
    MD5 = md5.GetMD5(userAndRadiusWithTime, 32);
    memcpy(timeAfterFirst,first(timeByte), sizeof(byte)*4);
    memcpy(timeAfterSecond,second(timeAfterFirst), sizeof(byte)*6);
    memcpy(timeAfterLast,last(timeAfterSecond), sizeof(byte)*6);
    for(int i = 0; i < 6; i++)
        PIN[i] = timeAfterLast[i];
    PIN[6] = MD5.GetAt(0);
    PIN[7] = MD5.GetAt(1);
    PIN[8] = '\0';
    static CString retval;
    retval = PIN;
   // retval.Replace("%","%25");
    //retval.Replace(" ","%20");
    /*
    char temp[3]={0};
    static char encodePIN[255];
    memset(encodePIN,0,255);
    for(int i=0;i<strlen(PIN);i++)
    {
        strcat(encodePIN,"%");
        temp[0]  = (int)PIN[i]/16;
        temp[1]  = (int)PIN[i]%16;
        if (temp[0]>=16)
            temp[0] += 61 - 16;
        else
            temp[0] += 48;
        if (temp[1]>=16)
            temp[1] += 61 - 16;
        else
            temp[1] += 48;
        strcat(encodePIN,temp);
    }
    */
	VIRTUALIZER_END
    return retval;
}

byte* CNov30thAuth::first(byte source[])
{
	VIRTUALIZER_START
    byte temp[32];
    static byte result[4];
    for(int i = 0; i < 32; i++)
    {
        temp[i] = (byte)(source[(31 - i) / 8] & 1);
        source[(31 - i) / 8] = (byte)(source[(31 - i) / 8] >> 1);
    }
    for(int i = 0; i < 4; i++)
        result[i] = (byte)(temp[i] * 128 + temp[4 + i] * 64 + temp[8 + i] * 32 + temp[12 + i] * 16 + temp[16 + i] * 8 + temp[20 + i] * 4 + temp[24 + i] * 2 + temp[28 + i]);
	VIRTUALIZER_END
    return result;
}

byte* CNov30thAuth::second(byte source[])
{
	VIRTUALIZER_START
    static byte result[6];
    result[1] = (byte)((source[0] & 3) << 4);
    result[0] = (byte)(source[0] >> 2 & 0x3f);
    result[2] = (byte)((source[1] & 0xf) << 2);
    result[1] = (byte)((source[1] >> 4 & 0xf) + result[1]);
    result[3] = (byte)(source[2] & 0x3f);
    result[2] = (byte)((source[2] >> 6 & 3) + result[2]);
    result[5] = (byte)((source[3] & 3) << 4);
    result[4] = (byte)(source[3] >> 2 & 0x3f);
	VIRTUALIZER_END
    return result;
}

char* CNov30thAuth::last(byte source[])
{
	VIRTUALIZER_START
    static char result[6];
    for(int i = 0; i < 6; i++)
    {
        result[i] = (char)(source[i] + 32);
        if(result[i] >= '@')
            result[i]++;
    }
	VIRTUALIZER_END
    return result;
}

CString CNov30thAuth::getSecret(CString passWord)
{
    return passWord;
}



void CNov30thAuth::long2char(unsigned long a, byte *b)
{
	VIRTUALIZER_START
    int c =0;
    while(a)
    {
        b[c++] = a%10;
        a/=10;
    }
	VIRTUALIZER_END
}

