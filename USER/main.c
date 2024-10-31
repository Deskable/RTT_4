#include "STC15F2K60S2.h"
#include "led.h"
#include "delay.h"
#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"

#define KEY_DELAY_TIME 10 // 按键降速时间
#define KEY_LONG_TIME 60  // 长按 按键等待时间

unsigned char ucKey_Dly, ucKey_Old, ucState; // 按键专属，降速，按键判别等变量
unsigned int key_cut;                        // 计算按下后循环次数与KEY_LONG_TIME配套使用
unsigned int uiSeg_Dly, ucSeg_Pos = 0;       // 显示专用，降速,显示位置
unsigned char key_flag, key_type, key_wait;  // 自锁,按键类型\0为短按，1为长按
unsigned char pucSeg_Code[8];                // 本数组内存放的是要显示的字符的段码，为转换之后的结果
unsigned char pucSeg_Buf[9];                 // 本数组内存放的是要显示的字符串，用来进行转换，结果放在pucSeg_Code
unsigned char mode = 0;                      // 模式切换
unsigned char flag = 0;                      // 呼吸灯、上位机切换

char ta[4] = {'T', 'E', 'A', 'P'};
char a[4] = {'A', 'A', 'A', 'A'};
float numtest = 123.456;

void Key_Proc(void);
void Seg_Proc(void);
void Mode_Type(void);

void main()
{
    Cls_Peripheral(); // 关闭LED及周边蜂鸣器等
    Timer1_Init();    // 定时器配置并打开1ms

    while (1)
    {
        Key_Proc(); // 按键判断
        Seg_Proc(); // 数据采集及数据转换，用于数据显示
        Mode_Type();
    }
}

void Time_1(void) interrupt 3 // 中断函数，每隔1ms会进入一次
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
        // 分成几种情况
        // 情况1：100ms两次扫描，按键得到的结果从0（都没按下）到B4按下，产生了下降沿。
        // ucKey_Val = 4(0000 0100)
        // unKey_Down = 0000 0100 & ( 0000 0000 ^ 0000 0100) = 0000 0100 & 0000 0100 = 0000 0100 (4)
        // ucKey_Up = 1111 1011 & 0000 0100 = 0000 0000
        // ucKey_Old = 4

        // 情况2：B4产生了下降沿后，按键一直按着
        // ucKey_Val = 4(0000 0100)
        // unKey_Down = 0000 0100 & ( 0000 0100 ^ 0000 0100) = 0000 0100 & 0000 0000 = 0000 0000 (0)
        // ucKey_Up = 1111 1011 & 0000 0000 = 0000 0000
        // ucKey_Old = 4

        // 情况3：B4按键一直按着随后弹起
        // ucKey_Val = 0(0000 0000)
        // unKey_Down = 0000 0000 & ( 0000 0100 ^ 0000 0000) = 0000 0000 & 0000 0100 = 0000 0000 (0)
        // ucKey_Up = 1111 1111 & 0000 0100 = 0000 0100 (4)
        // ucKey_Old = 0
    }

    ucKey_Val = Key_Read();
    if (ucKey_Val == 10 || ucKey_Old == 10)
        ucKey_Down = ~ucKey_Val & (ucKey_Val ^ ucKey_Old); // 上升沿判定按键（松开生效）
    else
        ucKey_Down = ucKey_Val & (ucKey_Val ^ ucKey_Old); // 下降沿判定按键（按下生效）

    ucKey_Old = ucKey_Val;

    // 判断长按
    if (!ucKey_Val) // 按键未按下
    {
        key_cut = 0;  // 计数
        key_flag = 0; // 自锁
    }
    else if (!key_flag)
    {
        key_cut++;    // 计算按下后循环次数
        key_type = 0; // 按键类型0为短按，1为长按
        if (key_cut > KEY_LONG_TIME)
        {
            key_cut = 0; // 循环次数归零
            key_flag = 1;
            // 自锁锁定，!key_flag为假 跳过 如果为上升
            // 等待按键松开恢复
            // 避免重复触发
            key_type = 1;           // 按键类型0为短按，1为长按
            ucKey_Down = ucKey_Old; // 长按按键，但是时间达到直接输出
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
                /* 长按*/
                sprintf(pucSeg_Buf, "%-8c", '7');
            }
            else
            {
                /* 短按 */
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