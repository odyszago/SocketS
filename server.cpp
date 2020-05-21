#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>


using namespace std;

int main(int argc, char const *argv[]) {

    //create socket
    int listening = socket(AF_INET,SOCK_STREAM,0);
    if(listening == -1){

      cerr << "Can not open socket" << '\n';
      exit(1);
    }


    //Bind socket to ip /port

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(55000);
    inet_pton(AF_INET,"0.0.0.0",&hint.sin_addr);//127.0.0.1, 0.0.0.0 any adress

    //Bind
    if(bind(listening,(sockaddr *)&hint,sizeof(hint)) == -1){
      cerr << "No Bind" << '\n';
      exit(2);
    }
   // mark socket to listen
   if(listen(listening,SOMAXCONN)==-1){
     cerr << "Cant listen" << '\n';
     exit(3);
   }
   //accept call
   sockaddr_in client;
   socklen_t clientsize = sizeof(client);
   char host[NI_MAXHOST];
   char svc[NI_MAXSERV];

   int clientSocket = accept(listening,
                          (sockaddr *)&client,
                          &clientsize);

  if(clientSocket == -1){
    cerr << "Provlem client connecting" << '\n';
    exit(4);
  }

//close the listening socket
  close(listening);

  memset(host,0,NI_MAXHOST);
  memset(svc,0,NI_MAXSERV);


  int result = getnameinfo((sockaddr*)&client,
                      sizeof(client),
                      host,NI_MAXHOST,
                      svc , NI_MAXSERV,
                      0);
  if (result){
    cout << host << " conncected on 1 " << svc <<'\n';
  }else{
     inet_ntop(AF_INET,&client.sin_addr,host , NI_MAXHOST);
     cout << host << " connected on 2 " << ntohs(client.sin_port) <<'\n';

  }

 char buf[4096];
 //receive mesg
 while(true){
   //clear buffer
   memset(buf,0,4096);
   int bytesRecv = recv(clientSocket,buf,4096,0);
   if(bytesRecv == -1){
     cerr << "Connection issue" << '\n';
     exit(5);
   }
   if(bytesRecv == 0){

     std::cout << "Client left" << '\n';
     break;
   }

   cout << "Received  " << string(buf,0,bytesRecv)<< '\n';

   //echo back
   string msg = "message received";
   send(clientSocket,msg.c_str(),msg.size()+1,0);
  }

   close(clientSocket);




    return 0;
  }
