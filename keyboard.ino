#include <MIDI.h>
#include <Song.h>

static const uint8_t numButtons = 6;

int lastStates[numButtons];
long lastDebounceTimes[numButtons];
long debounceDelay = 50;

// Use MIDI over Serial1
MIDI midi( Serial1 );


void setup()
{
  // Using pins 2-7
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  
  Serial1.begin(31250);
}

void loop()
{
  int buttonState;
  // Fashion a byte from the states of all six buttons
  uint8_t dataByte = 0x00;
  uint8_t i;
  for( i = 0; i < numButtons; i++ )
  {
    buttonState = digitalRead(i+2);
    // Set the ith bit if button pressed
    
    if( buttonState != lastStates[i] )
    {
      lastDebounceTimes[i] = millis();
    }
    if( ( (millis() - lastDebounceTimes[i]) > debounceDelay ) )
    {
      if( buttonState == LOW )
      {        
        midi.noteOn(song[i]);
      }
      else
      {
        midi.noteOff(song[i]);
      }
    }
    lastStates[i] = buttonState;
  }
}
