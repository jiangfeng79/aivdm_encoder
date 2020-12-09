// aivdm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "aivdm.h"
#include "String.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char * msg = "!AIVDM,1,1,,A,15RTgt0PAso;90TKcjM8h6g208CQ,0*4A";
	char * msg2 = "!AIVDM,2,1,1,A,55?MbV02;H;s<HtKR20EHE:0@T4@Dn2222222216L961O5Gf0NSQEp6ClRp8,0*1C";
	//char * msg2 = "!AIVDM,2,1,1,A,55?MbV02;H;s<HtKP00EHE:0@T4@Dl0000000016L961O5Gf0NSQEp6ClRh0,0*0E";
	char * msg3 = "!AIVDM,2,2,1,A,88888888880,2*25";
	//char * msg3 = "!AIVDM,2,2,1,A,00000000000,2*25";
	struct aivdm_context_t ais_context;
	struct ais_t ais;
	char out1[256], out2[256];

	aivdm_decode(msg, strlen(msg),&ais_context, &ais);

	aivdm_encode(&ais, out1, out2);
	printf("msg 1, original msg is: %s\n", msg);
	printf("msg 1,encode result is: %s\n", out1);

	aivdm_decode(msg2, strlen(msg2),&ais_context, &ais);
	aivdm_decode(msg3, strlen(msg3),&ais_context, &ais);


	aivdm_encode(&ais, out1, out2);
	printf("msg 2, original msg is: %s\n", msg2);
	printf("msg 2,encode result is: %s\n", out1);
	printf("msg 3, original msg is: %s\n", msg3);
	printf("msg 3,encode result is: %s\n", out2);
	getchar();
	return 0;
}

