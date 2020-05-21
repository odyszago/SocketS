#include <iostream>
#include <istream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
using namespace std;
int main(int argc, char const *argv[]) {

  //create sock
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock == -1){
    cerr << "Problem with socket"<< '\n';
    return -1;
  }
//seting port
//creating hint structure for the server
  int port = 55000;
  string ipAddress = "127.0.0.1";

  sockaddr_in hint ;

  hint.sin_family = AF_INET;
  hint.sin_port = htons(port);
  inet_pton(AF_INET,ipAddress.c_str(),&hint.sin_addr);//127.0.0.1, 0.0.0.0 any adress

  int connection =connect(sock,(sockaddr *)&hint,sizeof(hint));
  if(connection == -1){
    cerr << "Error " << '\n';
    return -2;
  }

//loop
  char buf[4096];
  string userInput;

  do {

      cout <<">";
      getline(cin,userInput);
      int sends = send(sock,userInput.c_str(),userInput.size()+1,0);

      if(sends == -1){
        cerr << "Error sending" << '\n';
        continue;
      }
      memset(buf,0,4096);
      int bytesRecv = recv(sock,buf,4096,0);

      cout << "Server " << string(buf,bytesRecv) << '\n';

  } while(true);


  close(sock);
  return 0;
}
