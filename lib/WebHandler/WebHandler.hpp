#ifndef __WEB_HANDLER_H__
#define __WEB_HANDLER_H__

class WebHandler
{
private:
  bool initialized;

public:
  WebHandler();
  void setup();
};

extern WebHandler webHandler;

#endif
