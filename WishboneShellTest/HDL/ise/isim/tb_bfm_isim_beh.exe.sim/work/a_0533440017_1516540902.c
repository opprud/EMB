/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0xb869381d */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "C:/Workspace/EMB/WB_shell/HDL/src/Intercon.vhd";
extern char *IEEE_P_2592010699;
extern char *WORK_P_1716878159;

unsigned char ieee_p_2592010699_sub_2545490612_2592010699(char *, unsigned char , unsigned char );


static void work_a_0533440017_1516540902_p_0(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;

LAB0:    xsi_set_current_line(67, ng0);

LAB3:    t1 = (t0 + 1512U);
    t2 = *((char **)t1);
    t1 = (t0 + 5928);
    t3 = (t1 + 32U);
    t4 = *((char **)t3);
    t5 = (t4 + 40U);
    t6 = *((char **)t5);
    memcpy(t6, t2, 16U);
    xsi_driver_first_trans_fast_port(t1);

LAB2:    t7 = (t0 + 5780);
    *((int *)t7) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_0533440017_1516540902_p_1(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    int t6;
    int t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned char t11;
    unsigned char t12;
    char *t13;
    char *t14;
    char *t15;

LAB0:    xsi_set_current_line(72, ng0);
    t1 = (t0 + 5964);
    t2 = (t1 + 32U);
    t3 = *((char **)t2);
    t4 = (t3 + 40U);
    t5 = *((char **)t4);
    *((unsigned char *)t5) = (unsigned char)2;
    xsi_driver_first_trans_fast_port(t1);
    xsi_set_current_line(73, ng0);
    t1 = (t0 + 1696U);
    t2 = *((char **)t1);
    t1 = (t0 + 2616U);
    t3 = *((char **)t1);
    t6 = *((int *)t3);
    t7 = (t6 - 2);
    t8 = (t7 * -1);
    xsi_vhdl_check_range_of_index(2, 0, -1, t6);
    t9 = (1U * t8);
    t10 = (0 + t9);
    t1 = (t2 + t10);
    t11 = *((unsigned char *)t1);
    t12 = (t11 == (unsigned char)3);
    if (t12 != 0)
        goto LAB2;

LAB4:    xsi_set_current_line(76, ng0);
    t1 = (t0 + 5964);
    t2 = (t1 + 32U);
    t3 = *((char **)t2);
    t4 = (t3 + 40U);
    t5 = *((char **)t4);
    *((unsigned char *)t5) = (unsigned char)2;
    xsi_driver_first_trans_fast_port(t1);

LAB3:    t1 = (t0 + 5788);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(74, ng0);
    t4 = (t0 + 5964);
    t5 = (t4 + 32U);
    t13 = *((char **)t5);
    t14 = (t13 + 40U);
    t15 = *((char **)t14);
    *((unsigned char *)t15) = (unsigned char)3;
    xsi_driver_first_trans_fast_port(t4);
    goto LAB3;

}

static void work_a_0533440017_1516540902_p_2(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    int t6;
    int t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned char t11;
    unsigned char t12;
    unsigned char t13;
    unsigned char t14;
    char *t15;
    char *t16;
    char *t17;
    char *t18;

LAB0:    xsi_set_current_line(82, ng0);
    t1 = (t0 + 6000);
    t2 = (t1 + 32U);
    t3 = *((char **)t2);
    t4 = (t3 + 40U);
    t5 = *((char **)t4);
    *((unsigned char *)t5) = (unsigned char)2;
    xsi_driver_first_trans_fast_port(t1);
    xsi_set_current_line(83, ng0);
    t1 = (t0 + 2064U);
    t2 = *((char **)t1);
    t1 = (t0 + 2616U);
    t3 = *((char **)t1);
    t6 = *((int *)t3);
    t7 = (t6 - 2);
    t8 = (t7 * -1);
    xsi_vhdl_check_range_of_index(2, 0, -1, t6);
    t9 = (1U * t8);
    t10 = (0 + t9);
    t1 = (t2 + t10);
    t11 = *((unsigned char *)t1);
    t4 = (t0 + 2892U);
    t5 = *((char **)t4);
    t12 = *((unsigned char *)t5);
    t13 = ieee_p_2592010699_sub_2545490612_2592010699(IEEE_P_2592010699, t11, t12);
    t14 = (t13 == (unsigned char)3);
    if (t14 != 0)
        goto LAB2;

LAB4:    xsi_set_current_line(86, ng0);
    t1 = (t0 + 6000);
    t2 = (t1 + 32U);
    t3 = *((char **)t2);
    t4 = (t3 + 40U);
    t5 = *((char **)t4);
    *((unsigned char *)t5) = (unsigned char)2;
    xsi_driver_first_trans_fast_port(t1);

LAB3:    t1 = (t0 + 5796);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(84, ng0);
    t4 = (t0 + 6000);
    t15 = (t4 + 32U);
    t16 = *((char **)t15);
    t17 = (t16 + 40U);
    t18 = *((char **)t17);
    *((unsigned char *)t18) = (unsigned char)3;
    xsi_driver_first_trans_fast_port(t4);
    goto LAB3;

}

static void work_a_0533440017_1516540902_p_3(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    int t6;
    int t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned char t11;
    char *t12;
    char *t13;
    char *t14;

LAB0:    xsi_set_current_line(92, ng0);
    t1 = (t0 + 6036);
    t2 = (t1 + 32U);
    t3 = *((char **)t2);
    t4 = (t3 + 40U);
    t5 = *((char **)t4);
    *((unsigned char *)t5) = (unsigned char)2;
    xsi_driver_first_trans_fast_port(t1);
    xsi_set_current_line(93, ng0);
    t1 = (t0 + 2156U);
    t2 = *((char **)t1);
    t1 = (t0 + 2616U);
    t3 = *((char **)t1);
    t6 = *((int *)t3);
    t7 = (t6 - 2);
    t8 = (t7 * -1);
    xsi_vhdl_check_range_of_index(2, 0, -1, t6);
    t9 = (1U * t8);
    t10 = (0 + t9);
    t1 = (t2 + t10);
    t11 = *((unsigned char *)t1);
    t4 = (t0 + 6036);
    t5 = (t4 + 32U);
    t12 = *((char **)t5);
    t13 = (t12 + 40U);
    t14 = *((char **)t13);
    *((unsigned char *)t14) = t11;
    xsi_driver_first_trans_fast_port(t4);
    t1 = (t0 + 5804);
    *((int *)t1) = 1;

LAB1:    return;
}

static void work_a_0533440017_1516540902_p_4(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    int t8;
    int t9;
    unsigned int t10;
    unsigned int t11;
    unsigned int t12;
    char *t13;

LAB0:    xsi_set_current_line(98, ng0);
    t1 = xsi_get_transient_memory(16U);
    memset(t1, 0, 16U);
    t2 = t1;
    memset(t2, (unsigned char)2, 16U);
    t3 = (t0 + 6072);
    t4 = (t3 + 32U);
    t5 = *((char **)t4);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    memcpy(t7, t1, 16U);
    xsi_driver_first_trans_fast_port(t3);
    xsi_set_current_line(99, ng0);
    t1 = (t0 + 2432U);
    t2 = *((char **)t1);
    t1 = (t0 + 2616U);
    t3 = *((char **)t1);
    t8 = *((int *)t3);
    t9 = (t8 - 2);
    t10 = (t9 * -1);
    xsi_vhdl_check_range_of_index(2, 0, -1, t8);
    t11 = (16U * t10);
    t12 = (0 + t11);
    t1 = (t2 + t12);
    t4 = (t0 + 6072);
    t5 = (t4 + 32U);
    t6 = *((char **)t5);
    t7 = (t6 + 40U);
    t13 = *((char **)t7);
    memcpy(t13, t1, 16U);
    xsi_driver_first_trans_fast_port(t4);
    t1 = (t0 + 5812);
    *((int *)t1) = 1;

LAB1:    return;
}

static void work_a_0533440017_1516540902_p_5(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    unsigned char t8;
    unsigned char t9;
    int t10;
    int t11;
    unsigned int t12;
    unsigned int t13;
    unsigned int t14;

LAB0:    xsi_set_current_line(106, ng0);
    t1 = xsi_get_transient_memory(3U);
    memset(t1, 0, 3U);
    t2 = t1;
    memset(t2, (unsigned char)2, 3U);
    t3 = (t0 + 6108);
    t4 = (t3 + 32U);
    t5 = *((char **)t4);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    memcpy(t7, t1, 3U);
    xsi_driver_first_trans_fast_port(t3);
    xsi_set_current_line(107, ng0);
    t1 = (t0 + 1328U);
    t2 = *((char **)t1);
    t8 = *((unsigned char *)t2);
    t9 = (t8 == (unsigned char)3);
    if (t9 != 0)
        goto LAB2;

LAB4:    xsi_set_current_line(110, ng0);
    t1 = (t0 + 2616U);
    t2 = *((char **)t1);
    t10 = *((int *)t2);
    t11 = (t10 - 2);
    t12 = (t11 * -1);
    t13 = (1 * t12);
    t14 = (0U + t13);
    t1 = (t0 + 6108);
    t3 = (t1 + 32U);
    t4 = *((char **)t3);
    t5 = (t4 + 40U);
    t6 = *((char **)t5);
    *((unsigned char *)t6) = (unsigned char)2;
    xsi_driver_first_trans_delta(t1, t14, 1, 0LL);

LAB3:    t1 = (t0 + 5820);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(108, ng0);
    t1 = (t0 + 2616U);
    t3 = *((char **)t1);
    t10 = *((int *)t3);
    t11 = (t10 - 2);
    t12 = (t11 * -1);
    t13 = (1 * t12);
    t14 = (0U + t13);
    t1 = (t0 + 6108);
    t4 = (t1 + 32U);
    t5 = *((char **)t4);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = (unsigned char)3;
    xsi_driver_first_trans_delta(t1, t14, 1, 0LL);
    goto LAB3;

}

static void work_a_0533440017_1516540902_p_6(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    int t4;
    int t5;
    unsigned int t6;
    unsigned int t7;
    unsigned int t8;
    char *t9;
    char *t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;

LAB0:    xsi_set_current_line(114, ng0);

LAB3:    t1 = (t0 + 868U);
    t2 = *((char **)t1);
    t1 = ((WORK_P_1716878159) + 1012U);
    t3 = *((char **)t1);
    t4 = *((int *)t3);
    t5 = (t4 - 1);
    t6 = (9 - t5);
    t7 = (t6 * 1U);
    t8 = (0 + t7);
    t1 = (t2 + t8);
    t9 = (t0 + 6144);
    t10 = (t9 + 32U);
    t11 = *((char **)t10);
    t12 = (t11 + 40U);
    t13 = *((char **)t12);
    memcpy(t13, t1, 8U);
    xsi_driver_first_trans_fast_port(t9);

LAB2:    t14 = (t0 + 5828);
    *((int *)t14) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_0533440017_1516540902_p_7(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;

LAB0:    xsi_set_current_line(116, ng0);

LAB3:    t1 = (t0 + 960U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 6180);
    t4 = (t1 + 32U);
    t5 = *((char **)t4);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast_port(t1);

LAB2:    t8 = (t0 + 5836);
    *((int *)t8) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_0533440017_1516540902_p_8(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;

LAB0:    xsi_set_current_line(117, ng0);

LAB3:    t1 = (t0 + 1052U);
    t2 = *((char **)t1);
    t1 = (t0 + 6216);
    t3 = (t1 + 32U);
    t4 = *((char **)t3);
    t5 = (t4 + 40U);
    t6 = *((char **)t5);
    memcpy(t6, t2, 3U);
    xsi_driver_first_trans_fast_port(t1);

LAB2:    t7 = (t0 + 5844);
    *((int *)t7) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_0533440017_1516540902_p_9(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;

LAB0:    xsi_set_current_line(118, ng0);

LAB3:    t1 = (t0 + 1420U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 6252);
    t4 = (t1 + 32U);
    t5 = *((char **)t4);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast_port(t1);

LAB2:    t8 = (t0 + 5852);
    *((int *)t8) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_0533440017_1516540902_p_10(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;

LAB0:    xsi_set_current_line(119, ng0);

LAB3:    t1 = (t0 + 868U);
    t2 = *((char **)t1);
    t1 = (t0 + 6288);
    t3 = (t1 + 32U);
    t4 = *((char **)t3);
    t5 = (t4 + 40U);
    t6 = *((char **)t5);
    memcpy(t6, t2, 10U);
    xsi_driver_first_trans_fast(t1);

LAB2:    t7 = (t0 + 5860);
    *((int *)t7) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_0533440017_1516540902_p_11(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    int t4;
    int t5;
    unsigned int t6;
    unsigned int t7;
    unsigned int t8;
    char *t9;
    char *t10;
    int t11;
    char *t12;
    int t13;
    char *t14;
    char *t15;
    char *t16;
    char *t17;

LAB0:    xsi_set_current_line(124, ng0);
    t1 = (t0 + 2984U);
    t2 = *((char **)t1);
    t1 = ((WORK_P_1716878159) + 944U);
    t3 = *((char **)t1);
    t4 = *((int *)t3);
    t5 = (t4 - 1);
    t6 = (9 - t5);
    t7 = (t6 * 1U);
    t8 = (0 + t7);
    t1 = (t2 + t8);
    t9 = ((WORK_P_1716878159) + 1352U);
    t10 = *((char **)t9);
    t11 = xsi_mem_cmp(t10, t1, 2U);
    if (t11 == 1)
        goto LAB3;

LAB6:    t9 = ((WORK_P_1716878159) + 1420U);
    t12 = *((char **)t9);
    t13 = xsi_mem_cmp(t12, t1, 2U);
    if (t13 == 1)
        goto LAB4;

LAB7:
LAB5:    xsi_set_current_line(127, ng0);
    t1 = (t0 + 6324);
    t2 = (t1 + 32U);
    t3 = *((char **)t2);
    t9 = (t3 + 40U);
    t10 = *((char **)t9);
    *((int *)t10) = 0;
    xsi_driver_first_trans_fast(t1);

LAB2:    t1 = (t0 + 5868);
    *((int *)t1) = 1;

LAB1:    return;
LAB3:    xsi_set_current_line(125, ng0);
    t9 = (t0 + 6324);
    t14 = (t9 + 32U);
    t15 = *((char **)t14);
    t16 = (t15 + 40U);
    t17 = *((char **)t16);
    *((int *)t17) = 1;
    xsi_driver_first_trans_fast(t9);
    goto LAB2;

LAB4:    xsi_set_current_line(126, ng0);
    t1 = (t0 + 6324);
    t2 = (t1 + 32U);
    t3 = *((char **)t2);
    t9 = (t3 + 40U);
    t10 = *((char **)t9);
    *((int *)t10) = 2;
    xsi_driver_first_trans_fast(t1);
    goto LAB2;

LAB8:;
}

static void work_a_0533440017_1516540902_p_12(char *t0)
{
    unsigned char t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    unsigned char t6;
    unsigned char t7;
    char *t8;
    unsigned char t9;
    unsigned char t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;
    int t15;
    int t16;
    unsigned int t17;
    unsigned int t18;
    unsigned int t19;
    int t20;

LAB0:    xsi_set_current_line(134, ng0);
    t2 = (t0 + 568U);
    t3 = xsi_signal_has_event(t2);
    if (t3 == 1)
        goto LAB5;

LAB6:    t1 = (unsigned char)0;

LAB7:    if (t1 != 0)
        goto LAB2;

LAB4:
LAB3:    t2 = (t0 + 5876);
    *((int *)t2) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(135, ng0);
    t4 = (t0 + 684U);
    t8 = *((char **)t4);
    t9 = *((unsigned char *)t8);
    t10 = (t9 == (unsigned char)3);
    if (t10 != 0)
        goto LAB8;

LAB10:    xsi_set_current_line(139, ng0);
    t2 = (t0 + 1328U);
    t4 = *((char **)t2);
    t3 = *((unsigned char *)t4);
    t6 = (t3 == (unsigned char)3);
    if (t6 == 1)
        goto LAB14;

LAB15:    t1 = (unsigned char)0;

LAB16:    if (t1 != 0)
        goto LAB11;

LAB13:    t2 = (t0 + 1328U);
    t4 = *((char **)t2);
    t1 = *((unsigned char *)t4);
    t3 = (t1 == (unsigned char)2);
    if (t3 != 0)
        goto LAB17;

LAB18:
LAB12:    xsi_set_current_line(145, ng0);
    t2 = (t0 + 1696U);
    t4 = *((char **)t2);
    t2 = (t0 + 2616U);
    t5 = *((char **)t2);
    t15 = *((int *)t5);
    t16 = (t15 - 2);
    t17 = (t16 * -1);
    xsi_vhdl_check_range_of_index(2, 0, -1, t15);
    t18 = (1U * t17);
    t19 = (0 + t18);
    t2 = (t4 + t19);
    t1 = *((unsigned char *)t2);
    t3 = (t1 == (unsigned char)3);
    if (t3 != 0)
        goto LAB19;

LAB21:
LAB20:    xsi_set_current_line(148, ng0);
    t2 = (t0 + 2800U);
    t4 = *((char **)t2);
    t15 = *((int *)t4);
    t2 = (t0 + 3156U);
    t5 = *((char **)t2);
    t16 = *((int *)t5);
    t20 = (t16 - 1);
    t1 = (t15 == t20);
    if (t1 != 0)
        goto LAB22;

LAB24:
LAB23:
LAB9:    goto LAB3;

LAB5:    t4 = (t0 + 592U);
    t5 = *((char **)t4);
    t6 = *((unsigned char *)t5);
    t7 = (t6 == (unsigned char)3);
    t1 = t7;
    goto LAB7;

LAB8:    xsi_set_current_line(136, ng0);
    t4 = (t0 + 6360);
    t11 = (t4 + 32U);
    t12 = *((char **)t11);
    t13 = (t12 + 40U);
    t14 = *((char **)t13);
    *((unsigned char *)t14) = (unsigned char)2;
    xsi_driver_first_trans_fast(t4);
    xsi_set_current_line(137, ng0);
    t2 = (t0 + 6396);
    t4 = (t2 + 32U);
    t5 = *((char **)t4);
    t8 = (t5 + 40U);
    t11 = *((char **)t8);
    *((unsigned char *)t11) = (unsigned char)0;
    xsi_driver_first_trans_fast(t2);
    goto LAB9;

LAB11:    xsi_set_current_line(140, ng0);
    t2 = (t0 + 6396);
    t8 = (t2 + 32U);
    t11 = *((char **)t8);
    t12 = (t11 + 40U);
    t13 = *((char **)t12);
    *((unsigned char *)t13) = (unsigned char)1;
    xsi_driver_first_trans_fast(t2);
    goto LAB12;

LAB14:    t2 = (t0 + 2892U);
    t5 = *((char **)t2);
    t7 = *((unsigned char *)t5);
    t9 = (t7 == (unsigned char)2);
    t1 = t9;
    goto LAB16;

LAB17:    xsi_set_current_line(142, ng0);
    t2 = (t0 + 6360);
    t5 = (t2 + 32U);
    t8 = *((char **)t5);
    t11 = (t8 + 40U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = (unsigned char)2;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(143, ng0);
    t2 = (t0 + 6396);
    t4 = (t2 + 32U);
    t5 = *((char **)t4);
    t8 = (t5 + 40U);
    t11 = *((char **)t8);
    *((unsigned char *)t11) = (unsigned char)0;
    xsi_driver_first_trans_fast(t2);
    goto LAB12;

LAB19:    xsi_set_current_line(146, ng0);
    t8 = (t0 + 6396);
    t11 = (t8 + 32U);
    t12 = *((char **)t11);
    t13 = (t12 + 40U);
    t14 = *((char **)t13);
    *((unsigned char *)t14) = (unsigned char)0;
    xsi_driver_first_trans_fast(t8);
    goto LAB20;

LAB22:    xsi_set_current_line(149, ng0);
    t2 = (t0 + 6360);
    t8 = (t2 + 32U);
    t11 = *((char **)t8);
    t12 = (t11 + 40U);
    t13 = *((char **)t12);
    *((unsigned char *)t13) = (unsigned char)3;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(150, ng0);
    t2 = (t0 + 6396);
    t4 = (t2 + 32U);
    t5 = *((char **)t4);
    t8 = (t5 + 40U);
    t11 = *((char **)t8);
    *((unsigned char *)t11) = (unsigned char)0;
    xsi_driver_first_trans_fast(t2);
    goto LAB23;

}

static void work_a_0533440017_1516540902_p_13(char *t0)
{
    unsigned char t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    unsigned char t6;
    unsigned char t7;
    char *t8;
    unsigned char t9;
    unsigned char t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;
    int t15;
    int t16;

LAB0:    xsi_set_current_line(158, ng0);
    t2 = (t0 + 568U);
    t3 = xsi_signal_has_event(t2);
    if (t3 == 1)
        goto LAB5;

LAB6:    t1 = (unsigned char)0;

LAB7:    if (t1 != 0)
        goto LAB2;

LAB4:
LAB3:    t2 = (t0 + 5884);
    *((int *)t2) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(159, ng0);
    t4 = (t0 + 684U);
    t8 = *((char **)t4);
    t9 = *((unsigned char *)t8);
    t10 = (t9 == (unsigned char)3);
    if (t10 != 0)
        goto LAB8;

LAB10:    xsi_set_current_line(162, ng0);
    t2 = (t0 + 2708U);
    t4 = *((char **)t2);
    t1 = *((unsigned char *)t4);
    if (t1 != 0)
        goto LAB11;

LAB13:    xsi_set_current_line(165, ng0);
    t2 = (t0 + 6432);
    t4 = (t2 + 32U);
    t5 = *((char **)t4);
    t8 = (t5 + 40U);
    t11 = *((char **)t8);
    *((int *)t11) = 0;
    xsi_driver_first_trans_fast(t2);

LAB12:
LAB9:    goto LAB3;

LAB5:    t4 = (t0 + 592U);
    t5 = *((char **)t4);
    t6 = *((unsigned char *)t5);
    t7 = (t6 == (unsigned char)3);
    t1 = t7;
    goto LAB7;

LAB8:    xsi_set_current_line(160, ng0);
    t4 = (t0 + 6432);
    t11 = (t4 + 32U);
    t12 = *((char **)t11);
    t13 = (t12 + 40U);
    t14 = *((char **)t13);
    *((int *)t14) = 0;
    xsi_driver_first_trans_fast(t4);
    goto LAB9;

LAB11:    xsi_set_current_line(163, ng0);
    t2 = (t0 + 2800U);
    t5 = *((char **)t2);
    t15 = *((int *)t5);
    t16 = (t15 + 1);
    t2 = (t0 + 6432);
    t8 = (t2 + 32U);
    t11 = *((char **)t8);
    t12 = (t11 + 40U);
    t13 = *((char **)t12);
    *((int *)t13) = t16;
    xsi_driver_first_trans_fast(t2);
    goto LAB12;

}


extern void work_a_0533440017_1516540902_init()
{
	static char *pe[] = {(void *)work_a_0533440017_1516540902_p_0,(void *)work_a_0533440017_1516540902_p_1,(void *)work_a_0533440017_1516540902_p_2,(void *)work_a_0533440017_1516540902_p_3,(void *)work_a_0533440017_1516540902_p_4,(void *)work_a_0533440017_1516540902_p_5,(void *)work_a_0533440017_1516540902_p_6,(void *)work_a_0533440017_1516540902_p_7,(void *)work_a_0533440017_1516540902_p_8,(void *)work_a_0533440017_1516540902_p_9,(void *)work_a_0533440017_1516540902_p_10,(void *)work_a_0533440017_1516540902_p_11,(void *)work_a_0533440017_1516540902_p_12,(void *)work_a_0533440017_1516540902_p_13};
	xsi_register_didat("work_a_0533440017_1516540902", "isim/tb_bfm_isim_beh.exe.sim/work/a_0533440017_1516540902.didat");
	xsi_register_executes(pe);
}
