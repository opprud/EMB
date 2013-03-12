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

#include "xsi.h"

struct XSI_INFO xsi_info;

char *IEEE_P_2592010699;
char *STD_STANDARD;
char *WORK_P_1716878159;
char *UNISIM_P_0947159679;
char *STD_TEXTIO;
char *WORK_P_4054474977;
char *WORK_P_0201983097;
char *IEEE_P_2717149903;
char *IEEE_P_1367372525;
char *UNISIM_P_3222816464;


int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    ieee_p_2592010699_init();
    std_textio_init();
    ieee_p_2717149903_init();
    ieee_p_1367372525_init();
    work_p_4054474977_init();
    work_p_0201983097_init();
    work_p_1716878159_init();
    unisim_p_0947159679_init();
    work_a_3929415402_1516540902_init();
    work_a_1591999293_1516540902_init();
    work_a_2860361472_1516540902_init();
    work_a_0533440017_1516540902_init();
    unisim_p_3222816464_init();
    unisim_a_4002211264_2683070183_init();
    unisim_a_1692151727_2982649196_init();
    unisim_a_1717487767_2982649196_init();
    unisim_a_0402065255_2679555531_init();
    unisim_a_1385814206_1233898481_init();
    work_a_3161402703_1516540902_init();
    work_a_1355048135_1516540902_init();
    work_a_3497438131_1516540902_init();
    work_a_2105629081_3671711236_init();


    xsi_register_tops("work_a_2105629081_3671711236");

    IEEE_P_2592010699 = xsi_get_engine_memory("ieee_p_2592010699");
    xsi_register_ieee_std_logic_1164(IEEE_P_2592010699);
    STD_STANDARD = xsi_get_engine_memory("std_standard");
    WORK_P_1716878159 = xsi_get_engine_memory("work_p_1716878159");
    UNISIM_P_0947159679 = xsi_get_engine_memory("unisim_p_0947159679");
    STD_TEXTIO = xsi_get_engine_memory("std_textio");
    WORK_P_4054474977 = xsi_get_engine_memory("work_p_4054474977");
    WORK_P_0201983097 = xsi_get_engine_memory("work_p_0201983097");
    IEEE_P_2717149903 = xsi_get_engine_memory("ieee_p_2717149903");
    IEEE_P_1367372525 = xsi_get_engine_memory("ieee_p_1367372525");
    UNISIM_P_3222816464 = xsi_get_engine_memory("unisim_p_3222816464");

    return xsi_run_simulation(argc, argv);

}
