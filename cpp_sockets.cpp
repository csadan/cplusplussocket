#include <iostream>
#include <cstring>      // Needed for memset
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions

/*
struct addrinfo {
    int              ai_flags;     // This  field  specifies additional options.
    int              ai_family;    // IPv4, IPv6 or IP agnostic.
    int              ai_socktype;  // TCP or UDP.
    int              ai_protocol;  // The protocol for the returned socket addresses.
    size_t           ai_addrlen;   // Size of ai_addr in bytes
    struct sockaddr *ai_addr;      // Containing the IP address and port.
    char            *ai_canonname; // The Canonical hostname.

    struct addrinfo *ai_next;      // linked list, next address.
};

*/
using namespace std;
int main()
{

  int status;
  struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
  struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.

  // The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
  // to by hints must contain either 0 or a null pointer, as appropriate." When a struct 
  // is created in C++, it will be given a block of memory. This memory is not necessary
  // empty. Therefor we use the memset function to make sure all fields are NULL.     
  memset(&host_info, 0, sizeof host_info);

  cout << "Setting up the structs..."  <<endl;

  host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
  host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.

  // Now fill up the linked list of host_info structs with google's address information.
  status = getaddrinfo("www.ehu.es", "80", &host_info, &host_info_list);
  /*
    int getaddrinfo(const char *node, const char *service,const struct addrinfo *host_info, struct addrinfo **res);
    The parameters:
      node : The host you want to connect to. This can be a hostname or IP address.
      service : This is the port number you want to connect to. Usually an integer, but can also be a known service name like 'http'.
      host_info : Points to the addrinfo struct to fill.
      res : Points to the linked list of filled addrinfo structs.
      return value : The function returns 0 if all succeeded or a non-zerro error code in case of an error. 
  */

  // getaddrinfo returns 0 on succes, or some other value when an error occured.
  // (translated into human readable text by the gai_gai_strerror function).
  if (status != 0)  cout << "getaddrinfo error" << gai_strerror(status) ;

  cout << "Creating a socket..."  << endl;
  int socketfd ; // The socket descripter
  socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
//int socket(int domain, int type, int protocol);  
  if (socketfd == -1)  cout << "socket error " ;

  cout << "Connecting..."  << endl;
  status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 
  if (status == -1)  cout << "connect error" ;
  
  cout << "sending message..."  << endl;
  char *msg = "GET / HTTP/1.1\nhost: www.ehu.es\n\n";
  int len;
  ssize_t bytes_sent;
  len = strlen(msg);
  bytes_sent = send(socketfd, msg, len, 0);
  // ssize_t recv(int sockfd, void *buf, size_t len, int flags); 


  cout << "Waiting to recieve data..."  << endl;
  ssize_t bytes_recieved;
  char incoming_data_buffer[1000];
  bytes_recieved = recv(socketfd, incoming_data_buffer,1000, 0);
  // If no data arrives, the program will just wait here until some data arrives.
  if (bytes_recieved == 0) cout << "host shut down." << endl ;
  if (bytes_recieved == -1)cout << "recieve error!" << endl ;
  cout << bytes_recieved << " bytes recieved :" << endl ;
  cout << incoming_data_buffer << endl;


}    