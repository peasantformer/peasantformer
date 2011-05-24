#ifndef PEASANTFORMER_Comm_NetworkMessages
#define PEASANTFORMER_Comm_NetworkMessages

#include <map>
#include <string>
#include <Proto/PlainIO/PlainIO.h>

class NetworkMessages {
	private:
		std::map<pio_string,pio_string>messages;
	public:
		NetworkMessages(const char *filename) {
			FILE *fh;
			fh = fopen(filename,"r");
			int ret;
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
//printf("%s | %s\n",pio_string(id).c_str(),pio_string(msg).c_str());
				for (int i=0; i < 1024; i++) {
					id[i] = '\0';
					msg[i] = '\0';
				}
			}
			//fscanf(fh,""
			fclose(fh);
		}
	public:
		const char *get(pio_string id)  {
			return this->messages[id].c_str();
		}
		size_t length(pio_string id)  {
			return this->messages[id].length();

		}
};

#endif
