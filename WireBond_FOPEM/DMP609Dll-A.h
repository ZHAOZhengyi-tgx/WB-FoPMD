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
/********************usbdll.h***************************
sdjn3k-USB-Dmp609.V1.2版-----版权所有--济南三科科技
         0531-86670252
********************************************************/

      OpenDevice=(OPENDEVICE)GetProcAddress(gLiDLL,"OpenDevice");
      CloseDevice=(CLOSEDEVICE)GetProcAddress(gLiDLL,"CloseDevice");
      Dmp609_Ad=(DMP609_AD)GetProcAddress(gLiDLL,"Dmp609_Ad");
      Dmp609_Scan_Ad=(DMP609_SCAN_AD)GetProcAddress(gLiDLL,"Dmp609_Scan_Ad");
      Dmp609_Timer_Ad=(DMP609_TIMER_AD)GetProcAddress(gLiDLL,"Dmp609_Timer_Ad");
      Dmp609_Timer_Scan_Ad=(DMP609_TIMER_SCAN_AD)GetProcAddress(gLiDLL,"Dmp609_Timer_Scan_Ad");
      Dmp609_Timer_End_Ad=(DMP609_TIMER_END_AD)GetProcAddress(gLiDLL,"Dmp609_Timer_End_Ad");
      Dmp609_Read_Ad=(DMP609_READ_AD)GetProcAddress(gLiDLL,"Dmp609_Read_Ad");
      Dmp609_Da=(DMP609_DA)GetProcAddress(gLiDLL,"Dmp609_Da");
      Dmp609_Pwm=(DMP609_PWM)GetProcAddress(gLiDLL,"Dmp609_Pwm");
      Dmp609_Get_Di=(DMP609_GET_DI)GetProcAddress(gLiDLL,"Dmp609_Get_Di");
      Dmp609_Set_Do=(DMP609_SET_DO)GetProcAddress(gLiDLL,"Dmp609_Set_Do");
      Dmp609_Set_Count=(DMP609_SET_COUNT)GetProcAddress(gLiDLL,"Dmp609_Set_Count");
      Dmp609_Read_Count=(DMP609_READ_COUNT)GetProcAddress(gLiDLL,"Dmp609_Read_Count");
      Dmp609_Stop_Pulse=(DMP609_STOP_PULSE)GetProcAddress(gLiDLL,"Dmp609_Stop_Pulse");
      Dmp609_Pulse=(DMP609_PULSE)GetProcAddress(gLiDLL,"Dmp609_Pulse");
      Dmp609_AnalogWave=(DMP609_ANALOGWAVE)GetProcAddress(gLiDLL,"Dmp609_AnalogWave");



