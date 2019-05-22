int esp_gets(int fd, char *buf, int can_timeout);
void esp_puts(int fd, char *buf);
void at_drain(void) ;
int esp_connect(const char **portname, int timeout);
int at_response(void);
int at_response_fn(int (*fp)(char *)) ;
int at_cmd(char *cmd) ;
int at_cmd_fn(char *cmd, int (*fp)(char *)) ;

extern int at_fd;
int esp_async_gets(int fd, char *buf, unsigned nbytes);

char * remove_cr(char *p);
