/* ---------------------------------------------------------------------------
 * @
 *				������� ��������������� �������������
 *			������������	��������	�������		������
 *			==============================================
 *
 * Build 0x001 05.07.2002
--------------------------------------------------------------------------- */
// ������������
#define BLOCK_SIZE (4*M)

// ���-�� ��������
#define N (BLOCK_SIZE/sizeof(int))


#include <DoCPU.h>

// -[Simple byte-crypt]-------------------------------------------------------
//
//	ARG:
//		src		- ��������� �� ��������� ����
//		mask	- ����� ���������� (����)
//
//	README:
//		��� ���������� ���������� ������������������, ��������� ���� ������
// ���� �������� �� ������� 4 ����
//----------------------------------------------------------------------------
void unalign_crypt(char *src, int n, int mask)
{
	int a;
	int supra_mask;

	// ���������� �������� ����� ��� ��������� �������� �����	
	supra_mask = mask+(mask<<8)+(mask<<16)+(mask<<24);
	
	// ������������ ����� �������� �������
	for (a = 0; a < n; a += 4)
	{
		 *(int *)src ^= supra_mask;  src+=4;
	}

	// ������������ ���������� "�����" (���� �� ����)
	for (a = (n & ~3); a < n; a++)
	{
		 *src ^= mask;	 src += 1;
	}

}

// -[Simple byte-crypt]-------------------------------------------------------
//
//	ARG:
//		src		- ��������� �� ��������� ����
//		mask	- ����� ���������� (����)
//
//	DEPENCE:
//		unalign_crypt
//
//	README:
//		������� ������������� ����������� ��������� ������
//----------------------------------------------------------------------------
void align_crypt(char *src, int n, int mask)
{
	int n_ualign;
	
	// ���������� �������� �� ������� ������� "�������" ����
	// ����� �� ���� ����������� ������
	n_ualign= 32 - ((int) src & 15);

	// ������� ���� �� ��������� ������� ��������� ����� ������
	unalign_crypt(src, n_ualign, mask);

	// ����� ������� ��� ���������
	// �.�. src+n_ualign - �������������� ����������� ���������!
	unalign_crypt(src+n_ualign, n-n_ualign, mask);
	/* �� �������� ���������   ^^^^^^^^^^^^ ��-�� ��������� ������ */
}


main()
{
	char *p;
	
	// TITLE
	PRINT("= = = ������������ ������������� ������������ �������� ������ = = =\n");
	PRINT_TITLE;

	// �������� ������
	p=malloc(BLOCK_SIZE);


	/* -----------------------------------------------------------------------
	 *
	 *					��������� ������������ �����
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		unalign_crypt(p, BLOCK_SIZE, 0x66);
	A_END(0)


	/* -----------------------------------------------------------------------
	 *
	 *					��������� �������������� �����
	 *
	----------------------------------------------------------------------- */
	VVV;
	// "���������" ���������
	p=p + 1;

	A_BEGIN(1)
		unalign_crypt(p , BLOCK_SIZE, 0x66);
	A_END(1)

	/* -----------------------------------------------------------------------
	 *
	 *					�������������� ������������
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
		align_crypt(p, BLOCK_SIZE, 0x66);
	A_END(2)

	// ����� ����������� �� �������
	Lx_OUT("unalign...",Ax_GET(0),Ax_GET(1));
	Lx_OUT("auto align",Ax_GET(0),Ax_GET(2));
	return 0;
}


void _P_S()
{
/*
	....� �������� ��� ��� �����������
			������ �� ����-��������. ��������
*/
}


