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
extern char *IEEE_P_2592010699;
extern char *STD_STANDARD;



void work_p_0201983097_sub_4253348202_201983097(char *t0, char *t1, char *t2, unsigned int t3, unsigned int t4, char *t5, unsigned int t6, unsigned int t7, char *t8, char *t9, char *t10, char *t11)
{
    char t13[24];
    char t14[16];
    char t19[16];
    char *t15;
    char *t16;
    int t17;
    unsigned int t18;
    char *t20;
    int t21;
    unsigned char t22;
    char *t23;
    char *t24;
    unsigned char t25;
    char *t26;
    char *t27;
    unsigned char t28;
    unsigned int t29;
    unsigned int t30;
    char *t31;
    char *t32;
    char *t33;
    char *t34;
    int64 t35;
    char *t36;
    char *t37;
    char *t38;
    char *t39;
    char *t40;
    char *t41;
    char *t42;
    int64 t43;
    int64 t44;
    char *t45;

LAB0:    t15 = (t14 + 0U);
    t16 = (t15 + 0U);
    *((int *)t16) = 15;
    t16 = (t15 + 4U);
    *((int *)t16) = 0;
    t16 = (t15 + 8U);
    *((int *)t16) = -1;
    t17 = (0 - 15);
    t18 = (t17 * -1);
    t18 = (t18 + 1);
    t16 = (t15 + 12U);
    *((unsigned int *)t16) = t18;
    t16 = (t19 + 0U);
    t20 = (t16 + 0U);
    *((int *)t20) = 15;
    t20 = (t16 + 4U);
    *((int *)t20) = 0;
    t20 = (t16 + 8U);
    *((int *)t20) = -1;
    t21 = (0 - 15);
    t18 = (t21 * -1);
    t18 = (t18 + 1);
    t20 = (t16 + 12U);
    *((unsigned int *)t20) = t18;
    t20 = (t13 + 4U);
    t22 = (t9 != 0);
    if (t22 == 1)
        goto LAB3;

LAB2:    t23 = (t13 + 8U);
    *((char **)t23) = t14;
    t24 = (t13 + 12U);
    t25 = (t10 != 0);
    if (t25 == 1)
        goto LAB5;

LAB4:    t26 = (t13 + 16U);
    *((char **)t26) = t19;
    t27 = (t13 + 20U);
    t28 = (t11 != 0);
    if (t28 == 1)
        goto LAB7;

LAB6:    t18 = (0 + 0U);
    t29 = (0 + 0U);
    t30 = (t29 + t6);
    t31 = (t8 + 32U);
    t32 = *((char **)t31);
    t33 = (t32 + 40U);
    t34 = *((char **)t33);
    *((unsigned char *)t34) = (unsigned char)2;
    xsi_driver_first_trans_delta(t8, t30, 1, 0LL);
    t15 = (t0 + 1216U);
    t16 = *((char **)t15);
    t35 = *((int64 *)t16);
    xsi_process_wait(t1, t35);

LAB11:    t15 = (t1 + 48U);
    t31 = *((char **)t15);
    t32 = (t31 + 1440U);
    *((unsigned int *)t32) = 1U;
    t33 = (t1 + 48U);
    t34 = *((char **)t33);
    t36 = (t34 + 0U);
    getcontext(t36);
    t37 = (t1 + 48U);
    t38 = *((char **)t37);
    t39 = (t38 + 1440U);
    t18 = *((unsigned int *)t39);
    if (t18 == 1)
        goto LAB12;

LAB13:    t40 = (t1 + 48U);
    t41 = *((char **)t40);
    t42 = (t41 + 1440U);
    *((unsigned int *)t42) = 3U;

LAB9:
LAB10:
LAB8:    t18 = (0 + 3U);
    t29 = (0 + 3U);
    t30 = (t29 + t6);
    t15 = (t8 + 32U);
    t16 = *((char **)t15);
    t31 = (t16 + 40U);
    t32 = *((char **)t31);
    memcpy(t32, t9, 16U);
    xsi_driver_first_trans_delta(t8, t30, 16U, 0LL);
    t15 = (t0 + 1216U);
    t16 = *((char **)t15);
    t35 = *((int64 *)t16);
    t15 = (t0 + 1352U);
    t31 = *((char **)t15);
    t43 = *((int64 *)t31);
    t44 = (t35 - t43);
    xsi_process_wait(t1, t44);

LAB17:    t15 = (t1 + 48U);
    t32 = *((char **)t15);
    t33 = (t32 + 1440U);
    *((unsigned int *)t33) = 1U;
    t34 = (t1 + 48U);
    t36 = *((char **)t34);
    t37 = (t36 + 0U);
    getcontext(t37);
    t38 = (t1 + 48U);
    t39 = *((char **)t38);
    t40 = (t39 + 1440U);
    t18 = *((unsigned int *)t40);
    if (t18 == 1)
        goto LAB18;

LAB19:    t41 = (t1 + 48U);
    t42 = *((char **)t41);
    t45 = (t42 + 1440U);
    *((unsigned int *)t45) = 3U;

LAB15:
LAB16:
LAB14:    t18 = (0 + 19U);
    t29 = (0 + 19U);
    t30 = (t29 + t6);
    t15 = (t8 + 32U);
    t16 = *((char **)t15);
    t31 = (t16 + 40U);
    t32 = *((char **)t31);
    memcpy(t32, t10, 16U);
    xsi_driver_first_trans_delta(t8, t30, 16U, 0LL);
    t15 = (t0 + 1284U);
    t16 = *((char **)t15);
    t35 = *((int64 *)t16);
    t15 = (t0 + 1216U);
    t31 = *((char **)t15);
    t43 = *((int64 *)t31);
    t44 = (t35 - t43);
    xsi_process_wait(t1, t44);

LAB23:    t15 = (t1 + 48U);
    t32 = *((char **)t15);
    t33 = (t32 + 1440U);
    *((unsigned int *)t33) = 1U;
    t34 = (t1 + 48U);
    t36 = *((char **)t34);
    t37 = (t36 + 0U);
    getcontext(t37);
    t38 = (t1 + 48U);
    t39 = *((char **)t38);
    t40 = (t39 + 1440U);
    t18 = *((unsigned int *)t40);
    if (t18 == 1)
        goto LAB24;

LAB25:    t41 = (t1 + 48U);
    t42 = *((char **)t41);
    t45 = (t42 + 1440U);
    *((unsigned int *)t45) = 3U;

LAB21:
LAB22:
LAB20:    t18 = (0 + 2U);
    t29 = (0 + 2U);
    t30 = (t29 + t6);
    t15 = (t8 + 32U);
    t16 = *((char **)t15);
    t31 = (t16 + 40U);
    t32 = *((char **)t31);
    *((unsigned char *)t32) = (unsigned char)2;
    xsi_driver_first_trans_delta(t8, t30, 1, 0LL);
    t15 = (t0 + 1420U);
    t16 = *((char **)t15);
    t35 = *((int64 *)t16);
    xsi_process_wait(t1, t35);

LAB29:    t15 = (t1 + 48U);
    t31 = *((char **)t15);
    t32 = (t31 + 1440U);
    *((unsigned int *)t32) = 1U;
    t33 = (t1 + 48U);
    t34 = *((char **)t33);
    t36 = (t34 + 0U);
    getcontext(t36);
    t37 = (t1 + 48U);
    t38 = *((char **)t37);
    t39 = (t38 + 1440U);
    t18 = *((unsigned int *)t39);
    if (t18 == 1)
        goto LAB30;

LAB31:    t40 = (t1 + 48U);
    t41 = *((char **)t40);
    t42 = (t41 + 1440U);
    *((unsigned int *)t42) = 3U;

LAB27:
LAB28:
LAB26:    t15 = (t0 + 1556U);
    t16 = *((char **)t15);
    t35 = *((int64 *)t16);
    xsi_process_wait(t1, t35);

LAB35:    t15 = (t1 + 48U);
    t31 = *((char **)t15);
    t32 = (t31 + 1440U);
    *((unsigned int *)t32) = 1U;
    t33 = (t1 + 48U);
    t34 = *((char **)t33);
    t36 = (t34 + 0U);
    getcontext(t36);
    t37 = (t1 + 48U);
    t38 = *((char **)t37);
    t39 = (t38 + 1440U);
    t18 = *((unsigned int *)t39);
    if (t18 == 1)
        goto LAB36;

LAB37:    t40 = (t1 + 48U);
    t41 = *((char **)t40);
    t42 = (t41 + 1440U);
    *((unsigned int *)t42) = 3U;

LAB33:
LAB34:
LAB32:    t15 = xsi_get_transient_memory(16U);
    memset(t15, 0, 16U);
    t16 = t15;
    memset(t16, (unsigned char)0, 16U);
    t18 = (0 + 3U);
    t29 = (0 + 3U);
    t30 = (t29 + t6);
    t31 = (t8 + 32U);
    t32 = *((char **)t31);
    t33 = (t32 + 40U);
    t34 = *((char **)t33);
    memcpy(t34, t15, 16U);
    xsi_driver_first_trans_delta(t8, t30, 16U, 0LL);
    t15 = xsi_get_transient_memory(16U);
    memset(t15, 0, 16U);
    t16 = t15;
    memset(t16, (unsigned char)0, 16U);
    t18 = (0 + 19U);
    t29 = (0 + 19U);
    t30 = (t29 + t6);
    t31 = (t8 + 32U);
    t32 = *((char **)t31);
    t33 = (t32 + 40U);
    t34 = *((char **)t33);
    memcpy(t34, t15, 16U);
    xsi_driver_first_trans_delta(t8, t30, 16U, 0LL);
    t18 = (0 + 0U);
    t29 = (0 + 0U);
    t30 = (t29 + t6);
    t15 = (t8 + 32U);
    t16 = *((char **)t15);
    t31 = (t16 + 40U);
    t32 = *((char **)t31);
    *((unsigned char *)t32) = (unsigned char)3;
    xsi_driver_first_trans_delta(t8, t30, 1, 0LL);
    t18 = (0 + 2U);
    t29 = (0 + 2U);
    t30 = (t29 + t6);
    t15 = (t8 + 32U);
    t16 = *((char **)t15);
    t31 = (t16 + 40U);
    t32 = *((char **)t31);
    *((unsigned char *)t32) = (unsigned char)3;
    xsi_driver_first_trans_delta(t8, t30, 1, 0LL);

LAB1:    return;
LAB3:    *((char **)t20) = *((char **)t9);
    goto LAB2;

LAB5:    *((char **)t24) = *((char **)t10);
    goto LAB4;

LAB7:    *((char **)t27) = *((char **)t11);
    goto LAB6;

LAB12:    xsi_saveStackAndSuspend(t1);
    goto LAB13;

LAB18:    xsi_saveStackAndSuspend(t1);
    goto LAB19;

LAB24:    xsi_saveStackAndSuspend(t1);
    goto LAB25;

LAB30:    xsi_saveStackAndSuspend(t1);
    goto LAB31;

LAB36:    xsi_saveStackAndSuspend(t1);
    goto LAB37;

}

void work_p_0201983097_sub_3301268761_201983097(char *t0, char *t1, char *t2, unsigned int t3, unsigned int t4, char *t5, unsigned int t6, unsigned int t7, char *t8, char *t9, char *t10)
{
    char t11[72];
    char t12[16];
    char t13[16];
    char t18[16];
    char t23[16];
    char *t14;
    char *t15;
    int t16;
    unsigned int t17;
    char *t19;
    int t20;
    char *t21;
    char *t22;
    char *t24;
    char *t25;
    char *t26;
    char *t27;
    unsigned char t28;
    char *t29;
    char *t30;
    unsigned char t31;
    unsigned int t32;
    unsigned int t33;
    char *t34;
    char *t35;
    char *t36;
    char *t37;
    int64 t38;
    char *t39;
    char *t40;
    int64 t41;
    int64 t42;
    char *t43;

LAB0:    t14 = (t13 + 0U);
    t15 = (t14 + 0U);
    *((int *)t15) = 15;
    t15 = (t14 + 4U);
    *((int *)t15) = 0;
    t15 = (t14 + 8U);
    *((int *)t15) = -1;
    t16 = (0 - 15);
    t17 = (t16 * -1);
    t17 = (t17 + 1);
    t15 = (t14 + 12U);
    *((unsigned int *)t15) = t17;
    t15 = (t18 + 0U);
    t19 = (t15 + 0U);
    *((int *)t19) = 15;
    t19 = (t15 + 4U);
    *((int *)t19) = 0;
    t19 = (t15 + 8U);
    *((int *)t19) = -1;
    t20 = (0 - 15);
    t17 = (t20 * -1);
    t17 = (t17 + 1);
    t19 = (t15 + 12U);
    *((unsigned int *)t19) = t17;
    t19 = (t11 + 4U);
    t21 = ((IEEE_P_2592010699) + 2332);
    t22 = (t19 + 52U);
    *((char **)t22) = t21;
    t24 = (t19 + 36U);
    *((char **)t24) = t23;
    xsi_type_set_default_value(t21, t23, t18);
    t25 = (t19 + 40U);
    *((char **)t25) = t18;
    t26 = (t19 + 48U);
    *((unsigned int *)t26) = 16U;
    t27 = (t12 + 4U);
    t28 = (t9 != 0);
    if (t28 == 1)
        goto LAB3;

LAB2:    t29 = (t12 + 8U);
    *((char **)t29) = t13;
    t30 = (t12 + 12U);
    t31 = (t10 != 0);
    if (t31 == 1)
        goto LAB5;

LAB4:    t17 = (0 + 0U);
    t32 = (0 + 0U);
    t33 = (t32 + t6);
    t34 = (t8 + 32U);
    t35 = *((char **)t34);
    t36 = (t35 + 40U);
    t37 = *((char **)t36);
    *((unsigned char *)t37) = (unsigned char)2;
    xsi_driver_first_trans_delta(t8, t33, 1, 0LL);
    t14 = (t0 + 876U);
    t15 = *((char **)t14);
    t38 = *((int64 *)t15);
    xsi_process_wait(t1, t38);

LAB9:    t14 = (t1 + 48U);
    t21 = *((char **)t14);
    t22 = (t21 + 1440U);
    *((unsigned int *)t22) = 1U;
    t24 = (t1 + 48U);
    t25 = *((char **)t24);
    t26 = (t25 + 0U);
    getcontext(t26);
    t34 = (t1 + 48U);
    t35 = *((char **)t34);
    t36 = (t35 + 1440U);
    t17 = *((unsigned int *)t36);
    if (t17 == 1)
        goto LAB10;

LAB11:    t37 = (t1 + 48U);
    t39 = *((char **)t37);
    t40 = (t39 + 1440U);
    *((unsigned int *)t40) = 3U;

LAB7:
LAB8:
LAB6:    t17 = (0 + 1U);
    t32 = (0 + 1U);
    t33 = (t32 + t6);
    t14 = (t8 + 32U);
    t15 = *((char **)t14);
    t21 = (t15 + 40U);
    t22 = *((char **)t21);
    *((unsigned char *)t22) = (unsigned char)2;
    xsi_driver_first_trans_delta(t8, t33, 1, 0LL);
    t14 = (t0 + 808U);
    t15 = *((char **)t14);
    t38 = *((int64 *)t15);
    t14 = (t0 + 876U);
    t21 = *((char **)t14);
    t41 = *((int64 *)t21);
    t42 = (t38 - t41);
    xsi_process_wait(t1, t42);

LAB15:    t14 = (t1 + 48U);
    t22 = *((char **)t14);
    t24 = (t22 + 1440U);
    *((unsigned int *)t24) = 1U;
    t25 = (t1 + 48U);
    t26 = *((char **)t25);
    t34 = (t26 + 0U);
    getcontext(t34);
    t35 = (t1 + 48U);
    t36 = *((char **)t35);
    t37 = (t36 + 1440U);
    t17 = *((unsigned int *)t37);
    if (t17 == 1)
        goto LAB16;

LAB17:    t39 = (t1 + 48U);
    t40 = *((char **)t39);
    t43 = (t40 + 1440U);
    *((unsigned int *)t43) = 3U;

LAB13:
LAB14:
LAB12:    t17 = (0 + 3U);
    t32 = (0 + 3U);
    t33 = (t32 + t6);
    t14 = (t8 + 32U);
    t15 = *((char **)t14);
    t21 = (t15 + 40U);
    t22 = *((char **)t21);
    memcpy(t22, t9, 16U);
    xsi_driver_first_trans_delta(t8, t33, 16U, 0LL);
    t14 = (t0 + 944U);
    t15 = *((char **)t14);
    t38 = *((int64 *)t15);
    t14 = (t0 + 1080U);
    t21 = *((char **)t14);
    t41 = *((int64 *)t21);
    t42 = (t38 - t41);
    xsi_process_wait(t1, t42);

LAB21:    t14 = (t1 + 48U);
    t22 = *((char **)t14);
    t24 = (t22 + 1440U);
    *((unsigned int *)t24) = 1U;
    t25 = (t1 + 48U);
    t26 = *((char **)t25);
    t34 = (t26 + 0U);
    getcontext(t34);
    t35 = (t1 + 48U);
    t36 = *((char **)t35);
    t37 = (t36 + 1440U);
    t17 = *((unsigned int *)t37);
    if (t17 == 1)
        goto LAB22;

LAB23:    t39 = (t1 + 48U);
    t40 = *((char **)t39);
    t43 = (t40 + 1440U);
    *((unsigned int *)t43) = 3U;

LAB19:
LAB20:
LAB18:    t17 = (0 + 0U);
    t32 = (0 + 0U);
    t33 = (t32 + t6);
    t14 = (t8 + 32U);
    t15 = *((char **)t14);
    t21 = (t15 + 40U);
    t22 = *((char **)t21);
    *((unsigned char *)t22) = (unsigned char)3;
    xsi_driver_first_trans_delta(t8, t33, 1, 0LL);
    t14 = (t0 + 1080U);
    t15 = *((char **)t14);
    t38 = *((int64 *)t15);
    xsi_process_wait(t1, t38);

LAB27:    t14 = (t1 + 48U);
    t21 = *((char **)t14);
    t22 = (t21 + 1440U);
    *((unsigned int *)t22) = 1U;
    t24 = (t1 + 48U);
    t25 = *((char **)t24);
    t26 = (t25 + 0U);
    getcontext(t26);
    t34 = (t1 + 48U);
    t35 = *((char **)t34);
    t36 = (t35 + 1440U);
    t17 = *((unsigned int *)t36);
    if (t17 == 1)
        goto LAB28;

LAB29:    t37 = (t1 + 48U);
    t39 = *((char **)t37);
    t40 = (t39 + 1440U);
    *((unsigned int *)t40) = 3U;

LAB25:
LAB26:
LAB24:    t17 = (0 + 1U);
    t32 = (0 + 1U);
    t33 = (t32 + t6);
    t14 = (t8 + 32U);
    t15 = *((char **)t14);
    t21 = (t15 + 40U);
    t22 = *((char **)t21);
    *((unsigned char *)t22) = (unsigned char)3;
    xsi_driver_first_trans_delta(t8, t33, 1, 0LL);
    t14 = (t0 + 1012U);
    t15 = *((char **)t14);
    t38 = *((int64 *)t15);
    xsi_process_wait(t1, t38);

LAB33:    t14 = (t1 + 48U);
    t21 = *((char **)t14);
    t22 = (t21 + 1440U);
    *((unsigned int *)t22) = 1U;
    t24 = (t1 + 48U);
    t25 = *((char **)t24);
    t26 = (t25 + 0U);
    getcontext(t26);
    t34 = (t1 + 48U);
    t35 = *((char **)t34);
    t36 = (t35 + 1440U);
    t17 = *((unsigned int *)t36);
    if (t17 == 1)
        goto LAB34;

LAB35:    t37 = (t1 + 48U);
    t39 = *((char **)t37);
    t40 = (t39 + 1440U);
    *((unsigned int *)t40) = 3U;

LAB31:
LAB32:
LAB30:    t14 = xsi_get_transient_memory(16U);
    memset(t14, 0, 16U);
    t15 = t14;
    memset(t15, (unsigned char)0, 16U);
    t17 = (0 + 3U);
    t32 = (0 + 3U);
    t33 = (t32 + t6);
    t21 = (t8 + 32U);
    t22 = *((char **)t21);
    t24 = (t22 + 40U);
    t25 = *((char **)t24);
    memcpy(t25, t14, 16U);
    xsi_driver_first_trans_delta(t8, t33, 16U, 0LL);

LAB1:    return;
LAB3:    *((char **)t27) = *((char **)t9);
    goto LAB2;

LAB5:    *((char **)t30) = *((char **)t10);
    goto LAB4;

LAB10:    xsi_saveStackAndSuspend(t1);
    goto LAB11;

LAB16:    xsi_saveStackAndSuspend(t1);
    goto LAB17;

LAB22:    xsi_saveStackAndSuspend(t1);
    goto LAB23;

LAB28:    xsi_saveStackAndSuspend(t1);
    goto LAB29;

LAB34:    xsi_saveStackAndSuspend(t1);
    goto LAB35;

}

unsigned char work_p_0201983097_sub_3330996147_201983097(char *t1, char *t2, char *t3, char *t4, char *t5)
{
    char t6[72];
    char t7[24];
    char t11[8];
    unsigned char t0;
    char *t8;
    char *t9;
    char *t10;
    char *t12;
    char *t13;
    char *t14;
    unsigned char t15;
    char *t16;
    char *t17;
    unsigned char t18;
    char *t19;
    char *t20;
    int t21;
    char *t22;
    int t23;
    char *t24;
    int t25;
    int t26;
    int t27;
    int t28;
    int t29;
    unsigned char t30;
    char *t31;
    int t32;
    char *t33;
    int t34;
    int t35;
    unsigned int t36;
    unsigned int t37;
    unsigned int t38;
    char *t39;
    unsigned char t40;
    unsigned char t41;
    char *t42;
    int t43;
    char *t44;
    int t45;
    int t46;
    unsigned int t47;
    char *t48;
    int t49;
    unsigned int t50;
    unsigned int t51;
    char *t52;
    unsigned char t53;
    unsigned char t54;
    unsigned char t55;
    char *t56;
    int t57;
    char *t58;
    int t59;
    int t60;
    unsigned int t61;
    unsigned int t62;
    unsigned int t63;
    char *t64;
    unsigned char t65;
    char *t66;
    int t67;
    char *t68;
    int t69;
    int t70;
    unsigned int t71;
    char *t72;
    int t73;
    unsigned int t74;
    unsigned int t75;
    char *t76;
    unsigned char t77;
    unsigned char t78;

LAB0:    t8 = (t6 + 4U);
    t9 = ((STD_STANDARD) + 552);
    t10 = (t8 + 52U);
    *((char **)t10) = t9;
    t12 = (t8 + 36U);
    *((char **)t12) = t11;
    xsi_type_set_default_value(t9, t11, 0);
    t13 = (t8 + 48U);
    *((unsigned int *)t13) = 4U;
    t14 = (t7 + 4U);
    t15 = (t2 != 0);
    if (t15 == 1)
        goto LAB3;

LAB2:    t16 = (t7 + 8U);
    *((char **)t16) = t3;
    t17 = (t7 + 12U);
    t18 = (t4 != 0);
    if (t18 == 1)
        goto LAB5;

LAB4:    t19 = (t7 + 16U);
    *((char **)t19) = t5;
    t20 = (t3 + 8U);
    t21 = *((int *)t20);
    t22 = (t3 + 4U);
    t23 = *((int *)t22);
    t24 = (t3 + 0U);
    t25 = *((int *)t24);
    t26 = t25;
    t27 = t23;

LAB6:    t28 = (t27 * t21);
    t29 = (t26 * t21);
    if (t29 <= t28)
        goto LAB7;

LAB9:    t0 = (unsigned char)1;

LAB1:    return t0;
LAB3:    *((char **)t14) = *((char **)t2);
    goto LAB2;

LAB5:    *((char **)t17) = *((char **)t4);
    goto LAB4;

LAB7:    t31 = (t3 + 0U);
    t32 = *((int *)t31);
    t33 = (t3 + 8U);
    t34 = *((int *)t33);
    t35 = (t26 - t32);
    t36 = (t35 * t34);
    t37 = (1U * t36);
    t38 = (0 + t37);
    t39 = (t2 + t38);
    t40 = *((unsigned char *)t39);
    t41 = (t40 == (unsigned char)8);
    if (t41 == 1)
        goto LAB13;

LAB14:    t42 = (t5 + 0U);
    t43 = *((int *)t42);
    t44 = (t5 + 8U);
    t45 = *((int *)t44);
    t46 = (t26 - t43);
    t47 = (t46 * t45);
    t48 = (t5 + 4U);
    t49 = *((int *)t48);
    xsi_vhdl_check_range_of_index(t43, t49, t45, t26);
    t50 = (1U * t47);
    t51 = (0 + t50);
    t52 = (t4 + t51);
    t53 = *((unsigned char *)t52);
    t54 = (t53 == (unsigned char)8);
    t30 = t54;

LAB15:    t55 = (!(t30));
    if (t55 != 0)
        goto LAB10;

LAB12:
LAB11:
LAB8:    if (t26 == t27)
        goto LAB9;

LAB20:    t23 = (t26 + t21);
    t26 = t23;
    goto LAB6;

LAB10:    t56 = (t3 + 0U);
    t57 = *((int *)t56);
    t58 = (t3 + 8U);
    t59 = *((int *)t58);
    t60 = (t26 - t57);
    t61 = (t60 * t59);
    t62 = (1U * t61);
    t63 = (0 + t62);
    t64 = (t2 + t63);
    t65 = *((unsigned char *)t64);
    t66 = (t5 + 0U);
    t67 = *((int *)t66);
    t68 = (t5 + 8U);
    t69 = *((int *)t68);
    t70 = (t26 - t67);
    t71 = (t70 * t69);
    t72 = (t5 + 4U);
    t73 = *((int *)t72);
    xsi_vhdl_check_range_of_index(t67, t73, t69, t26);
    t74 = (1U * t71);
    t75 = (0 + t74);
    t76 = (t4 + t75);
    t77 = *((unsigned char *)t76);
    t78 = (t65 != t77);
    if (t78 != 0)
        goto LAB16;

LAB18:
LAB17:    goto LAB11;

LAB13:    t30 = (unsigned char)1;
    goto LAB15;

LAB16:    t0 = (unsigned char)0;
    goto LAB1;

LAB19:    goto LAB17;

LAB21:;
}


extern void work_p_0201983097_init()
{
	static char *se[] = {(void *)work_p_0201983097_sub_4253348202_201983097,(void *)work_p_0201983097_sub_3301268761_201983097,(void *)work_p_0201983097_sub_3330996147_201983097};
	xsi_register_didat("work_p_0201983097", "isim/tb_bfm_isim_beh.exe.sim/work/p_0201983097.didat");
	xsi_register_subprogram_executes(se);
}
