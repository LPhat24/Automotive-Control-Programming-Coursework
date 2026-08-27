


#ifndef TrTmonitor_h
#define TrTmonitor_h


class TrTmonitor {
  public:
    TrTmonitor(unsigned long int _cong);
    int version(void);
    void begin(unsigned long int _tdbaud);

    void synch(unsigned char & _nhanA, unsigned char & _nhanB, unsigned char & _nut, signed int _gui1, signed int _gui2, unsigned char _den);

  private:

    //unsigned long TrTmonitor_delay;

};

#endif

