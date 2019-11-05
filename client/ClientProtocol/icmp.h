// ICMPEchoServer.h : header file
//#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define REQ_DATASIZE 27
//#define ICMP_ECHOREPLY	0
//#define ICMP_ECHOREQ	8
#define SM_EVENT		WM_USER+192

#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma pack(1)

struct ICMP {
	unsigned char type;
	unsigned char code;
	USHORT cksum;
	USHORT id;
	USHORT seq;
}_ICMP;
typedef struct tagIPHDR
{
	u_char  version;	 // version number
	u_char	TOS;			// Type Of Service
	u_short	TotLen;			// Total Length
	u_short	ID;				// Identification
	u_short	FlagOff;		// Flags and Fragment Offset
	u_char	TTL;			// Time To Live
	u_char	Protocol;		// Protocol
	u_short	Checksum;		// Checksum
	struct	in_addr iaSrc;	// Internet Address - Source
	struct	in_addr iaDst;	// Internet Address - Destination
}IPHDR, *PIPHDR;

// ICMP Header - RFC 792
typedef struct tagICMPHDR
{
	u_char	Type;			// Type
	u_char	Code;			// Code
	u_short	Checksum;		// Checksum
	u_short	ID;				// Identification
	u_short	Seq;			// Sequence
	char	Data[32];			// Data
}ICMPHDR, *PICMPHDR;

#define REQ_DATASIZE 27		// Echo Request Data size

// ICMP Echo Reply..
typedef struct tagECHOREPLY
{
	IPHDR ipHdr;
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
	char cFiller[256];
}ECHOREPLY, *PECHOREPLY;

//ICMP Echo Request...
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;

//Here it is object client address...
sockaddr_in objClientAddress;
sockaddr_in objServAddress;
//Check sum is done for ICMP Request..
u_short in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nleft == 1) {
		u_short	u = 0;

		*(u_char *)(&u) = *(u_char *)w;
		sum += u;
	}

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */

	return (answer);
}


#pragma pack()



















//Here it sends the echo request..
//bool SendEchoRequest(SOCKET& objRawSocket);
//}ECHOREQUEST, *PECHOREQUEST;
//ICMP Echo Reply....
//char cFiller[256];
/*
typedef struct tagECHOREPLY
{

//struct	in_addr iaSrc;	// Internet Address - Source
//struct	in_addr iaDst;	// Internet Address - Destination
	//IPHDR ipHdr;
	ECHOREQUEST echoRequest;
	char cFiller[256];
}ECHOREPLY,*PECHOREPLY;
*/
