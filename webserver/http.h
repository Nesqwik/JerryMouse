#ifndef __HTTP_H__
#define __HTTP_H__

void send_400_error(int fd_socket);
void send_200_response(int fd_socket) ;
void send_404_error(int fd_socket);

#endif
