#include "STC15F2K60S2.h"
#include "led.h"
#include "delay.h"
#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"

#define KEY_DELAY_TIME 10 // ��������ʱ��
#define KEY_LONG_TIME 60  // ���� �����ȴ�ʱ��

unsigned char ucKey_Dly, ucKey_Old, ucState; // ����ר�������٣������б�ȱ���
unsigned int key_cut;                        // ���㰴�º�ѭ��������KEY_LONG_TIME����ʹ��
unsigned int uiSeg_Dly, ucSeg_Pos = 0;       // ��ʾר�ã�����,��ʾλ��
unsigned char key_flag, key_type, key_wait;  // ����,��������\0Ϊ�̰���1Ϊ����
unsigned char pucSeg_Code[8];                // �������ڴ�ŵ���Ҫ��ʾ���ַ��Ķ��룬Ϊת��֮��Ľ��
unsigned char pucSeg_Buf[9];                 // �������ڴ�ŵ���Ҫ��ʾ���ַ�������������ת�����������pucSeg_Code
unsigned char mode = 0;                      // ģʽ�л�
unsigned char flag = 0;                      // �����ơ���λ���л�

char ta[4] = {'T', 'E', 'A', 'P'};
char a[4] = {'A', 'A', 'A', 'A'};
float numtest = 123.456;

void Key_Proc(void);
void Seg_Proc(void);
void Mode_Type(void);

void main()
{
    Cls_Peripheral(); // �ر�LED���ܱ߷�������
    Timer1_Init();    // ��ʱ�����ò���1ms

    while (1)
    {
        Key_Proc(); // �����ж�
        Seg_Proc(); // ���ݲɼ�������ת��������������ʾ
        Mode_Type();
    }
}

void Time_1(void) interrupt 3 // �жϺ�����ÿ��1ms�����һ��
{
    if (++uiSeg_Dly == 500)
        uiSeg_Dly = 0;

    if (++ucKey_Dly == KEY_DELAY_TIME)
        ucKey_Dly = 0;

    if (uiSeg_Dly)
    {
        uiSeg_Dly = 1;
        Seg_Disp(pucSeg_Code, ucSeg_Pos);
        if (++ucSeg_Pos == 8)
            ucSeg_Pos = 0;
    }
}

void Key_Proc(void)
{
    unsigned char ucKey_Val, ucKey_Down;

    if (ucKey_Dly)
        return;
    ucKey_Dly = 1;

    {
        // �ֳɼ������
        // ���1��100ms����ɨ�裬�����õ��Ľ����0����û���£���B4���£��������½��ء�
        // ucKey_Val = 4(0000 0100)
        // unKey_Down = 0000 0100 & ( 0000 0000 ^ 0000 0100) = 0000 0100 & 0000 0100 = 0000 0100 (4)
        // ucKey_Up = 1111 1011 & 0000 0100 = 0000 0000
        // ucKey_Old = 4

        // ���2��B4�������½��غ󣬰���һֱ����
        // ucKey_Val = 4(0000 0100)
        // unKey_Down = 0000 0100 & ( 0000 0100 ^ 0000 0100) = 0000 0100 & 0000 0000 = 0000 0000 (0)
        // ucKey_Up = 1111 1011 & 0000 0000 = 0000 0000
        // ucKey_Old = 4

        // ���3��B4����һֱ���������
        // ucKey_Val = 0(0000 0000)
        // unKey_Down = 0000 0000 & ( 0000 0100 ^ 0000 0000) = 0000 0000 & 0000 0100 = 0000 0000 (0)
        // ucKey_Up = 1111 1111 & 0000 0100 = 0000 0100 (4)
        // ucKey_Old = 0
    }

    ucKey_Val = Key_Read();
    if (ucKey_Val == 10 || ucKey_Old == 10)
        ucKey_Down = ~ucKey_Val & (ucKey_Val ^ ucKey_Old); // �������ж��������ɿ���Ч��
    else
        ucKey_Down = ucKey_Val & (ucKey_Val ^ ucKey_Old); // �½����ж�������������Ч��

    ucKey_Old = ucKey_Val;

    // �жϳ���
    if (!ucKey_Val) // ����δ����
    {
        key_cut = 0;  // ����
        key_flag = 0; // ����
    }
    else if (!key_flag)
    {
        key_cut++;    // ���㰴�º�ѭ������
        key_type = 0; // ��������0Ϊ�̰���1Ϊ����
        if (key_cut > KEY_LONG_TIME)
        {
            key_cut = 0; // ѭ����������
            key_flag = 1;
            // ����������!key_flagΪ�� ���� ���Ϊ����
            // �ȴ������ɿ��ָ�
            // �����ظ�����
            key_type = 1;           // ��������0Ϊ�̰���1Ϊ����
            ucKey_Down = ucKey_Old; // ��������������ʱ��ﵽֱ�����
        }
    }

    switch (ucKey_Down)
    {
    case 4:
        ucState = 4;
        break;
    case 5:
        ucState = 5;
        break;
    case 6:
        ucState = 6;
        break;
    case 7:
        ucState = 7;
        flag = !flag;
        break;
    // case 8:
    //     ucState = 8;
    //     break;
    // case 9:
    //     ucState = 9;
    //     break;
    case 10:
        ucState = 10;
        break;
    case 11:
        ucState = 11;
        flag = !flag;
        break;
    case 16:
        mode++;
        if (mode > 4)
            mode = 1;
        break;

    default:
        break;
    }
}

void Seg_Proc(void)
{
}

void Mode_Type(void)
{
    if (mode == 1)
    {
        if (ucState == 7)
        {
            if (flag)
            {
                Led_Disp(0x3C);
            }
            else
            {
                Led_Disp(0x00);
                /* code */
            }
        }
        sprintf(pucSeg_Buf, "%-8c", ' ');
    }
    else if (mode == 2)
    {
        switch (ucState)
        {
        case 6:
            sprintf(pucSeg_Buf, "%-8.4s", ta);
            break;
        case 5:
            //
            sprintf(pucSeg_Buf, "%9.3f", numtest);
            break;
        }
    }
    else if (mode == 3)
    {
        if (ucState == 4)
        {
            sprintf(pucSeg_Buf, "%s", a);
        }
        else if (ucState == 10)
        {
            if (key_type)
            {
                /* ����*/
                sprintf(pucSeg_Buf, "%-8c", '7');
            }
            else
            {
                /* �̰� */
                sprintf(pucSeg_Buf, "%8c", '6');
            }
        }
    }
    else if (mode == 4)
    {
        if (ucState == 11)
        {
            if (flag)
            {
                sprintf(pucSeg_Buf, "%-8c", 'E');
            }
            else
            {
                sprintf(pucSeg_Buf, "%8c", 'A');
            }
        }
    }
    Seg_Tran(pucSeg_Buf, pucSeg_Code);
}