#include "Messages.h"

/// @file

Messages::Messages(pio_string filename) {
	FILE *fh;
	fh = fopen(filename.c_str(),"r");
	
	short ret;
	wchar_t id[1024] = {0};
	wchar_t msg[1024] = {0};
	wchar_t num[7] = {0};
	while ((ret = fwscanf(fh,L"%1024l[a-z_] %6lc ",id,num)) > 0) {
		int i = 0;
		wint_t ch = '\0';
		while (ch != WEOF && ch != '\n') {
			ch = fgetwc(fh);
			if (i >= 1024) {
				while (ch != WEOF && ch != '\n')
					ch = fgetwc(fh);
			}
			msg[i++] = ch;
		}
		msg[i] = '\0';
		pio_string pio_num(num);
		pio_string pio_msg(msg);
		pio_string pio_resulting_message = pio_num + " ";
		pio_resulting_message = pio_resulting_message + pio_msg;
		
		this->messages[pio_string(id)] = pio_resulting_message;
		this->messages[pio_num] = pio_string(id);
		for (int i=0; i < 1024; i++) {
			id[i] = '\0';
			msg[i] = '\0';
		}
	}
	fclose(fh);
}
const char *Messages::get(pio_string id)  {
	return this->messages[id].c_str();
}
size_t Messages::length(pio_string id)  {
	return this->messages[id].length();
}
