#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

struct MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__
#define COIN_SIZE 5

struct Special{
  double special_x;
  double special_y;
  double radius;
  Circle circle;
  Special(double arg_special_x,double arg_special_y, double arg_radius,bool gold){
    special_x=arg_special_x;
    special_y=arg_special_y;
    radius=arg_radius;
    Circle fake(special_x,special_y,radius);
    circle=fake;
    fake.hide();
    if(gold){
      circle.setColor(COLOR("yellow"));
    }
    else circle.setColor(COLOR("black"));
    circle.setFill(true);
  }
  //makes the gold move to a new random location
  void move(){
    double new_x=(rand()%5)*100+200;
    double new_y=(rand()%5)*60+100;
    special_x=new_x;
    special_y=new_y;
    circle.reset(new_x,new_y,radius);
  }
  double getXPos(){
    return special_x;
  }
  double getYPos(){
    return special_y;
  }
};

struct Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  bool catchable;

  // Moving parts
  Circle coin_circle;

  public: Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta,double argcoin_start_x,double argcoin_start_y, bool argcatchable) :
  MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    coin_start_x = argcoin_start_x;
    coin_start_y = argcoin_start_y;
    coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
    catchable=argcatchable;
    if(catchable)coin_circle.setColor(COLOR("green"));
    else coin_circle.setColor(COLOR("red"));
    coin_circle.setFill(true);
    addPart(&coin_circle);
  }

  void resetCoin(double speed,double angle);
  void resetCoin1(double speed,double angle);
  void resetCoin2(double speed,double angle);
  void changecolor();
  bool Catchable(){
    return catchable;
  }

}; // End struct Coin

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 30
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 300
#define INIT_RELEASE_SPEED 150

#define COIN_SPEED 180
#define COIN_ANGLE_DEG 90
#define COIN_ANGLE_DEG1 80
#define COIN_ANGLE_DEG2 100

#define LASSO_G 50
#define COIN_G 50

#define LASSO_SIZE 10
#define LASSO_RADIUS 50

struct Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  public:Coin *the_coin;

  int points;

  void initLasso();
  public:
  Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) :
  MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  bool check_for_coin(Coin *coin);
  bool check_for_gold(Special *goldPtr){
    double lasso_x1 = getXPos();
    double lasso_y1 = getYPos();
    double gold_x1 = goldPtr->getXPos();
    double gold_y1 = goldPtr->getYPos();
    double xdiff1 = (lasso_x1 - gold_x1);
    double ydiff1 = (lasso_y1 - gold_y1);
    double distance1 = sqrt((xdiff1*xdiff1)+(ydiff1*ydiff1));
    if(distance1 <= 30) {
      return true;
    }
    else return false;
  }
  int getNumCoins() { return points; }

}; // End struct Lasso

#endif

//coin.h

void Coin::resetCoin(double speed,double angle) {
  double coin_speed = speed;
  double coin_angle_deg = angle;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::resetCoin1(double speed,double angle) {
  double coin_speed = speed;
  double coin_angle_deg = angle;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::resetCoin2(double speed,double angle) {
  double coin_speed = speed;
  double coin_angle_deg = angle;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::changecolor(){
  catchable=!catchable;
  if(catchable){
    coin_circle.setColor(COLOR("green"));
    coin_circle.setFill(true);
  }
  else {
    coin_circle.setColor(COLOR("red"));
    coin_circle.setFill(true);
  }
}

//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("blue"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  points = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("blue"));

  lasso_band.setColor(COLOR("green"));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  //adds points if the coin is green
  if(the_coin != NULL&&the_coin->Catchable()) {
    points++;
    the_coin=NULL;
  }
  //subtracts points if the coin is red
  if(the_coin != NULL&&!the_coin->Catchable()) {
    points--;
    the_coin=NULL;
  }
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

bool Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
    return true;
  }
  else return false;
} // End Lasso::check_for_coin()

void introscreen(){
  //initialise intro screen
  Rectangle startrect(WINDOW_X/2,WINDOW_Y/2,50,20);
  startrect.setColor(COLOR("yellow"));
  startrect.setFill(true);
  Rectangle instructionsrect(WINDOW_X/2,2*WINDOW_Y/3,50,20);
  instructionsrect.setColor(COLOR("yellow"));
  instructionsrect.setFill(true);
  Rectangle titlerect(WINDOW_X/2,WINDOW_Y/3,100,40);
  titlerect.setColor(COLOR("red"));
  titlerect.setFill(true);
  Text title(WINDOW_X/2,WINDOW_Y/3,"LASSO");
  Text StartGame(WINDOW_X/2,WINDOW_Y/2,"Play");
  Text Help(WINDOW_X/2,2*WINDOW_Y/3,"Help");
  Text instructionpagetitle(WINDOW_X/2,WINDOW_Y/10,"HELP");
  //make a text array for instructions
  int k=18;
  Text instructionline[k];
  instructionline[0]=Text(WINDOW_X/2,100,"The objective of the game is to score the most points by catching coins.");
  instructionline[1]=Text(WINDOW_X/2,130,"Catching a coin while it is green results in an extra point and catching it red deducts a point.");
  instructionline[2]=Text(WINDOW_X/2,160,"The coins switch colors every few seconds. Try to catch them before they turn red, and yank otherwise.");
  instructionline[3]=Text(WINDOW_X/2,190,"You get three lives at the beginning of the game and you lose if you have no lives.");
  instructionline[4]=Text(WINDOW_X/2,220,"You have to catch a coin(green or red) every 10 seconds, otherwise you lose a life.");
  instructionline[5]=Text(WINDOW_X/2,250,"You can increase your life by one by catching a special golden coin using a special key g.");
  instructionline[6]=Text(WINDOW_X/2,280,"The golden special coin is hard to catch as it moves randomly and changes its position very fast.");
  instructionline[7]=Text(WINDOW_X/2,310,"Avoid the black obstacle which moves in the same way as the golden coin to avoid losing a life.");
  instructionline[8]=Text(WINDOW_X/2,340,"Commands:");
  instructionline[9]=Text(WINDOW_X/4,360,"t throw lasso");
  instructionline[10]=Text(WINDOW_X/4,380,"y yank lasso");
  instructionline[11]=Text(WINDOW_X/4,400,"l loop lasso");
  instructionline[12]=Text(WINDOW_X/4,420,"g catch golden coin");
  instructionline[13]=Text(WINDOW_X/2,430,"q quit game");
  instructionline[14]=Text(3*WINDOW_X/4,360,"[ decrease lasso release angle");
  instructionline[15]=Text(3*WINDOW_X/4,380,"] increase lasso release angle");
  instructionline[16]=Text(3*WINDOW_X/4,400,"- decrease lasso release speed");
  instructionline[17]=Text(3*WINDOW_X/4,420,"= increase lasso release speed");
  Rectangle GoBackRect(700,100,60,20);
  GoBackRect.setColor(COLOR("yellow"));
  GoBackRect.setFill(true);
  Text GoBack(700,100,"Go Back");
  instructionpagetitle.hide();
  for(int i=0;i<k;i++){
    instructionline[i].hide();
  }
  GoBack.hide();
  GoBackRect.hide();

  int x=0,y=0;
  //while loop ensures that clicking out of the box doesnt end the program
  while(true){
      //Wait for user to click
      int w=getClick();
      x=w/65536; y=w%65536;
      if(x>=375&&x<=425&&y>=390&&y<=410)break;
      if(x>=375&&x<=425&&y>=290&&y<=310)break;
  }
  //User clicks on Help
  if(x>=375&&x<=425&&y>=390&&y<=410){
      //Hide previous screen
      title.hide();
      StartGame.hide();
      Help.hide();
      startrect.hide();
      instructionsrect.hide();
      titlerect.hide();

      //show instructions
      instructionpagetitle.show();
      for(int i=0;i<k;i++){
        instructionline[i].show();
      }
      //show a back button
      GoBack.show();
      GoBackRect.show();
      int x1=0,y1=0;
      while(true){
          int w1=getClick();
          x1=w1/65536; y1=w1%65536;
          if(x1>=670&&x1<=730&&y1>=90&&y1<=110)break;
      }
      if(x1>=670&&x1<=730&&y1>=90&&y1<=110){
          GoBack.hide();
          GoBackRect.hide();
          instructionpagetitle.hide();
          for(int i=0;i<k;i++){
            instructionline[i].hide();
          }
          //recursive call to return back to main screen
          introscreen();
      }
  }

  //User clicks on Play
  if(x>=375&&x<=425&&y>=290&&y<=310){
      title.hide();
      StartGame.hide();
      Help.hide();
      startrect.hide();
      instructionsrect.hide();
      titlerect.hide();
      return;
  }
}

int main() {
  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  Rectangle background(WINDOW_X/2,WINDOW_Y/2,WINDOW_X,WINDOW_Y);
  background.setColor(COLOR(135,206,235));
  background.setFill(true);
  introscreen();

  //initialize time variables and lives
  int lives=3;
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = 10; // sec; -ve means infinite
  float currTime = 0;
  int timeleft=10;

   // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  bool catchable= false;
  bool paused1 = true;
  bool rtheta1 = true;
  bool catchable1=true;
  bool paused2 = true;
  bool rtheta2 = true;
  bool catchable2=true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Points: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  char livesStr[256];
  sprintf(livesStr, "Lives: %d", lives);
  Text Lives(PLAY_X_START+150, PLAY_Y_HEIGHT+20, lives);
  char timeleftStr[256];
  sprintf(timeleftStr, "Time Left: %d", timeleft);
  Text Timeleft(PLAY_X_START+150, PLAY_Y_HEIGHT+50, timeleft);

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta,(PLAY_X_START+WINDOW_X)/2,PLAY_Y_HEIGHT,catchable);

  paused1 = true; rtheta1 = true;
  double coin1_speed = COIN_SPEED;
  double coin1_angle_deg = 80;
  double coin1_ax = 0;
  double coin1_ay = COIN_G;
  Coin coin1(coin1_speed, coin1_angle_deg, coin1_ax, coin1_ay, paused1, rtheta1,600,PLAY_Y_HEIGHT,catchable1);

  paused2 = true; rtheta2 = true;
  double coin2_speed = COIN_SPEED;
  double coin2_angle_deg = 100;
  double coin2_ax = 0;
  double coin2_ay = COIN_G;
  Coin coin2(coin2_speed, coin2_angle_deg, coin2_ax, coin2_ay, paused2, rtheta2,350,PLAY_Y_HEIGHT,catchable2);

  Special gold((rand()%5)*100+200,(rand()%5)*60+100,20,true);
  Special obstacle((rand()%5)*100+180,(rand()%5)*60+80,20,false);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // Press g to claim gold and increase life
  // When q is pressed, quit

  for(;;){
    if((runTime > 0) && (currTime > runTime)) {
      lives--;
      currTime=0;
    }
    if(lives<=0||lasso.getNumCoins()<0)break;
    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
    case 't':
	  lasso.unpause();
	  break;
    case 'y':
	  lasso.yank();
	  break;
    case 'l':
	  lasso.loopit();
	  if(lasso.check_for_coin(&coin)){
	    coin.resetCoin(70+20*(rand()%8),45+10*(rand()%10));
	    currTime=0;
	  }
	  if(lasso.check_for_coin(&coin1)){
	    coin1.resetCoin1(70+20*(rand()%8),45+10*(rand()%10));
	    currTime=0;
	  }
	  if(lasso.check_for_coin(&coin2)){
	    coin2.resetCoin2(70+20*(rand()%8),45+10*(rand()%10));
	    currTime=0;
	  }
	  wait(STEP_TIME*5);
	  break;
    case 'g':
      if(lasso.check_for_gold(&gold)){
      gold.move();
      lives++;
      }
      break;
    case '[':
      if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG); }
	  break;
    case ']':
	  if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	  break;
    case '-':
	  if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	  break;
    case '=':
	  if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	  break;
    case 'q':
	  exit(0);
    default:
	break;
      }
    }
    // make objects move
    lasso.nextStep(stepTime);
    coin.nextStep(stepTime);
    coin1.nextStep(stepTime);
    coin2.nextStep(stepTime);

    //unpause coins
    if(coin.isPaused()) {
      if((currTime) >= COIN_GAP) {
	    coin.unpause();
      }
    }
    if(coin1.isPaused()) {
      if((currTime) >= COIN_GAP) {
	    coin1.unpause();
      }
    }
    if(coin2.isPaused()) {
      if((currTime) >= COIN_GAP) {
	    coin2.unpause();
      }
    }
    //reset balls if they hit the x or y axis
    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin(70+12*(rand()%8),45+10*(rand()%10));
      last_coin_jump_end = currTime;
    }
    if(coin1.getYPos() > PLAY_Y_HEIGHT) {
      coin1.resetCoin1(70+12*(rand()%8),45+10*(rand()%10));
      last_coin_jump_end = currTime;
    }
    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.resetCoin2(70+12*(rand()%8),45+10*(rand()%10));
      last_coin_jump_end = currTime;
    }
    if(coin.getXPos() < PLAY_X_START) {
      coin.resetCoin(70+12*(rand()%8),45+10*(rand()%10));
      last_coin_jump_end = currTime;
    }
    if(coin1.getXPos() < PLAY_X_START) {
      coin1.resetCoin1(70+12*(rand()%8),45+10*(rand()%10));
      last_coin_jump_end = currTime;
    }
    if(coin2.getXPos() < PLAY_X_START) {
      coin2.resetCoin2(70+12*(rand()%8),45+10*(rand()%10));
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Points: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    sprintf(livesStr, "Lives: %d", lives);
    Lives.setMessage(livesStr);

    sprintf(timeleftStr, "Time Left: %d", timeleft);
    Timeleft.setMessage(timeleftStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);

    //change color of balls and make gold coin and obstacle move
    if(stepCount%60==0){
      coin.changecolor();
      gold.move();
      obstacle.move();
    }
    if(stepCount%80==0){
      coin1.changecolor();
    }
    if(stepCount%100==0){
      coin2.changecolor();
    }
    timeleft=10-currTime;
    //lose life if lasso hits obstacle
    double squaredistancex=(lasso.getXPos()-obstacle.getXPos())*(lasso.getXPos()-obstacle.getXPos());
    double squaredistancey=(lasso.getYPos()-obstacle.getYPos())*(lasso.getYPos()-obstacle.getYPos());
    double distance=sqrt(squaredistancex+squaredistancey);
    if(distance<30){
      lives--;
      lasso.yank();
    }
  } // End for(;;)
  //Game over page displaying points and lives
  Text gameover(400,300," GAME OVER ");
  Text pointsatend(400,330, "Points:");
  Text pointsnumber(450,330,lasso.getNumCoins());
  Text livesatend(400,360,"Lives:");
  Text livesnumver(450,360,lives);
  wait(5);
  return 0;
} // End main_program
