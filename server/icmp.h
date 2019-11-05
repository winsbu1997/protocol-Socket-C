#include <netinet/in.h>
#include <netinet/ip_icmp.h>

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
	char	Data[5];			// Data
}ICMPHDR, *PICMPHDR;

#define REQ_DATASIZE 27		// Echo Request Data size

// ICMP Echo Reply..
typedef struct tagECHOREPLY
{
	IPHDR ipHdr;
	ICMPHDR icmpHdr;
	unsigned long	dwTime;
	char	cData[REQ_DATASIZE];
	char cFiller[256];
}ECHOREPLY,*PECHOREPLY;
unsigned short checksum(unsigned short *b, int32_t size)
{
	int32_t len = size;
	unsigned short* buf = b;
	int32_t sum = 0;
	unsigned short result;

	for (; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}