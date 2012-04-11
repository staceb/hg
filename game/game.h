
// interface between a platform-independent game engine 
// and the underlying platform

#include "doublePair.h"



// these are called by SDL or iPhone (or other platform) app wrapper


const char *getWindowTitle();


// get data that should be saved in the header of the game that is 
// being recorded
// Must be encoded as a single string with no whitespace.
// Result destroyed by caller
char *getCustomRecordedGameData();

// if returns true, then a marker is drawn on screen during playback to show
// recorded mouse position and click activity.
char showMouseDuringPlayback();

// get secret salt data for secure hashing
// Result destroyed by caller
char *getHashSalt();



// name of custom font TGA file to find in "graphics" folder
const char *getFontTGAFileName();

// called by platform to draw status messages on top of game image
// game can pick where these should be displayed (though platform ensures
// that it is drawn after frame is drawn)
// String might contain multiple lines separated by '\n'
void drawString( const char *inString );


// should demo code be obtained from user and checked against server?
char isDemoMode();

// secret shared with demo server (so that we can detect that it's a real
// demo server)
const char *getDemoCodeSharedSecret();

// url of demo code server PHP script
const char *getDemoCodeServerURL();



// recorded game data will be null if no game is playing back
void initFrameDrawer( int inWidth, int inHeight, int inTargetFrameRate,
                      const char *inCustomRecordedGameData,
                      char inPlayingBack );


// called at application termination
// good time to save state for next launch
void freeFrameDrawer();


// draw scene into frame using GL function calls
// inUpdate is true if game logic should be updated
void drawFrame( char inUpdate );



// these are all in world coordinates based on what is set
// in setViewCenterPosition and setViewSize below

// pointer movement unpressed
void pointerMove( float inX, float inY );

// start of pointer press
void pointerDown( float inX, float inY );

// movement with pointer pressed
void pointerDrag( float inX, float inY );

// end of pointer press
void pointerUp( float inX, float inY );


void keyDown( unsigned char inASCII );

void keyUp( unsigned char inASCII );


// this GL file is actually platform-independent and has nothing to do 
// with OpenGL
// Defines key codes
#include "minorGems/graphics/openGL/KeyboardHandlerGL.h"

void specialKeyDown( int inKeyCode );

void specialKeyUp( int inKeyCode );





#include <stdint.h>
typedef int16_t Sint16;
typedef uint8_t Uint8;


// should sound be initialized?
char getUsesSound();


// gets the next buffer-full of sound samples from the game engine
// inBuffer should be filled with stereo Sint16 samples, little endian,
//    left-right left-right ....
// NOTE:  may not be called by the same thread that calls drawFrame,
//        depending on platform implementation
void getSoundSamples( Uint8 *inBuffer, int inLengthToFillInBytes );










// These are called BY game engine (implemented by supporting platform)


unsigned int getRandSeed();


// when inFromKey is pressed, an event for inToKey will be generated
// (and no event for inFromKey will be generated)
void mapKey( unsigned char inFromKey, unsigned char inToKey );


// returns true if Control, Alt, or Meta key is down
// behavior varies somewhat by platform
// used for implementing platform-independent keyboard shortcuts (ctrl-z, etc)
char isCommandKeyDown();



// sample rate shared by game engine and sound rendering platform
//#define gameSoundSampleRate 22050
//#define gameSoundSampleRate 44100
// #define gameSoundSampleRate 11025


// each rendering platform sets a sample rate
int getSampleRate();


// true to start or resume playing
// false to pause
// Audio starts off paused
void setSoundPlaying( char inPlaying );


// for platforms where audio runs in separate thread
// Lock when manipulating data that is touched by getSoundSamples
void lockAudio();
void unlockAudio();


// returns true if sound started up and is running
char isSoundRunning();



// for moving view around
void setViewCenterPosition( float inX, float inY );

doublePair getViewCenterPosition();


// 1:1 aspect ratio (truncated on shorter screen dimension)
void setViewSize( float inSize );


// default visible
void setCursorVisible( char inIsVisible );


// confines mouse pointer to window and prevents window manager
// from intercepting key presses
// default off
void grabInput( char inGrabOn );


// sets how mouse events are reported
// true to use game world coordinates
// false to use screen coordinates
//
// defaults to game world coordinates
void setMouseReportingMode( char inWorldCoordinates );


// warps mouse to center of screen
// does not generate pointer event callback (and dumps all pending pointer
// motion events that happened before warp)
// returns new mouse position
void warpMouseToCenter( int *outNewMouseX, int *outNewMouseY );



// returns translation of key using current language
const char *translate( const char *inTranslationKey );



// pause and resume the game
void pauseGame();

char isPaused();


// pause mode eventually runs in reduced frame rate to reduce CPU usage
// call this to temporarily increase framerate back to normal
void wakeUpPauseFrameRate();


// returns true if we're currently executing a recorded game
char isGamePlayingBack();







// save a screenshot to the "screenShots" folder
// screenshot may be delayed until the end of the next redraw.
void saveScreenShot( const char *inPrefix );



// the supporting platform wraps web requests to ensure that web
// data repeats during playback of recorded games

// inMethod = GET, POST, etc.
// inURL the url to retrieve
// inBody the body of the request, can be NULL
// request body must be in application/x-www-form-urlencoded format

// returns unique int handle for web request, always > -1 
int startWebRequest( const char *inMethod, const char *inURL,
                     const char *inBody );

// take anoter non-blocking step
// return 1 if request complete
// return -1 if request hit an error
// return 0 if request still in-progress
int stepWebRequest( int inHandle );


// gets the response body as a \0-terminated string, destroyed by caller
char *getWebResult( int inHandle );


// frees resources associated with a web request
// if request is not complete, this cancels it
// if hostname lookup is not complete, this call might block.
void clearWebRequest( int inHandle );



