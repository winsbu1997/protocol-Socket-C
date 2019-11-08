#include "main.h"
int rand_var=0;
char* Name[7]={"Hoang","Luan","Long","Duong","Hiep","Viet","*"};
char PRO[6]={'1','2','3','4', '5','6'};
int TCP1(int new_socket){
	printf("\nUsing TCP ......... \n");
	char sendbuf[DEFAULT_BUFLEN] ; 
	char recvbuf[DEFAULT_BUFLEN]={0};
	
	while(1){
		sleep(1);
		printf("Enter TCP the mesage: ");
		//char sendbuf[DEFAULT_BUFLEN] ={0}; 
		//scanf("%s",sendbuf);
		strcpy(sendbuf,Name[rand()%7]);
		printf("%s\n",sendbuf);
		int sendResult= send(new_socket , sendbuf , strlen(sendbuf) , 0 );
		if(sendResult>0){	
			if(sendbuf[0]=='*'){
				return 0;
			}
			memset(recvbuf, '\0', DEFAULT_BUFLEN);
			int readResult = read( new_socket , recvbuf, DEFAULT_BUFLEN); 
			if(readResult>0){
				printf("%s \n", recvbuf);
			}
			else
			{
				exit(EXIT_FAILURE);
			}						
		}
		else
		{
			exit(EXIT_FAILURE);
		}		
	}
}
int HTTPS(){
	int flag = 0;
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();

	SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
	if (!ctx) {
		fprintf(stderr, "SSL_CTX_new() failed.\n");
		return 1;
	}


	if (!SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM)
		|| !SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM)) {
		fprintf(stderr, "SSL_CTX_use_certificate_file() failed.\n");
		ERR_print_errors_fp(stderr);
		return 1;
	}


	SOCKET server = create_socket(0, HTTPS_PORT);


	while (1) {

		fd_set reads;
		reads = wait_on_clients(server);

		if (FD_ISSET(server, &reads)) {
			struct client_info *client = get_client(-1);

			client->socket = accept(server,
				(struct sockaddr*) &(client->address),
				&(client->address_length));

			if (!ISVALIDSOCKET(client->socket)) {
				fprintf(stderr, "accept() failed. (%d)\n",
					GETSOCKETERRNO());
				return 1;
			}


			client->ssl = SSL_new(ctx);
			if (!ctx) {
				fprintf(stderr, "SSL_new() failed.\n");
				return 1;
			}

			SSL_set_fd(client->ssl, client->socket);
			if (SSL_accept(client->ssl) != 1) {
				//SSL_get_error(client->ssl, SSL_accept(...));
				ERR_print_errors_fp(stderr);
				drop_client(client);
			}
			//else {
				//printf("New connection from %s.\n", get_client_address(client));

				//printf("SSL connection using %s\n", SSL_get_cipher(client->ssl));
			//}
		}


		struct client_info *client = clients;
		while (client) {
			struct client_info *next = client->next;
			if (FD_ISSET(client->socket, &reads)) {

				if (MAX_REQUEST_SIZE == client->received) {
					send_400(client);
					continue;
				}

				int r = SSL_read(client->ssl,
					client->request + client->received,
					MAX_REQUEST_SIZE - client->received);

				if (r < 1) {
					printf("Unexpected disconnect from %s.\n",
						get_client_address(client));
					drop_client(client);

				}
				else {
					client->received += r;
					client->request[client->received] = 0;

					char *q = strstr(client->request, "\r\n\r\n");
					if (q) {
						*q = 0;

						if (strncmp("GET /", client->request, 5)) {
							send_400(client);
						}
						else {
							char *path = client->request + 4;
							char *end_path = strstr(path, " ");
							if (!end_path) {
								send_400(client);
							}
							else {			
								char* receive = strtok(path, "\r\n");
								int dem = 0;
								while (receive != NULL)
								{
									if(dem++ == 2) printf("Received from client: %s\n", receive);

									receive = strtok(NULL, "\r\n");
								}
								char msg[32] ={0};
								strcpy(msg,Name[rand()%7]);								
								serve_resource(client, path,msg);
								if(msg[0]=='*')
								{
									printf("\nClosing socket...\n");
									CLOSESOCKET(server);
									SSL_CTX_free(ctx);

									printf("Finished.\n");
									return 0;
								}
								*end_path = 0;
							}
						}
					} 
				}
			}
			client = next;
		}
	} 
	printf("\nClosing socket...\n");
	CLOSESOCKET(server);
	SSL_CTX_free(ctx);

	printf("Finished.\n");
	return 0;
}

int Protocol(){
	// Creating socket file descriptor 
	struct sockaddr_in address ; 
	int server_fd, new_socket, valread, opt=1; 
	int addrlen = sizeof(address); 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 27015 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 5) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	while(1)
	{
		//printf("In while in Protocol \n");
		char cmd[1000] ={0} ; 
		printf("Enter the Protocol (1:TCP, 2: UDP, 3: HTTP, 4: DNS, 5: ICMP, 6: HTTPS): ");
		//scanf("%s", cmd);
		cmd[0]=PRO[(rand_var++)%6];
		sleep(1);
		int sendResult= send(new_socket , cmd , strlen(cmd) , 0 );
		if(sendResult >0){
			char recvbuf[DEFAULT_BUFLEN]={0};
			int recvResult = recv( new_socket , recvbuf, DEFAULT_BUFLEN,0); 
			if(recvResult >0){
				if(recvbuf[0]=='1'){
					if(cmd[0]=='1'){
						TCP1(new_socket);
						///printf("Sau TCP1 \n");
						//return 1;
					}
					else{
						///printf("AAAAAAAAAAAA");
						memset(&address, 0, sizeof(address));
						close(server_fd);
						return cmd[0]-'0';
					}
					
				}
				else if(recvbuf[0]=='9')
				{
					return 0;
				}
				
			}
			else{
				printf("Socket error %d", recvResult);
				//exit(EXIT_FAILURE);
			}
			
		}
		else{
			exit(EXIT_FAILURE);
		}
	}
}

int UDP(){
	//struct sockaddr_in servaddr, cliaddr;
	printf("\nUsing UDP .....\n");
	int sockfd; 
    char recvbuf[DEFAULT_BUFLEN]; 
    char *hello = "Hello from server UDP"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT_UDP); 
      
      // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    while (1)
    {
		sleep(1);
        int len= sizeof(cliaddr);
		memset(recvbuf, '\0', DEFAULT_BUFLEN);

        int recvResult = recvfrom(sockfd, (char *)recvbuf, DEFAULT_BUFLEN, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
		if(recvResult<0){
			printf("Error from Recevfrom");
		}
        printf("Client : %s\n", recvbuf); 
		printf("Server message: ");
		char sendbuf[DEFAULT_BUFLEN];
		memset(sendbuf, '\0', DEFAULT_BUFLEN);
		//scanf("%s", sendbuf);
		strcpy(sendbuf,Name[rand()%7]);
        int sendResult= sendto(sockfd, (const char *)sendbuf, strlen(sendbuf), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len); 
		if(sendResult <0){
			printf("error from sendto");
		}
		if(sendbuf[strlen(sendbuf)-1]=='*'){
			memset(&servaddr, 0, sizeof(servaddr));
			memset(&cliaddr, 0, sizeof(cliaddr));
			close(sockfd);
			return 0;
		}
        
    }
    
}

int HTTP(){
	printf("\nUsing HTTP .....\n");
	int server_fd, new_socket; long valread;
    	struct sockaddr_in address;
    	int addrlen = sizeof(address);
	char cmd[DEFAULT_BUFLEN]={0};
	char sendbuf[DEFAULT_BUFLEN]={0};

    // Only this line has been changed. Everything is same.
    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 17\n\nHTTP Message from SERVER";
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
	
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
	
    address.sin_port = htons( HTTP_PORT );
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    int reuse = 1;
   if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
       perror("setsockopt(SO_REUSEADDR) failed");
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
	
        perror("In bind1");
        exit(EXIT_FAILURE);
    }
	printf("\nTest HTTP .....\n");
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
	

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
	{
		perror("In accept");
		exit(EXIT_FAILURE);
	}

    while(1)
    {
		sleep(1);
		memset(sendbuf, '\0', DEFAULT_BUFLEN);
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
		if(valread>0){
			printf("%s\n",buffer );
		}
		else
		{
			printf("Read error");
		}		
        

		// noi message nhap vao voi hello
		printf("Enter the message: ");
		//scanf("%s", cmd);
		strcpy(cmd,Name[rand()%7]);
		for(int i=0; i< strlen(cmd); i++ ){
			sendbuf[i]= cmd[i];
		}
		// for(int i=0; i< strlen(cmd); i++ ){
		// 	sendbuf[i+strlen(hello)]= cmd[i];
		// }
		printf("%s \n\n",sendbuf);

        write(new_socket , sendbuf , strlen(cmd));
		if(cmd[strlen(cmd)-1]=='*'){
			close(new_socket);
		 	close(server_fd);
			return 1;
		}
    }
	//close(new_socket);
	close(server_fd);
	return 0;
}
int findString(char *s)
{
	int end=0;
	int temp=0;
	int n=(int)strlen(s);
	for(int i=0;i<n;i++)
	{
		if(s[i]=='.')
		 temp=i;
	}
	if(s[temp+1]=='c'||s[temp+1]=='n')
		end=temp+3;
	else end=temp+2;
	return end;
	
}
int DNS(){
	char *IPbuffer; 
	int server_fd, new_socket; 
	long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	
	char sendbuf[DEFAULT_BUFLEN]={0};

	struct hostent *host_entry; 
	int hostname; 

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( DNS_PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
	int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
       perror("setsockopt(SO_REUSEADDR) failed");
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
	{
		perror("In accept");
		exit(EXIT_FAILURE);
	}

	while(1){
		sleep(1);
		//memset(IPbuffer, '\0', DEFAULT_BUFLEN);
		char cmd[DEFAULT_BUFLEN]={0};
		char buffer[DEFAULT_BUFLEN]={0};
		valread = read( new_socket , buffer, 1024);
		//tach chuoi dau vao  c1--hostname   c2--messenger
		int vitri=findString(buffer);
		int dodai=(int)strlen(buffer);
		char c1[1024]={0};
		char c2[1024]={0};
		for(int i=0;i<dodai;i++)
		{
			if(i<=vitri) c1[i]=buffer[i];
			else
			{
				c2[i-vitri-1]=buffer[i];
			}
			
		}
		// To retrieve host information 
		host_entry = gethostbyname(c1); 
		if (host_entry == NULL) 
		{ 
			perror("gethostbyname"); 
			exit(1); 
		} 

		// To convert an Internet network 
		// address into ASCII string 
		IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])); 

	
		printf("Hostname: %s\n", c1); 
		printf("Host IP: %s\n", IPbuffer); 
		printf("Noi dung tin nhan: %s\n", c2); 

		memset(sendbuf, '\0', DEFAULT_BUFLEN);
		//memset(recvbuf, '\0', DEFAULT_BUFLEN);
		printf("Enter the message: ");
		//scanf("%s", cmd);
		strcpy(cmd,Name[rand()%7]);
		// for(int i=0; i< strlen(IPbuffer); i++ ){
		// 	sendbuf[i]= IPbuffer[i];
		// }
		for(int i=0; i< strlen(cmd); i++ ){
			sendbuf[i]= cmd[i];
		}
		printf("%s \n\n",cmd);
		write(new_socket ,sendbuf, strlen(sendbuf));
		if(sendbuf[strlen(sendbuf)-1]=='*'){
			close(new_socket);
			close(server_fd);
			return 1;
		} 
	}
}
// socket() create an endpoint for comunication and return a file description
//// that refers thi that endpoint

//setsockopt() manipulate options for the socket refered to by the file description

//bind() assigns the address specified by addr to the socket refered by file description

//listen() listen for connection on a socket

//accept() accept a connection on a socket
void Ping(const char *ip)
{
	struct in_addr dst;
	inet_aton(ip, &dst);
    struct icmphdr icmp_hdr;
    struct sockaddr addr;
	struct sockaddr_in *to = (struct sockaddr_in *) &addr;
	struct sockaddr_in from;
	int fromsize = sizeof(from);
    int sequence = 0;
    int bg  = 123;
    int sock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
    if (sock < 0) {
        perror("socket");
        return ;
    }

    while(1) {
        sleep(1);
        unsigned char data[2048];
        int rc;
        struct timeval timeout = {3, 0}; //wait max 3 seconds for a reply
        fd_set read_set;
        socklen_t slen;
        memset(&addr, 0, sizeof addr);
        to->sin_family = AF_INET;
        to->sin_addr = dst;

        memset(&icmp_hdr, 0, sizeof icmp_hdr);
        icmp_hdr.type = ICMP_ECHO;
        icmp_hdr.un.echo.sequence = sequence++;
        icmp_hdr.un.echo.id = bg++;
        memcpy(data, &icmp_hdr, sizeof icmp_hdr);
        char msg[32] ={0};
        printf("\nGui cho client: ");
        //scanf("%s", msg);
		strcpy(msg,Name[rand()%7]);
        //msg[strlen(msg)] = '\0';
		printf("%s\n", msg);
        memcpy(data + sizeof icmp_hdr, msg, strlen(msg) + 1); //icmp payload
        icmp_hdr.checksum = checksum((unsigned short*)data, sizeof(icmp_hdr) + strlen(msg) + 1);
        memcpy(data, &icmp_hdr, sizeof icmp_hdr);
        
        addr.sa_family=AF_INET;
        rc = sendto(sock, data, sizeof icmp_hdr + strlen(msg) + 1, 0, (struct sockaddr*)&addr, sizeof addr);
        if (rc <= 0) {
            perror("Sendto");
            break;
        }
		if(msg[0]=='*')
		{
			printf("end icmp\n");
			close(sock);
			return;
		}

		ECHOREPLY echoReply;

		int nRet;
		int nRet1 = recvfrom(sock,&echoReply,sizeof(ECHOREPLY),0,(struct sockaddr *)&from,
				&fromsize);
		//char on=1;
		//setsockopt(rawSocket,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on));
		if((nRet = recvfrom(sock,&echoReply,sizeof(ECHOREPLY),0,(struct sockaddr *)&from,
				&fromsize)) > 0)
		{          
			printf("Nhan duoc tu client: "); printf("%s",echoReply.icmpHdr.Data);
		}
    }
}

int main(int argc, char const *argv[]) 
{ 

	//DNS();
	int i, n;
   time_t t;
   
   /* Khoi tao bo sinh so ngau nhien */
   srand((unsigned int) time(&t));
	while (1)
	{
		int protocol= Protocol();
		printf("Protocol %d: \n", protocol);
		if(protocol==2){
			UDP();			
		}
		else if(protocol==3)
		{
			HTTP();
		}
		else if(protocol==4){
			DNS();
		}
		else if(protocol==5){
			Ping("192.168.8.101");
		}
		else{
			HTTPS();
		}
	}
	
	return 0; 
}
