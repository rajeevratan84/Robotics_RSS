#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std; 

class Whisker {

 private:

  int inputsignal;
  time_t timer,end;
 
	

 public:

  Whisker() { //default constructor
    inputsignal = 0;
    time(&timer); 
  }

  void in(int signal) {
    //printf("inputsignal: %d\n",signal);
    inputsignal=signal;
	

    //reset timedifference
    time(&timer);
    //printf("timer1: %d \n", timer);
  }

  int state() {
    
    //whisker.in hasn't been called in over a second  -> steady state
    if ((time(&end)-timer)>=1) {	  
      //printf("timer:%d , clock: %d , difference:%d \n ",timer,time(&end), time(&end)-timer);
      return inputsignal;
			
  }

  else{ //whisker.in is being called constantly
    return 2; //must be vibrating
  }

}




};

