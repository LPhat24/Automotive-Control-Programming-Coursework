

#include "Arduino.h"
#include "TrTmonitor.h"

String _chuoiNhan;
char _kytuNhan;

TrTmonitor::TrTmonitor(unsigned long int _cong)
{
	;
}

int TrTmonitor::version(void)
{
	Serial.println("#");  
	return 4;
}

void TrTmonitor::synch(unsigned char & _nhanA, unsigned char & _nhanB, unsigned char & _nut, signed int _gui1, signed int _gui2, unsigned char _den)
{
      Serial.print("*");  
      Serial.print(_den);  
      Serial.print("\t");  
      Serial.print(_gui1);  
      Serial.print("\t");  
      Serial.print(_gui2);  
      Serial.println("#");  

  while (Serial.available()>0) {
    _kytuNhan = Serial.read();
    if(_kytuNhan==' ') {
      if(_chuoiNhan[0]=='*') {
        _chuoiNhan.remove(0, 1); 
        _nut=_chuoiNhan.toInt();
        _chuoiNhan="";
      }
      else {
        _nhanA=_chuoiNhan.toInt();
        _chuoiNhan="";
      }
    } 
    else if(_kytuNhan=='#' || _kytuNhan==10) {
      _nhanB=_chuoiNhan.toInt();
      _chuoiNhan="";
    } 
    else {_chuoiNhan+=_kytuNhan;}
  }
}

void TrTmonitor::begin(unsigned long int _tdbaud)
{
	Serial.begin(_tdbaud);
}
