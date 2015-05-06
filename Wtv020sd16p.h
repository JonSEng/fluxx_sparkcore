/*
 Wtv020sd16p.h - Library to control a WTV020-SD-16P module to play voices from an Arduino board.
 Created by Diego J. Arevalo, August 6th, 2012.
 Released into the public domain.
 
 Ported to Spark Core by Sandro Lange, January 3th, 2015
 */

#ifndef Wtv020sd16p_h
#define Wtv020sd16p_h

class Wtv020sd16p
{
public:
  Wtv020sd16p(int resetPin,int clockPin,int dataPin,int busyPin);
  void reset();
  void playVoice(unsigned int voiceNumber);
  void asyncPlayVoice(unsigned int voiceNumber);
  void stopVoice();
  void pauseVoice();
  void mute();
  void unmute();
  void setVolume(unsigned int volume);

private:
  void sendCommand(unsigned int command);
  void delayMicros(unsigned long delayMicros);
  int _resetPin;
  int _clockPin;
  int _dataPin;
  int _busyPin;
  int _busyPinState;
  unsigned int _currentVolume;
  unsigned long _lastMicros;
};

#endif

 