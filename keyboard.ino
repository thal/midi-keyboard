#include <MIDI.h>
#include <Song.h>

static const uint8_t numButtons = 6;

// Map buttons to notes in the scale
/*
  [0][1][2]
  [5][4][3]

  Buttons 5, 0, 1, 2, 3 will play a pentatonic scale.
  Button 4 serves a special purpose.
*/
static const uint8_t buttonToNote[6] ={ 1, 2, 3, 4, 0, 0 };

static const uint8_t playButton = 4;

int lastStates[numButtons];
int lastPressedStates[numButtons];
long lastDebounceTimes[numButtons];
long debounceDelay = 50;

int songPos = 0;

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
  //Serial.begin(9600);
  Serial1.begin(31250);
}

void loop()
{
  int buttonState;
  uint8_t i;
  for( i = 0; i < numButtons; i++ )
  {
    buttonState = digitalRead(i+2);
    
    if( buttonState != lastStates[i] )
    {
      lastDebounceTimes[i] = millis();
    }
    if( ( (millis() - lastDebounceTimes[i]) > debounceDelay ) &&
        ( lastPressedStates[i] != buttonState )
    )
    {
      if( i != playButton )
      {
        if( buttonState == LOW )
        {        
          midi.noteOn(scale[buttonToNote[i]]);
          Serial.println(scale[buttonToNote[i]], DEC);
        }
        else
        {
          midi.noteOff(scale[buttonToNote[i]]);
        }
      }
      else
      {
        if( buttonState == LOW )
        {
          midi_chord chord = song[songPos];
          songPos = (songPos+1) % songLength;
          midi.playChord(chord);
        }
        else
        {
          midi_chord chord = song[songPos-1];      

          midi.releaseChord(chord);
        }
      }
      lastPressedStates[i] = buttonState;
    }
    lastStates[i] = buttonState;
  }
}
