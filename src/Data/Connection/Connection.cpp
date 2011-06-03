#include "Connection.h"

void Connection::init() {
	sockfd = -1;
	buffsize = 0;
	circus = NULL;
	raw_buffer = NULL;
	address_literal[0] = '\0';
	raw_bytes = -1;
	opcode = 0;
}

Connection::Connection() {
	init();
}
Connection::Connection(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) {
	init();
	this->sockfd = sockfd;
	this->remote_addr = remote_addr;
	this->buffsize = buffsize;
	this->raw_buffer = new char[buffsize];
	this->plain_buffer.resize(buffsize);
	circus = new CircularBuffer(buffsize);
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
}

Connection::~Connection() {
	delete[] raw_buffer;
	delete circus;
}

Connection & Connection::operator=(Connection const &r) {
	if (&r == this) return *this;
	delete[] raw_buffer;
	delete circus;
	init();
	sockfd = r.sockfd;
	buffsize = r.buffsize;
	remote_addr = r.remote_addr;
	raw_buffer = new char[buffsize];
	plain_buffer.resize(buffsize);
	circus = new CircularBuffer(buffsize);
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
	return *this;
}

void Connection::parse_init() {
	this->opcode = 0;
	//circus->dump();
	circus->write(raw_buffer);
	plain_buffer = PString(raw_buffer);
	raw_buffer[0] = '\0';
	while (true) {
		//int i=2;
		PString res;
		//circus->scroll(1);
		while ((res = circus->peek(2)) != "O" && !circus->is_peek_eof()) {
			//printf(">> %s\n",res.c_str());
			circus->seek(-1);
			//circus->scroll(1);
		}
		//printf(">> %s\n",res.c_str());
		if (circus->is_peek_eof()) {
			circus->seek(-1);
			circus->dump();
			//circus->scroll(i);
			//circus->reset_peek();
			//circus->dump();
			return;
		}
		//circus->dump();
		//printf("%s\n",circus->read(6).c_str());
		break;
	}
	/*
	
	//printf("%s\n",circus->peek(10).c_str());
	bool valid = false;
	while (valid == false) {
		while (circus->peek(2) != "OP" && !circus->is_peek_eof())
			circus->read_ch();
		if (circus->is_peek_eof()) {
			return;
		}
		circus->read(2);
		bool alldigits = true;
		PString p = circus->peek(6);
		for (int i=0;i<6;i++) {
			alldigits = alldigits && iswdigit(p[i]);
			if (!alldigits) break;
		}
		if (alldigits) break;
	}
	swscanf(circus->read(6).w_str(),L"%d",&opcode);
	printf("%d\n",opcode);
	while(iswspace(circus->peek_ch()) && !circus->is_peek_eof())
		circus->read_ch();
	*/
	/*
	while(iswspace(circus->peek_ch()) && !circus->is_peek_eof())
		circus->read_ch();
	int i=0;
	while(circus->peek_ch() != L'\n' && !circus->is_peek_eof()) {
		printf("!! %s\n",plain_buffer.c_str());
		plain_buffer[i++] = circus->read_ch();
	}
		
	//circus->dump();

	//plain_buffer[i] = L'\0';
	//raw_buffer[0] = '\0';

	
	//printf("%d >%s<\n",opcode,plain_buffer.c_str());
	//circus->clear();
	*/
}
