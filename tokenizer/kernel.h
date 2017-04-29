#pragma once

namespace ge{
  namespace core{
    /* there should not be text command interface
     * its instance should hold all variables types and methods
     * it should be able to run jobs in threads 
     * it should be able to terminate jobs
     * it should be able to load modules
     * it should be able to load so/dll plugins
     *
     * example:
     * i32 a = 32;
     * Window win;
     * loadPlugin("rtw.so");
     * run(win.mainLoop());
     * runInSandbox(weirdFunction());
     * runInThread(win.mainLoop());
     * win.bindKey("a",fce);
     * print a;
     * delete(a);
     * delete(win);
     * registerType(struct 2 i32 f32);
     * registerFce(...); //TODO
     * i32 asd.a=32; //add variable a into asd scope
     * set(a,100);
     * a=100;
     * variables();
     * compile("output.bin");
     * listenOpPort(27015); //only for private networks
     * listForSignals();
     *
     */
    class Kernel{
      public:

      protected:



    };
  }
}
