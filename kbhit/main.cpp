#include<iostream>
#include<cassert>
#include<functional>
#include<stdio.h>
#include<sys/select.h>
#include<termios.h>
#include<stropts.h>
#include<sys/ioctl.h>


int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

class KeyboardInput{
  public:
    enum KeyCode{
      KEYCODE_a,
      KEYCODE_b,
      KEYCODE_c,
      KEYCODE_d,
      KEYCODE_e,
      KEYCODE_f,
      KEYCODE_g,
      KEYCODE_h,
      KEYCODE_i,
      KEYCODE_j,
      KEYCODE_k,
      KEYCODE_l,
      KEYCODE_m,
      KEYCODE_n,
      KEYCODE_o,
      KEYCODE_p,
      KEYCODE_q,
      KEYCODE_r,
      KEYCODE_s,
      KEYCODE_t,
      KEYCODE_u,
      KEYCODE_v,
      KEYCODE_w,
      KEYCODE_x,
      KEYCODE_y,
      KEYCODE_z,
      KEYCODE_space,
    };
    KeyboardInput(std::function<void(KeyCode const&code)>const&clbk = nullptr):_callback(clbk){}
    void operator()(){
      assert(this != nullptr);
      if(!_kbhit())return;
      KeyCode const code = getKeyCode();
      if(this->_callback)this->_callback(code);
    }
    KeyCode getKeyCode(){
      assert(this != nullptr);
      int currentChar = getchar();
      std::cout << currentChar << std::endl;
      if(islower(currentChar))
        return static_cast<KeyCode>(KEYCODE_a+currentChar-'a');
      if(currentChar == ' ')
        return KEYCODE_space;
      return KEYCODE_z;
    };
  protected:
    std::function<void(KeyCode const&code)>_callback = nullptr;
};

int main() {
  KeyboardInput checkKeyboard([](KeyboardInput::KeyCode const&c){
    switch(c){
      case KeyboardInput::KEYCODE_a:
        std::cout << "doleva" << std::endl;
        break;
      case KeyboardInput::KEYCODE_b:
        std::cout << "doprava" << std::endl;
        break;
      default:
        break;
    }
  });
  do{
    checkKeyboard();
  }while(true);

  return 0;
} 
