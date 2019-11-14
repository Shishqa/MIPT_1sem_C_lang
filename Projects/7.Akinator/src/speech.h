#ifndef SPEECH_H_
#define SPEECH_H_

const size_t CALL_MAX_SIZE = 100;
const size_t PHRASE_MAX_SIZE = 100;

void Say (const char * phrase, 
          int speed = 140, 
          int vol = 100);
void Say (const char * prefix, 
          const char * gap, 
          const char * suffix, 
          int speed = 140, 
          int vol = 100);
void Say (const char * prefix, 
          const char * gap1, 
          const char * mid, 
          const char * gap2, 
          const char * suffix, 
          int speed = 140, 
          int vol = 100);

void flush ();

#endif // !SPEECH_H_