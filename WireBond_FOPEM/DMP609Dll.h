//The MIT License (MIT)
//
//Copyright (c) 2016 ZHAOZhengyi-tgx
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
/****************DMPxxx系列.h文件***************************
sdjn3k-USB-DMP609.V1.2版-----版权所有--济南三科科技
         0531-86670252
********************************************************/
#ifndef __DMP609_DLL__
#define __DMP609_DLL__
HINSTANCE gLiDLL=NULL;

typedef unsigned char(*CLOSEDEVICE)(HANDLE hDevice);
 CLOSEDEVICE  CloseDevice;

typedef HANDLE(*OPENDEVICE)(unsigned char );
 OPENDEVICE   OpenDevice;

typedef int(*DMP609_AD)(HANDLE hDevice,unsigned char ch,unsigned char n);
 DMP609_AD   Dmp609_Ad;
     
typedef int(*DMP609_SCAN_AD)(HANDLE hDevice,unsigned char start,unsigned char end ,unsigned char g , unsigned int *data);
 DMP609_SCAN_AD   Dmp609_Scan_Ad;

typedef int(*DMP609_TIMER_AD)(HANDLE hDevice,unsigned char ch,unsigned char g,unsigned char pin, double Fer);
 DMP609_TIMER_AD   Dmp609_Timer_Ad;

typedef int(*DMP609_TIMER_SCAN_AD)(HANDLE hDevice,unsigned char start,unsigned char end,unsigned char g,unsigned char pin, double Fer);
 DMP609_TIMER_SCAN_AD   Dmp609_Timer_Scan_Ad;

typedef int(*DMP609_TIMER_END_AD)(HANDLE hDevice);
 DMP609_TIMER_END_AD   Dmp609_Timer_End_Ad;

typedef int(*DMP609_READ_AD)(HANDLE hDevice,unsigned int Sn,unsigned int *data);
 DMP609_READ_AD   Dmp609_Read_Ad;

typedef unsigned char(*DMP609_DA)(HANDLE hDevice,unsigned char ch,unsigned int DaData);
 DMP609_DA   Dmp609_Da;

typedef unsigned char(*DMP609_PWM)(HANDLE hDevice,unsigned char Pwm_ch,unsigned char SW,unsigned char PwmData,unsigned float Fer);
 DMP609_PWM   Dmp609_Pwm;

typedef unsigned char(*DMP609_SET_DO)(HANDLE hDevice,unsigned char ch,unsigned int DoData);
 DMP609_SET_DO   Dmp609_Set_Do;

typedef int(*DMP609_GET_DI)(HANDLE hDevice,unsigned int ch);
 DMP609_GET_DI   Dmp609_Get_Di;

typedef long(*DMP609_READ_COUNT)(HANDLE hDevice);
 DMP609_READ_COUNT   Dmp609_Read_Count;

typedef unsigned char(*DMP609_SET_COUNT)(HANDLE hDevice,unsigned char sw,unsigned char charCo);
 DMP609_SET_COUNT   Dmp609_Set_Count;
 
typedef int(*DMP609_PULSE)(HANDLE hDevice,unsigned char sw,unsigned int T);
 DMP609_PULSE   Dmp609_Pulse;

typedef int(*DMP609_STOP_PULSE)(HANDLE hDevice);
 DMP609_STOP_PULSE   Dmp609_Stop_Pulse;

typedef int(*DMP609_ANALOGWAVE)(HANDLE hDevice,unsigned char Wave,unsigned char sw,unsigned int T);
 DMP609_ANALOGWAVE   Dmp609_AnalogWave;

#endif  // __DMP609_DLL__