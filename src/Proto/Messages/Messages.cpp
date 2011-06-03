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
		
		PString psid(id);
		PString nsid(num);
		messages[psid] = PString(msg);
		numbers[psid] = nsid;
		ids[nsid] = psid;
		
		for (int i=0; i < 1024; i++) {
			id[i] = '\0';
			msg[i] = '\0';
		}
	}
	fclose(fh);
}
PString Messages::get(PString id)  {
	return messages[id];
}

PString Messages::get_num(PString id) {
	return "OP" + numbers[id] + " " + messages[id];
}

