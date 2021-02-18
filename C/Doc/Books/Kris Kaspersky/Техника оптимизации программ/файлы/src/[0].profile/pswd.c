/*----------------------------------------------------------------------------
 *
 *				������, ��������������� ������� �����������
 *			� ������������ � ������� �������������� Intel VTune
 *			====================================================
 *
 * Build 0x001 12.08.2002
----------------------------------------------------------------------------*/
// ��� ������ ����, ��� �� ����� ������ ���������! ����� ��������  ���������
// ������, ��������� ������������������. ������������ ��������� ����� �� ���
// --------------------------------------------------------------------------

// ������������
#define ITER 100000						// ����. ��������
#define MAX_CRYPT_LEN	200				// ����. ����� �����������

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
//#include <memory.h>
#include <malloc.h>

/*----------------------------------------------------------------------------
 *
 *				��������� ����������� ����������� ��������� �������
 *				---------------------------------------------------
 *	ARG:
 *		pswd		:	������
 *		crypteddata	:	����������
 *
 *	RET:
 *		*crypreddata:	�������������� ���������
 *
 *	NOTE:
 *		none
-----------------------------------------------------------------------------*/
DeCrypt(char *pswd, char *crypteddata)
{
	unsigned int p = 0;	// ��������� ������� ������� ���������������� ������

	// * * * �������� ���� ����������� * * *
	do {
		// �������������� ������� ������
		crypteddata[p] ^= pswd[p % strlen(pswd)];
		// ��������: ������ ������� ^^^ _�����_ ��������� ��������
		// ��� �� ����� ����� �� ����� ������������  ������������,
		// �.�. ��� ������� ����������  ������ ����� (����  ������
		// ����������

		// ��������� � ����������� ���������� �������
	} while(++p < strlen(crypteddata));

	return 0;
}


/*----------------------------------------------------------------------------
 *
 *				��������� ���������� ����������� ����� ������
 *				---------------------------------------------
 *	ARG:
 *		pswd		:	������
 *
 *	RET:			CRC ������� ������
 *
 *	NOTE:
 *		none
-----------------------------------------------------------------------------*/
int CalculateCRC(char *pswd)
{
	unsigned int a;
	int x = -1;			// ������ ���������� CRC

	// �������� ���������� CRC, �������, ������ ��� ��������, �� ������  ���
	// ��   ������,   -   ���    ��������   �������������  ���  ����,  �����
	// ����������������� missaling
	for (a = 0; a <= strlen(pswd);  a++)
				/* ^^^^^^^^^^^	������� ���������� ������ ������� ���� �����
								�� ������� �����, �� �� ��� ���������� ��  �
								���� ������ ������, ��� �� ������ � ��������
				*/
		x += *(int *)((int)pswd + a);
		/*	^^^^^^^^^ pswd ������ ���� ��������! */

	return x;
}

/*----------------------------------------------------------------------------
 *
 *				��������� �������� ����������� ����� ������
 *				---------------------------------------------
 *	ARG:
 *		pswd		:	������
 *		validCRC	:	�������� CRC
 *
 *	RET:			0	������ �� ��������
 *					!=0	validCRC
 *
 *	NOTE:
 *		none
---------------------------------------------------------------------------*/
int CheckCRC(char *pswd, int validCRC)
{
	if (CalculateCRC(pswd) == validCRC)
		return validCRC;
	// else
		return 0;
}


/*----------------------------------------------------------------------------
 *
 *				��������� ��������� �������� ������
 *				-----------------------------------
 *	ARG:
 *		crypteddata	:	������������� ������, ������� ���� ������������
 *		pswd		:	������� ������
 *		validCRC	:	�������� CRC
 *		progress	:	������� ��� ����������� �������
 *
 *	RET:
 *		none
 *
 *	NOTE:
 *		������� do_pswd ��������� CRC ������������ ������ �, ���� ��� �����,
 * �������� ������������ ���� ������� ����������, ������ �������� ����������
 * �� �����.
 *		��� �� ��������� ������� ������������� ������� � ������� �����������
 * ������; ��! ������ �� ����, ��� � ��������� ��������� ������� �������!!!
-----------------------------------------------------------------------------*/
do_pswd(char *crypteddata, char *pswd, int validCRC, int progress)
{
	char *buff;

	// ����� �������� ��������� �� ��������
	printf("Current pswd : %10s [%d%%]\r",&pswd[0],progress);

	// �������� CRC ������
	if (CheckCRC(pswd, validCRC))
	{								// <- CRC �������

		// �������� ����������� �� ��������� �����
		buff = (char *) malloc(strlen(crypteddata));
		strcpy(buff, crypteddata);

		// ��������������
		DeCrypt(pswd, buff);
	
		// ������� ��������� ����������� �� �����
		printf("CRC %8X: try to decrypt: \"%s\"\n",
											CheckCRC(pswd, validCRC),buff);
	}

	return 0;
}


/*----------------------------------------------------------------------------
 *
 *						��������� �������� �������
 *						--------------------------
 *	ARG:
 *		crypteddata	:	������������� ������, ������� ���� ������������
 *		pswd		:	������, � �������� �������� ������� �������
 *		max_iter	:	���. ���-�� ������������ �������
 *		validCRC	:	�������� CRC
 *
 *	RET:
 *		none
 *
 *	NOTE:
 *		������� do_pswd ��������� CRC ������������ ������ �, ���� ��� �����,
 * �������� ������������ ���� ������� ����������, ������ �������� ����������
 * �� �����.
 *		��� �� ��������� ������� ������������� ������� � ������� �����������
 * ������; ��! ������ �� ����, ��� � ��������� ��������� ������� �������!!!
-----------------------------------------------------------------------------*/
int gen_pswd(char *crypteddata, char *pswd, int max_iter, int validCRC)
{
	int a;
	int p = 0;

	// ������������ ������
	for(a = 0; a < max_iter; a++)
	{
		// ���������� ������� ������
		do_pswd(crypteddata, pswd, validCRC, 100*a/max_iter);
		/*	��������: �������  ��� ����� ��������� ^^^ �������� � ��  ������
			������� ������� � ���!	*/

		// * �������� ���� ��������� ������� *
		// �� ��������� "�������" ��� "�������"
		while((++pswd[p])>'z') 
		{	/* ^^^^ -	����������� ������  ������ ������  ��  ���� */
			/*			���� �� ������������ �� 'z' - ������ � ���� */
			/*			����������, ���� ���� ����� ��� ����, ����� */
			/*			������������� �� ������������� ����. ����.  */

			// ������������ �� 'z' ������ - � ��������� ���������
			pswd[p] = '!';
			
			// ��������� ������
			p++; if (!pswd[p])
			{					// <--	������ ���������  ���������� �������
								//		���� �� ����� ���� �.�. ����� ������
								//		��   ���������  ������������  ������

			pswd[p]=' ';		//		��������! ������������ �� MIN_CHAR-1
								//		�.�. �� ������������� � ����� while!

			pswd[p+1]=0;		//		����� ����� ������!
			}
		} // end while(pswd)

		// ���������� ��������� �� �����
		p = 0;
	} // end for(a)

	return 0;
}

/*----------------------------------------------------------------------------
 *
 *				�������� ������� �����, �������� ������� �������
 *				------------------------------------------------
 *	ARG:
 *		per			:	����� ��� ������
 *
 *	RET:
 *		none
 *
 *	NOTE:
 *		������� ������� �� ����� �����, ������ ��� �� ����� �����
 *
-----------------------------------------------------------------------------*/
print_dot(float per)
{
	// * * * ������������ * * *
	#define N			3		// �������� �� ��� �������
								// ��� ������ HEX ���� ������� ���

	#define DOT_SIZE	1		// ������ �����-�����������

	#define	DOT			"."		// �����������
	
	int		a;
	char	buff[666];

	sprintf(buff,"%0.0f", per);
	/* ^^^^^^^^^^^^^^^^ ������ ������ */

	// * * * ���� ������ ����� �� �������� * * *
	for(a = strlen(buff) - N; a > 0; a -= N)		// <-- �������
	{ /* ^^^^^^^^^^^^^^^^ - ��� ������ ���, - �� ��������� ������� ����� */

			memmove(buff + a + DOT_SIZE, buff + a, 66);
			/* ��������!						^^^^^^^^^ */

			if(buff[a]==' ') break;	// ��������� ������ - ����� ������
				else
			// �������� �����������
			memcpy(buff + a, DOT, DOT_SIZE);
	}
	// �������� �� �����
	printf("%s\n",buff);

	return 0;
}


main(int argc, char **argv)
{
	// ����������
	FILE *f;				// ��� ������ ��������� ����� (���� ����)
	char *buff;				// ��� ������ ������ ��������� �����
	char *pswd;				// ������� ����������� ������ (need by gen_pswd)
	int validCRC;			// ��� �������� ������������� CRC ������
	unsigned int t;			// ��� ������ ������� ���������� ��������
	int iter = ITER;		// ����. ���-�� ������������ �������
	char *crypteddata;		// ��� �������� �����������

	//	build-in default crypt
	//	��� �������, ��� �����  �����������, ���  ��������  �������  �����
	//	���� ��������� ;)
	char _DATA_[] = "\x4B\x72\x69\x73\x20\x4B\x61\x73\x70\x65\x72\x73\x6B"\
					"\x79\x20\x44\x65\x6D\x6F\x20\x43\x72\x79\x70\x74\x3A"\
					"\xB9\x50\xE7\x73\x20\x39\x3D\x30\x4B\x42\x53\x3E\x22"\
					"\x27\x32\x53\x56\x49\x3F\x3C\x3D\x2C\x73\x73\x0D\x0A";

	// TITLE
	printf("= = = VTune profiling demo = = =\n==================================\n");

	// HELP
	if (argc==2)
	{
			printf("USAGE:\n\tpswd.exe [StartPassword MAX_ITER]\n");
			return 0;
	}
	
	// ��������� ������
	printf("memory malloc\t\t");
	buff = (char *) malloc(MAX_CRYPT_LEN);
	if (buff) printf("+OK\n"); else {printf("-ERR\n"); return -1;}

	// ��������� ����������� ��� �����������
	printf("get source from\t\t");
	if ((f=fopen("crypted.dat","r"))!=0)
	{
		printf("crypted.dat\n");
		fgets(buff,MAX_CRYPT_LEN, f);
	}
	else
	{
		printf("build-in data\n");
		buff=_DATA_;
	}

	// ��������� CRC
	validCRC=*(int *)((int) strstr(buff,":")+1);
	printf("calculate CRC\t\t%X\n",validCRC);
	if (!validCRC)
	{
		printf("-ERR: CRC is invalid\n");
		return -1;
	}

	// ��������� ����������� ������
	crypteddata=strstr(buff,":") + 5;
	//printf("cryptodata\t\t%s\n",crypteddata);

	// ��������� ������ ��� ���������� ������ 
	printf("memory malloc\t\t");
	pswd = (char *) malloc(512*1024); pswd+=62;
		/*	������������ ����������� ^^^^^^^^^^^ �������������  ������  */
		/*	������ ����� ����������� ���, ��� ��� ������� ����� ������  */
		/*	malloc ������ ����������� ����� �� 64 ��, ��� ��� � ����	*/

	memset(pswd,0,666);		// <-- ������������ 

	if (pswd) printf("+OK\n"); else {printf("-ERR\n"); return -1;}
	
	// ������ ���������� ��������� ������
	// ��������� ���������� ������ � ����. ���-�� ��������
	printf("get arg from\t\t");
	if (argc>2)
	{
		printf("command line\n");
		if(atol(argv[2])>0) iter=atol(argv[2]);
		strcpy(pswd,argv[1]);
	}
		else
	{
		printf("build-in default\n");
		strcpy(pswd,"!");
	}
	printf("start password\t\t%s\nmax iter\t\t%d\n",pswd,iter);
	

	// ������ �������� �������
	printf("==================================\ntry search... wait!\n");
	t=clock();
		gen_pswd(crypteddata,pswd,iter,validCRC);
	t=clock()-t;

	// ����� ���-�� ������������ ������� �� ���
	printf("                                       \rPassword per sec:\t");
	print_dot(iter/(float)t*CLOCKS_PER_SEC);

	return 0;
}