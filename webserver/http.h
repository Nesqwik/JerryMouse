#ifndef __HTTP_H__
#define __HTTP_H__

void send_status(FILE* client, int code, const char* reason_phrase);
void send_response(FILE* client, int code, const char* reason_phrase, const char* message_body);
void send_header(FILE* client, int code, const char* reason_phrase, int fd_message, char* type);
void send_stats(FILE* client);

#endif
