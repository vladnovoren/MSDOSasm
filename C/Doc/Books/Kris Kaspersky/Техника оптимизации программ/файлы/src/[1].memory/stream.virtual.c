/* ---------------------------------------------------------------------------
 * @
 *					������� ��� ������������
 *			������������� ������������� ������� ������
 *			==========================================
 *
 * Build 0x001	07.06.2002
--------------------------------------------------------------------------- */

// ������������
#define BLOCK_SIZE			(2*M)	// ����. ����� ������������ ������
#define MAX_N_DST			16		// ����. ���-�� ����������� ������� ������


#define MAIL_ROOL(a)		for(a = 2; a <= MAX_N_DST; a++)
/*                            ^^^^^^^ �������� � ���� ����������� ������� */

#include <DoCPU.h>
main()
{
	int a, b, r, x = 0;
	int *p, *px[MAX_N_DST];

	// TITLE
	PRINT("=== ������������ ������������� ������������� ������� ������  ===\n");
	PRINT_TITLE;
	
	// �����
	printf("N DATA STREAM"); MAIL_ROOL(a) printf("\t%d",a);printf("\n");

	/* -----------------------------------------------------------------------
	 *
	 *		��������� ������� ������������ (������������������) ��������
	 *
	------------------------------------------------------------------------ */
	// �������� ������ ���� �������
	for (a = 0; a < MAX_N_DST; a++) px[a] = (int *) _malloc32(BLOCK_SIZE);

	printf("CLASSIC");
	MAIL_ROOL(r)
	{
		VVV;
		A_BEGIN(0)						/* ������ ������ ������� ���������� */
			for(a = 0; a < BLOCK_SIZE; a += sizeof(int))
					for(b=0; b < r; b++)
						x += *(int *)((int)px[b] + a );
						// ������� ���� ������� ���� �� ������
						// ������, ��� ����� ���������, ������
						// ���� ������� ��������� � ���������
						// ��������� DRAM, ������� ��� ���������
						// ����� ������� �������, DRAM ��������
						// ����� ��������� �����������/����������
						// ������ ��� ����� ������������������
						// ��������! � ������ ������ ����� a � b
						// � �������� �������� ������� �������,
						// �������� ��� ����� ������������������,
						// �� �� �� ������������ ������������
						// ������ _�����������_
		A_END(0)						/* ����� ������ ������� ���������� */

		printf("\t%d",Ax_GET(0));		// ����� ������� ��������� ������
	} printf("\n");	/* end for */



	/* -----------------------------------------------------------------------
	 *
	 *				���������������� ��������� ����������� �������
	 *
	------------------------------------------------------------------------ */
	// �������� ������ ����������� ������
	p = (int*) _malloc32(BLOCK_SIZE*MAX_N_DST);

	printf("OPTIMIZED");
	MAIL_ROOL(r)
	{
		VVV;
		A_BEGIN(1)						/* ������ ������ ������� ���������� */
			for(a = 0; a < BLOCK_SIZE * r; a += (sizeof(int)*r))
			// ��� ����������? ��������,^^ - ��� ���������� ^^^
			// ������ ������������ ����� ���-�� ����������� �������
				for(b = 0; b < r; b++)
					x += *(int *)((int)p + a + b*sizeof(int));
					// ������ ������ ���� ������� ����������� _�����_
					// �������, ����� �� ��������� - ����������
		A_END(1)						/* ����� ������ ������� ���������� */
		printf("\t%d",Ax_GET(1));		// ����� ������� ��������� ������
	} printf("\n");	/* end for */

	return 0;
}

_P_S()
{
/*
	���������  ��������  - ��� ������ �����, �������. ���������� ����� ����� �
	���� ��� ������� ���������, �� ������ ��� ��� ��������.
																	���������
*/
}
