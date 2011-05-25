#include "Messages.h"

Messages::Messages(pio_string filename) {
	FILE *fh;
	fh = fopen(filename.c_str(),"r");
	
	short ret;
	wchar_t id[1024] = {0};
	wchar_t msg[1024] = {0};
	while ((ret = fwscanf(fh,L"%1024l[a-z_] ",id)) > 0) {
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
		msg[i-1] = '\0';
		this->messages[pio_string(id)] = pio_string(msg);
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
