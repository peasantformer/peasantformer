#include "Messages.h"

/// @file

Messages::Messages(PString filename) {
	FILE *fh;
	fh = fopen(filename.c_str(),"r");
	
	short ret;
	wchar_t id[1024] = {0};
	wchar_t msg[1024] = {0};
	wchar_t num[7] = {0};
	while ((ret = fwscanf(fh,L"%1024l[a-z_] OP%6lc ",id,num)) > 0) {
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
		
		
		this->messages[PString(id)] = "OP" + PString(num) + " " + PString(msg);
		this->messages[PString(num)] = PString(id);
		for (int i=0; i < 1024; i++) {
			id[i] = '\0';
			msg[i] = '\0';
		}
	}
	fclose(fh);
}
const char *Messages::get(PString id)  {
	return this->messages[id].c_str();
}
size_t Messages::length(PString id)  {
	return this->messages[id].length();
}
