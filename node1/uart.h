void uart_init(unsigned int ubrr);
void uart_transmit(unsigned char data);
unsigned char uart_receive(void);
void uart_send_string(const char *str);
void uart_recieved(void);