#include "World.h"

ServerWorld::ServerWorld(Server *srvr) {
	this->engine = srvr;
	//this->circus = new CircularBuffer<wchar_t>(1024);
}

ServerWorld::~ ServerWorld() {
	//delete this->circus;
}

int ServerWorld::scroll_till_opcode(CircularBuffer<wchar_t> &buffer) {
	wchar_t peek_buf[7] = {0};
	while (!buffer.is_peek_eof()) {
		buffer.peek_str(peek_buf,7);
		if (buffer.is_peek_eof()) {
			break;
		}
		bool still_opcode = true;
		for (int i=0; i < 6; i++) {
			still_opcode = still_opcode && iswdigit(peek_buf[i]);
		}
		still_opcode = still_opcode && iswspace(peek_buf[6]);
		if (still_opcode) {
			return 1;
		}
		buffer.read(NULL,1);
	}
	return 0;
}

void *ServerWorld::game_server(void *raw_data) {
	ServerWorld *world_engine = (ServerWorld *)raw_data;
	ServerNetwork *network_engine = world_engine->engine->network;


	struct timeval timeout;
	
	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	char plain_buffer[1024];
	int nbytes;
	
	while (true) {
		
		read_fds = network_engine->fd_accepted_socks;
		
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000 * 1000;
		
		pn_select(network_engine->fd_accepted_socks_max + 1, &read_fds, NULL, NULL, &timeout);
		for (int i=0; i <= network_engine->fd_accepted_socks_max; i++) {
			if (!FD_ISSET(i,&network_engine->fd_accepted_socks)) continue;
			if (network_engine->connections[i].isLogged == false) {
				network_engine->connections[i].isLogged = true;
				pnh_send_str(i,network_engine->nmsgs->get("login_successful"));
			}
			if ((nbytes = recv(i,plain_buffer,sizeof(plain_buffer)-1,0)) <= 0) {
				pn_close(i);
				FD_CLR((unsigned int)i,&network_engine->fd_accepted_socks);
				printf("[%s] Connection closed by client.\n",network_engine->connections[i].address_literal);
				network_engine->connections.erase(i);
			} else {
				plain_buffer[nbytes] = '\0';
				pio_string pio_buffer(plain_buffer);
				CircularBuffer<wchar_t> &circus = *network_engine->connections[i].circus;
				circus.write(pio_buffer.w_str(),pio_buffer.length());
				int ret;
				ret = world_engine->scroll_till_opcode(circus);
				if (!ret) {
					pio_buffer.weedout_control();
					printf("[%s] invalid query \"%s\".\n",network_engine->connections[i].address_literal,pio_buffer.c_str());
					//network_engine->connections.erase(i);
					//FD_CLR((unsigned int)i,&network_engine->fd_accepted_socks);
					pnh_send_fstr(i,1024,network_engine->nmsgs->get("invalid_query"),pio_buffer.c_str());
					//pn_close(i);
					continue;
				}
				int opcode = 0;
				wchar_t wchar_buffer[1024] = {0};
				circus.read(wchar_buffer,7);
				swscanf(wchar_buffer,L"%d ",&opcode);
				
				wint_t ch = circus.peek(0);
				int i;
				for (i=0; i < 1023 && ch != '\n'; i++) {
					circus.read(wchar_buffer+i,1);
					ch = circus.peek(0);
				}
				wchar_buffer[i] = '\0';
				
				printf("%d >%s<\n",opcode,pio_string(wchar_buffer).c_str());
			}
		}
	}
	pthread_exit(NULL);
	return NULL;
}

void ServerWorld::setup_world() {
	pthread_t game_server_thread;
	pthread_create(&game_server_thread,NULL,game_server,this);
	printf("Game server fired up\n");

	pthread_join(game_server_thread,NULL);


	//	pthread_exit(NULL);
}
