/*
 * @file
 * @ingroup drv
 * @brief bitfile defines for the LPC2xxx CAN module
 * can_bitdefs.h
 *
 *  Created on: 28/10/2010
 *      Author: morten
 */

#ifndef CAN_BITDEFS_H_
#define CAN_BITDEFS_H_

typedef volatile unsigned int reg32;
#define __REG32 reg32

/* CAN acceptance filter mode register */
typedef struct {
  __REG32 AccOff          :1;
  __REG32 AccBP           :1;
  __REG32 eFCAN           :1;
  __REG32                 :29;
} __afmr_bits;


/* CAN central transmit status register */
typedef struct {
  __REG32 TS              :4;
  __REG32                 :4;
  __REG32 TBS             :4;
  __REG32                 :4;
  __REG32 TCS             :4;
  __REG32                 :12;
} __cantxsr_bits;


/* CAN central receive status register */
typedef struct {
  __REG32 RS              :4;
  __REG32                 :4;
  __REG32 RBS             :4;
  __REG32                 :4;
  __REG32 DOS             :4;
  __REG32                 :12;
} __canrxsr_bits;


/* CAN miscellaneous status register */
typedef struct {
  __REG32 ES              :4;
  __REG32                 :4;
  __REG32 BS              :4;
  __REG32                 :20;
} __canmsr_bits;


/* CAN mode register */
typedef struct {
  __REG32 RM              :1;
  __REG32 LOM             :1;
  __REG32 STM             :1;
  __REG32 TPM             :1;
  __REG32 SM              :1;
  __REG32 RPM             :1;
  __REG32                 :1;
  __REG32 TM              :1;
  __REG32                 :24;
} __canmod_bits;


/* CAN command register */
typedef struct {
  __REG32 TR              :1;
  __REG32 AT              :1;
  __REG32 RRB             :1;
  __REG32 CDO             :1;
  __REG32 SRR             :1;
  __REG32 STB1            :1;
  __REG32 STB2            :1;
  __REG32 STB3            :1;
  __REG32                 :24;
} __cancmr_bits;


/* CAN global status register */
typedef struct {
  __REG32 RBS              :1;
  __REG32 DOS              :1;
  __REG32 TBS              :1;
  __REG32 TCS              :1;
  __REG32 RS               :1;
  __REG32 TS               :1;
  __REG32 ES               :1;
  __REG32 BS               :1;
  __REG32                  :8;
  __REG32 RXERR            :8;
  __REG32 TXERR            :8;
} __cangsr_bits;


/* CAN interrupt capture register */
typedef struct {
  __REG32 RI               :1;
  __REG32 TI1              :1;
  __REG32 EI               :1;
  __REG32 DOI              :1;
  __REG32 WUI              :1;
  __REG32 EPI              :1;
  __REG32 ALI              :1;
  __REG32 BEI              :1;
  __REG32 IDI              :1;
  __REG32 TI2              :1;
  __REG32 TI3              :1;
  __REG32                  :5;
  __REG32 ERRBIT           :5;
  __REG32 ERRDIR           :1;
  __REG32 ERRC             :2;
  __REG32 ALCBIT           :5;
  __REG32                  :3;
} __canicr_bits;


/* CAN interrupt enable register */
typedef struct {
  __REG32 RIE               :1;
  __REG32 TIE1              :1;
  __REG32 EIE               :1;
  __REG32 DOIE              :1;
  __REG32 WUIE              :1;
  __REG32 EPIE              :1;
  __REG32 ALIE              :1;
  __REG32 BEIE              :1;
  __REG32 IDIE              :1;
  __REG32 TIE2              :1;
  __REG32 TIE3              :1;
  __REG32                   :21;
} __canier_bits;


/* CAN bus timing register */
typedef struct {
  __REG32 BRP                :10;
  __REG32                    :4;
  __REG32 SJW                :2;
  __REG32 TSEG1              :4;
  __REG32 TSEG2              :3;
  __REG32 SAM                :1;
  __REG32                    :8;
} __canbtr_bits;


/* CAN error warning limit register */
typedef struct {
  __REG32 EWL                :8;
  __REG32                    :24;
} __canewl_bits;


/* CAN status register */
typedef struct {
  __REG32 RBS                :1;
  __REG32 DOS                :1;
  __REG32 TBS1               :1;
  __REG32 TCS1               :1;
  __REG32 RS                 :1;
  __REG32 TS1                :1;
  __REG32 ES                 :1;
  __REG32 BS                 :1;
  __REG32 /*RBS*/            :1;
  __REG32 /*DOS*/            :1;
  __REG32 TBS2               :1;
  __REG32 TCS2               :1;
  __REG32 /*RS*/             :1;
  __REG32 TS2                :1;
  __REG32 /*ES*/             :1;
  __REG32 /*BS*/             :1;
  __REG32 /*RBS*/            :1;
  __REG32 /*DOS*/            :1;
  __REG32 TBS3               :1;
  __REG32 TCS3               :1;
  __REG32 /*RS*/             :1;
  __REG32 TS3                :1;
  __REG32 /*ES*/             :1;
  __REG32 /*BS*/             :1;
  __REG32                    :8;
} __cansr_bits;


/* CAN rx frame status register */
typedef struct {
  __REG32 IDIndex            :10;
  __REG32 BP                 :1;
  __REG32                    :5;
  __REG32 DLC                :4;
  __REG32                    :10;
  __REG32 RTR                :1;
  __REG32 FF                 :1;
} __canrfs_bits;


/* CAN rx identifier register */
typedef union {
  struct {
   __REG32 ID10_0             :11;
   __REG32                    :21;
  };
  struct {
   __REG32 ID29_18            :11;
   __REG32                    :21;
  };
  struct {
   __REG32 ID29_0             :29;
   __REG32                    :3;
  };
} __canrid_bits;


/* CAN rx data register A */
typedef struct {
  __REG32 Data1               :8;
  __REG32 Data2               :8;
  __REG32 Data3               :8;
  __REG32 Data4               :8;
} __canrda_bits;


/* CAN rx data register B */
typedef struct {
  __REG32 Data5               :8;
  __REG32 Data6               :8;
  __REG32 Data7               :8;
  __REG32 Data8               :8;
} __canrdb_bits;


/* CAN tx frame information register */
typedef struct {
  __REG32 PRIO              :8;
  __REG32                   :8;
  __REG32 DLC               :4;
  __REG32                   :10;
  __REG32 RTR               :1;
  __REG32 FF                :1;
} __cantfi_bits;


/* CAN tx identifier register */
typedef union {
  struct {
   __REG32 ID10_0             :11;
   __REG32                    :21;
  };
  struct {
   __REG32 ID29_18            :11;
   __REG32                    :21;
  };
  struct {
   __REG32 ID29_0             :29;
   __REG32                    :3;
  };
} __cantid_bits;


/* CAN tx data register A */
typedef struct {
  __REG32 Data1               :8;
  __REG32 Data2               :8;
  __REG32 Data3               :8;
  __REG32 Data4               :8;
} __cantda_bits;


/* CAN tx data register B */
typedef struct {
  __REG32 Data5               :8;
  __REG32 Data6               :8;
  __REG32 Data7               :8;
  __REG32 Data8               :8;
} __cantdb_bits;


#endif /* CAN_BITDEFS_H_ */
