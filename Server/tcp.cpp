#include <stdio.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <fstream>

#define PORT 5001

using namespace std;

int main(int argc, char **argv)
{
  int serv_sock, clnt_sock;
  char message[1024];
  int n;
  size_t str_len;
  size_t InLen = strlen((char *)message);
  size_t OutLen = sizeof(message);
  char *Input = (char *)message;
  char *Output = message;

  iconv_t iConv = iconv_open("UTF-8", "EUC-KR");

  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
  {
    printf("bind error");
  }

  if (listen(serv_sock, 5) == -1)
  {
    printf("listen error");
  }

  clnt_addr_size = sizeof(clnt_addr);

  while (1)
  {
    memset(message, 0x00, sizeof(message));
    n = iconv(iConv, &Input, &InLen, &Output, &OutLen);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    printf("server connected!!\n");

    while ((str_len = read(clnt_sock, message, 1024)) != 0)
    {
      write(clnt_sock, message, str_len);
    }

    printf("Message: %s", message);
    ofstream fout("/home/hyun/navigation/src/navigation/src/socket.txt", ios::trunc);
    ofstream f2out("/home/hyun/navigation/src/navigation/src/order.txt");
    fout << message;
    f2out << message;
    fout.close();
    f2out.close();
    close(clnt_sock);
  }

  iconv_close(iConv);
  close(serv_sock);
  return 0;
}
