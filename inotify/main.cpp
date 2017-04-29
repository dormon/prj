#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include<stropts.h>
#include<poll.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
  struct pollfd pfd;
  pfd.fd=open("a",O_RDONLY);
  pfd.events = POLLIN;

  int ret = poll(&pfd, 1, 50000);  // timeout of 50ms

  if (ret < 0) {
    fprintf(stderr, "poll failed\n");
  } else if (ret == 0) {
    // Timeout with no events, move on.
  } else {
    // Process the new event.
//    struct inotify_event event;
//    int nbytes = read(fd, &event, sizeof(event));
    // Do what you need...
  }

  close(pfd.fd);
  return 0;
}
