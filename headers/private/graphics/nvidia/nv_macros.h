/* NV registers definitions and macros for access to them */

/* PCI_config_space */
#define NVCFG_DEVID		0x00
#define NVCFG_DEVCTRL	0x04
#define NVCFG_CLASS		0x08
#define NVCFG_HEADER	0x0c
#define NVCFG_BASE1REGS	0x10
#define NVCFG_BASE2FB	0x14
#define NVCFG_BASE3		0x18
#define NVCFG_BASE4		0x1c //unknown if used
#define NVCFG_BASE5		0x20 //unknown if used
#define NVCFG_BASE6		0x24 //unknown if used
#define NVCFG_BASE7		0x28 //unknown if used
#define NVCFG_SUBSYSID1	0x2c
#define NVCFG_ROMBASE	0x30
#define NVCFG_CAPPTR	0x34
#define NVCFG_CFG_1		0x38 //unknown if used
#define NVCFG_INTERRUPT	0x3c
#define NVCFG_SUBSYSID2	0x40
#define NVCFG_AGPREF	0x44
#define NVCFG_AGPSTAT	0x48
#define NVCFG_AGPCMD	0x4c
#define NVCFG_ROMSHADOW	0x50
#define NVCFG_VGA		0x54
#define NVCFG_SCHRATCH	0x58
#define NVCFG_CFG_10	0x5c
#define NVCFG_CFG_11	0x60
#define NVCFG_CFG_12	0x64
#define NVCFG_CFG_13	0x68 //unknown if used
#define NVCFG_CFG_14	0x6c //unknown if used
#define NVCFG_CFG_15	0x70 //unknown if used
#define NVCFG_CFG_16	0x74 //unknown if used
#define NVCFG_CFG_17	0x78 //unknown if used
#define NVCFG_CFG_18	0x7c //unknown if used
#define NVCFG_CFG_19	0x80 //unknown if used
#define NVCFG_CFG_20	0x84 //unknown if used
#define NVCFG_CFG_21	0x88 //unknown if used
#define NVCFG_CFG_22	0x8c //unknown if used
#define NVCFG_CFG_23	0x90 //unknown if used
#define NVCFG_CFG_24	0x94 //unknown if used
#define NVCFG_CFG_25	0x98 //unknown if used
#define NVCFG_CFG_26	0x9c //unknown if used
#define NVCFG_CFG_27	0xa0 //unknown if used
#define NVCFG_CFG_28	0xa4 //unknown if used
#define NVCFG_CFG_29	0xa8 //unknown if used
#define NVCFG_CFG_30	0xac //unknown if used
#define NVCFG_CFG_31	0xb0 //unknown if used
#define NVCFG_CFG_32	0xb4 //unknown if used
#define NVCFG_CFG_33	0xb8 //unknown if used
#define NVCFG_CFG_34	0xbc //unknown if used
#define NVCFG_CFG_35	0xc0 //unknown if used
#define NVCFG_CFG_36	0xc4 //unknown if used
#define NVCFG_CFG_37	0xc8 //unknown if used
#define NVCFG_CFG_38	0xcc //unknown if used
#define NVCFG_CFG_39	0xd0 //unknown if used
#define NVCFG_CFG_40	0xd4 //unknown if used
#define NVCFG_CFG_41	0xd8 //unknown if used
#define NVCFG_CFG_42	0xdc //unknown if used
#define NVCFG_CFG_43	0xe0 //unknown if used
#define NVCFG_CFG_44	0xe4 //unknown if used
#define NVCFG_CFG_45	0xe8 //unknown if used
#define NVCFG_CFG_46	0xec //unknown if used
#define NVCFG_CFG_47	0xf0 //unknown if used
#define NVCFG_CFG_48	0xf4 //unknown if used
#define NVCFG_CFG_49	0xf8 //unknown if used
#define NVCFG_CFG_50	0xfc //unknown if used

/* used NV INT registers for vblank */
#define NV32_MAIN_INTE		0x00000140
#define NV32_CRTC_INTS		0x00600100
#define NV32_CRTC_INTE		0x00600140

/* NV ACCeleration registers */
/* engine initialisation registers */
#define NVACC_FORMATS		0x00400618
#define NVACC_OFFSET0		0x00400640
#define NVACC_OFFSET1		0x00400644
#define NVACC_OFFSET2		0x00400648
#define NVACC_OFFSET3		0x0040064c
#define NVACC_OFFSET4		0x00400650
#define NVACC_OFFSET5		0x00400654
#define NVACC_BBASE0		0x00400658
#define NVACC_BBASE1		0x0040065c
#define NVACC_BBASE2		0x00400660
#define NVACC_BBASE3		0x00400664
#define NVACC_NV10_BBASE4	0x00400668
#define NVACC_NV10_BBASE5	0x0040066c
#define NVACC_PITCH0		0x00400670
#define NVACC_PITCH1		0x00400674
#define NVACC_PITCH2		0x00400678
#define NVACC_PITCH3		0x0040067c
#define NVACC_PITCH4		0x00400680
#define NVACC_BLIMIT0		0x00400684
#define NVACC_BLIMIT1		0x00400688
#define NVACC_BLIMIT2		0x0040068c
#define NVACC_BLIMIT3		0x00400690
#define NVACC_NV10_BLIMIT4	0x00400694
#define NVACC_NV10_BLIMIT5	0x00400698
#define NVACC_BPIXEL		0x00400724
#define NVACC_NV20_OFFSET0	0x00400820
#define NVACC_NV20_OFFSET1	0x00400824
#define NVACC_NV20_OFFSET2	0x00400828
#define NVACC_NV20_OFFSET3	0x0040082c
#define NVACC_STRD_FMT		0x00400830
#define NVACC_NV20_PITCH0	0x00400850
#define NVACC_NV20_PITCH1	0x00400854
#define NVACC_NV20_PITCH2	0x00400858
#define NVACC_NV20_PITCH3	0x0040085c
#define NVACC_NV20_BLIMIT6	0x00400864
#define NVACC_NV20_BLIMIT7	0x00400868
#define NVACC_NV20_BLIMIT8	0x0040086c
#define NVACC_NV20_BLIMIT9	0x00400870
#define NVACC_NV30_WHAT		0x00400890

/* specials */
#define	NVACC_DEBUG0 		0x00400080
#define	NVACC_DEBUG1 		0x00400084
#define	NVACC_DEBUG2		0x00400088
#define	NVACC_DEBUG3		0x0040008c
#define	NVACC_NV10_DEBUG4 	0x00400090
#define NVACC_ACC_INTS		0x00400100
#define NVACC_ACC_INTE		0x00400140
#define NVACC_NV10_CTX_CTRL	0x00400144
//new:
#define NVACC_NV4X_WHAT0	0x00400220
#define NVACC_NV4X_WHAT1	0x0040032c
#define NVACC_NV4X_WHAT2	0x00400610
#define NVACC_NV40P_WHAT0	0x00400820
#define NVACC_NV40P_WHAT1	0x00400824
#define NVACC_NV40P_WHAT2	0x00400828
#define NVACC_NV40P_WHAT3	0x0040082c
#define NVACC_NV40P_OFFSET0	0x00400840
#define NVACC_NV40P_OFFSET1	0x00400844
//fixme?
#define NVACC_NV40P_PITCH0	0x00400870
#define NVACC_NV40P_PITCH1	0x00400874
//end fixme.
#define NVACC_NV40P_BLIMIT6	0x004008a0
#define NVACC_NV40P_BLIMIT7	0x004008a4
#define NVACC_NV45_WHAT2	0x00400860
#define NVACC_NV45_WHAT3	0x00400864
#define NVACC_NV40_WHAT0	0x004009b0
#define NVACC_NV40_WHAT1	0x004009b4
#define NVACC_NV40_WHAT2	0x004009b8
#define NVACC_NV40_WHAT3	0x004009bc
//end new.
#define NVACC_STATUS		0x00400700
#define NVACC_NV04_SURF_TYP	0x0040070c
#define NVACC_NV10_SURF_TYP	0x00400710
#define NVACC_NV04_ACC_STAT	0x00400710
#define NVACC_NV10_ACC_STAT	0x00400714
#define NVACC_FIFO_EN		0x00400720
#define NVACC_PAT_SHP		0x00400810
#define NVACC_WINCLIP_H_0	0x00400f00
#define NVACC_WINCLIP_H_1	0x00400f04
#define NVACC_WINCLIP_H_2	0x00400f08
#define NVACC_WINCLIP_H_3	0x00400f0c
#define NVACC_WINCLIP_H_4	0x00400f10
#define NVACC_WINCLIP_H_5	0x00400f14
#define NVACC_WINCLIP_H_6	0x00400f18
#define NVACC_WINCLIP_H_7	0x00400f1c
#define NVACC_WINCLIP_V_0	0x00400f20
#define NVACC_WINCLIP_V_1	0x00400f24
#define NVACC_WINCLIP_V_2	0x00400f28
#define NVACC_WINCLIP_V_3	0x00400f2c
#define NVACC_WINCLIP_V_4	0x00400f30
#define NVACC_WINCLIP_V_5	0x00400f34
#define NVACC_WINCLIP_V_6	0x00400f38
#define NVACC_WINCLIP_V_7	0x00400f3c
#define NVACC_NV10_XFMOD0	0x00400f40
#define NVACC_NV10_XFMOD1	0x00400f44
#define NVACC_GLOB_STAT_0	0x00400f48
#define NVACC_GLOB_STAT_1	0x00400f4c
#define NVACC_NV10_PIPEADR	0x00400f50
#define NVACC_NV10_PIPEDAT	0x00400f54
/* PGRAPH unknown registers */
#define	NVACC_PGWHAT_00		0x00400e00
#define	NVACC_PGWHAT_01		0x00400e04
#define	NVACC_PGWHAT_02		0x00400e08
#define	NVACC_PGWHAT_03		0x00400e0c
#define	NVACC_PGWHAT_04		0x00400e10
#define	NVACC_PGWHAT_05		0x00400e14
#define	NVACC_PGWHAT_06		0x00400e18
#define	NVACC_PGWHAT_07		0x00400e1c
#define	NVACC_PGWHAT_08		0x00400e20
#define	NVACC_PGWHAT_09		0x00400e24
#define	NVACC_PGWHAT_0A		0x00400e28
#define	NVACC_PGWHAT_0B		0x00400e2c
#define	NVACC_PGWHAT_0C		0x00400e30
#define	NVACC_PGWHAT_0D		0x00400e34
#define	NVACC_PGWHAT_0E		0x00400e38
#define	NVACC_PGWHAT_0F		0x00400e3c
#define	NVACC_PGWHAT_10		0x00400e40
#define	NVACC_PGWHAT_11		0x00400e44
#define	NVACC_PGWHAT_12		0x00400e48
#define	NVACC_PGWHAT_13		0x00400e4c
#define	NVACC_PGWHAT_14		0x00400e50
#define	NVACC_PGWHAT_15		0x00400e54
#define	NVACC_PGWHAT_16		0x00400e58
#define	NVACC_PGWHAT_17		0x00400e5c
#define	NVACC_PGWHAT_18		0x00400e60
#define	NVACC_PGWHAT_19		0x00400e64
#define	NVACC_PGWHAT_1A		0x00400e68
#define	NVACC_PGWHAT_1B		0x00400e6c
#define	NVACC_PGWHAT_1C		0x00400e70
#define	NVACC_PGWHAT_1D		0x00400e74
#define	NVACC_PGWHAT_1E		0x00400e78
#define	NVACC_PGWHAT_1F		0x00400e7c
#define	NVACC_PGWHAT_20		0x00400e80
#define	NVACC_PGWHAT_21		0x00400e84
#define	NVACC_PGWHAT_22		0x00400e88
#define	NVACC_PGWHAT_23		0x00400e8c
#define	NVACC_PGWHAT_24		0x00400e90
#define	NVACC_PGWHAT_25		0x00400e94
#define	NVACC_PGWHAT_26		0x00400e98
#define	NVACC_PGWHAT_27		0x00400e9c
#define	NVACC_PGWHAT_28		0x00400ea0
#define	NVACC_PGWHAT_29		0x00400ea4
#define	NVACC_PGWHAT_2A		0x00400ea8
/* PGRAPH cache registers */
#define	NVACC_CACHE1_1		0x00400160
#define	NVACC_CACHE1_2		0x00400180
#define	NVACC_CACHE1_3		0x004001a0
#define	NVACC_CACHE1_4		0x004001c0
#define	NVACC_CACHE1_5		0x004001e0
#define	NVACC_CACHE2_1		0x00400164
#define	NVACC_CACHE2_2		0x00400184
#define	NVACC_CACHE2_3		0x004001a4
#define	NVACC_CACHE2_4		0x004001c4
#define	NVACC_CACHE2_5		0x004001e4
#define	NVACC_CACHE3_1		0x00400168
#define	NVACC_CACHE3_2		0x00400188
#define	NVACC_CACHE3_3		0x004001a8
#define	NVACC_CACHE3_4		0x004001c8
#define	NVACC_CACHE3_5		0x004001e8
#define	NVACC_CACHE4_1		0x0040016c
#define	NVACC_CACHE4_2		0x0040018c
#define	NVACC_CACHE4_3		0x004001ac
#define	NVACC_CACHE4_4		0x004001cc
#define	NVACC_CACHE4_5		0x004001ec
#define	NVACC_NV10_CACHE5_1	0x00400170
#define	NVACC_NV04_CTX_CTRL	0x00400170
#define	NVACC_CACHE5_2		0x00400190
#define	NVACC_CACHE5_3		0x004001b0
#define	NVACC_CACHE5_4		0x004001d0
#define	NVACC_CACHE5_5		0x004001f0
#define	NVACC_NV10_CACHE6_1	0x00400174
#define	NVACC_CACHE6_2		0x00400194
#define	NVACC_CACHE6_3		0x004001b4
#define	NVACC_CACHE6_4		0x004001d4
#define	NVACC_CACHE6_5		0x004001f4
#define	NVACC_NV10_CACHE7_1	0x00400178
#define	NVACC_CACHE7_2		0x00400198
#define	NVACC_CACHE7_3		0x004001b8
#define	NVACC_CACHE7_4		0x004001d8
#define	NVACC_CACHE7_5		0x004001f8
#define	NVACC_NV10_CACHE8_1	0x0040017c
#define	NVACC_CACHE8_2		0x0040019c
#define	NVACC_CACHE8_3		0x004001bc
#define	NVACC_CACHE8_4		0x004001dc
#define	NVACC_CACHE8_5		0x004001fc
#define	NVACC_NV10_CTX_SW1	0x0040014c
#define	NVACC_NV10_CTX_SW2	0x00400150
#define	NVACC_NV10_CTX_SW3	0x00400154
#define	NVACC_NV10_CTX_SW4	0x00400158
#define	NVACC_NV10_CTX_SW5	0x0040015c
/* engine tile registers src */
#define NVACC_NV20_FBWHAT0	0x00100200
#define NVACC_NV20_FBWHAT1	0x00100204
#define NVACC_NV10_FBTIL0AD	0x00100240
#define NVACC_NV10_FBTIL0ED	0x00100244
#define NVACC_NV10_FBTIL0PT	0x00100248
#define NVACC_NV10_FBTIL0ST	0x0010024c
#define NVACC_NV10_FBTIL1AD	0x00100250
#define NVACC_NV10_FBTIL1ED	0x00100254
#define NVACC_NV10_FBTIL1PT	0x00100258
#define NVACC_NV10_FBTIL1ST	0x0010025c
#define NVACC_NV10_FBTIL2AD	0x00100260
#define NVACC_NV10_FBTIL2ED	0x00100264
#define NVACC_NV10_FBTIL2PT	0x00100268
#define NVACC_NV10_FBTIL2ST	0x0010026c
#define NVACC_NV10_FBTIL3AD	0x00100270
#define NVACC_NV10_FBTIL3ED	0x00100274
#define NVACC_NV10_FBTIL3PT	0x00100278
#define NVACC_NV10_FBTIL3ST	0x0010027c
#define NVACC_NV10_FBTIL4AD	0x00100280
#define NVACC_NV10_FBTIL4ED	0x00100284
#define NVACC_NV10_FBTIL4PT	0x00100288
#define NVACC_NV10_FBTIL4ST	0x0010028c
#define NVACC_NV10_FBTIL5AD	0x00100290
#define NVACC_NV10_FBTIL5ED	0x00100294
#define NVACC_NV10_FBTIL5PT	0x00100298
#define NVACC_NV10_FBTIL5ST	0x0010029c
#define NVACC_NV10_FBTIL6AD	0x001002a0
#define NVACC_NV10_FBTIL6ED	0x001002a4
#define NVACC_NV10_FBTIL6PT	0x001002a8
#define NVACC_NV10_FBTIL6ST	0x001002ac
#define NVACC_NV10_FBTIL7AD	0x001002b0
#define NVACC_NV10_FBTIL7ED	0x001002b4
#define NVACC_NV10_FBTIL7PT	0x001002b8
#define NVACC_NV10_FBTIL7ST	0x001002bc
/* engine tile registers dst */
#define NVACC_NV20_WHAT_T0	0x004009a4
#define NVACC_NV20_WHAT_T1	0x004009a8
//new:
#define NVACC_NV40_WHAT_T2	0x004069a4
#define NVACC_NV40_WHAT_T3	0x004069a8
#define NVACC_NV40P_WHAT_T0	0x004009f0
#define NVACC_NV40P_WHAT_T1	0x004009f4
#define NVACC_NV40P_WHAT_T2	0x004069f0
#define NVACC_NV40P_WHAT_T3	0x004069f4
//end new.
#define NVACC_NV10_TIL0AD	0x00400b00
#define NVACC_NV10_TIL0ED	0x00400b04
#define NVACC_NV10_TIL0PT	0x00400b08
#define NVACC_NV10_TIL0ST	0x00400b0c
#define NVACC_NV10_TIL1AD	0x00400b10
#define NVACC_NV10_TIL1ED	0x00400b14
#define NVACC_NV10_TIL1PT	0x00400b18
#define NVACC_NV10_TIL1ST	0x00400b1c
#define NVACC_NV10_TIL2AD	0x00400b20
#define NVACC_NV10_TIL2ED	0x00400b24
#define NVACC_NV10_TIL2PT	0x00400b28
#define NVACC_NV10_TIL2ST	0x00400b2c
#define NVACC_NV10_TIL3AD	0x00400b30
#define NVACC_NV10_TIL3ED	0x00400b34
#define NVACC_NV10_TIL3PT	0x00400b38
#define NVACC_NV10_TIL3ST	0x00400b3c
#define NVACC_NV10_TIL4AD	0x00400b40
#define NVACC_NV10_TIL4ED	0x00400b44
#define NVACC_NV10_TIL4PT	0x00400b48
#define NVACC_NV10_TIL4ST	0x00400b4c
#define NVACC_NV10_TIL5AD	0x00400b50
#define NVACC_NV10_TIL5ED	0x00400b54
#define NVACC_NV10_TIL5PT	0x00400b58
#define NVACC_NV10_TIL5ST	0x00400b5c
#define NVACC_NV10_TIL6AD	0x00400b60
#define NVACC_NV10_TIL6ED	0x00400b64
#define NVACC_NV10_TIL6PT	0x00400b68
#define NVACC_NV10_TIL6ST	0x00400b6c
#define NVACC_NV10_TIL7AD	0x00400b70
#define NVACC_NV10_TIL7ED	0x00400b74
#define NVACC_NV10_TIL7PT	0x00400b78
#define NVACC_NV10_TIL7ST	0x00400b7c
/* cache setup registers */
#define NVACC_PF_INTSTAT	0x00002100
#define NVACC_PF_INTEN		0x00002140
#define NVACC_PF_RAMHT		0x00002210
#define NVACC_PF_RAMFC		0x00002214
#define NVACC_PF_RAMRO		0x00002218
#define NVACC_PF_CACHES		0x00002500
#define NVACC_PF_MODE		0x00002504
#define NVACC_PF_SIZE		0x0000250c
#define NVACC_PF_CACH0_PSH0	0x00003000
#define NVACC_PF_CACH0_PUL0	0x00003050
#define NVACC_PF_CACH0_PUL1	0x00003054
#define NVACC_PF_CACH1_PSH0	0x00003200
#define NVACC_PF_CACH1_PSH1	0x00003204
#define NVACC_PF_CACH1_DMAS	0x00003220
#define NVACC_PF_CACH1_DMAF	0x00003224
#define NVACC_PF_CACH1_DMAI	0x0000322c
#define NVACC_PF_CACH1_DMAC	0x00003230
#define NVACC_PF_CACH1_DMAP	0x00003240
#define NVACC_PF_CACH1_DMAG	0x00003244
#define NVACC_PF_CACH1_PUL0	0x00003250
#define NVACC_PF_CACH1_PUL1 0x00003254
#define NVACC_PF_CACH1_HASH	0x00003258
#define NVACC_PF_CACH1_ENG	0x00003280
/* Ptimer registers */
#define NVACC_PT_INTSTAT	0x00009100
#define NVACC_PT_INTEN		0x00009140
#define NVACC_PT_NUMERATOR	0x00009200
#define NVACC_PT_DENOMINATR	0x00009210
/* used PRAMIN registers */
#define NVACC_PR_CTX0_R		0x00711400
#define NVACC_PR_CTX1_R		0x00711404
#define NVACC_PR_CTX2_R		0x00711408
#define NVACC_PR_CTX3_R		0x0071140c
#define NVACC_PR_CTX0_0		0x00711420
#define NVACC_PR_CTX1_0		0x00711424
#define NVACC_PR_CTX2_0		0x00711428
#define NVACC_PR_CTX3_0		0x0071142c
#define NVACC_PR_CTX0_1		0x00711430
#define NVACC_PR_CTX1_1		0x00711434
#define NVACC_PR_CTX2_1		0x00711438
#define NVACC_PR_CTX3_1		0x0071143c
#define NVACC_PR_CTX0_2		0x00711440
#define NVACC_PR_CTX1_2		0x00711444
#define NVACC_PR_CTX2_2		0x00711448
#define NVACC_PR_CTX3_2		0x0071144c
#define NVACC_PR_CTX0_3		0x00711450
#define NVACC_PR_CTX1_3		0x00711454
#define NVACC_PR_CTX2_3		0x00711458
#define NVACC_PR_CTX3_3		0x0071145c
#define NVACC_PR_CTX0_4		0x00711460
#define NVACC_PR_CTX1_4		0x00711464
#define NVACC_PR_CTX2_4		0x00711468
#define NVACC_PR_CTX3_4		0x0071146c
#define NVACC_PR_CTX0_5		0x00711470
#define NVACC_PR_CTX1_5		0x00711474
#define NVACC_PR_CTX2_5		0x00711478
#define NVACC_PR_CTX3_5		0x0071147c
#define NVACC_PR_CTX0_6		0x00711480
#define NVACC_PR_CTX1_6		0x00711484
#define NVACC_PR_CTX2_6		0x00711488
#define NVACC_PR_CTX3_6		0x0071148c
#define NVACC_PR_CTX0_7		0x00711490
#define NVACC_PR_CTX1_7		0x00711494
#define NVACC_PR_CTX2_7		0x00711498
#define NVACC_PR_CTX3_7		0x0071149c
#define NVACC_PR_CTX0_8		0x007114a0
#define NVACC_PR_CTX1_8		0x007114a4
#define NVACC_PR_CTX2_8		0x007114a8
#define NVACC_PR_CTX3_8		0x007114ac
#define NVACC_PR_CTX0_9		0x007114b0
#define NVACC_PR_CTX1_9		0x007114b4
#define NVACC_PR_CTX2_9		0x007114b8
#define NVACC_PR_CTX3_9		0x007114bc
#define NVACC_PR_CTX0_A		0x007114c0
#define NVACC_PR_CTX1_A		0x007114c4 /* not used */
#define NVACC_PR_CTX2_A		0x007114c8
#define NVACC_PR_CTX3_A		0x007114cc
#define NVACC_PR_CTX0_B		0x007114d0
#define NVACC_PR_CTX1_B		0x007114d4
#define NVACC_PR_CTX2_B		0x007114d8
#define NVACC_PR_CTX3_B		0x007114dc
#define NVACC_PR_CTX0_C		0x007114e0
#define NVACC_PR_CTX1_C		0x007114e4
#define NVACC_PR_CTX2_C		0x007114e8
#define NVACC_PR_CTX3_C		0x007114ec
#define NVACC_PR_CTX0_D		0x007114f0
#define NVACC_PR_CTX1_D		0x007114f4
#define NVACC_PR_CTX2_D		0x007114f8
#define NVACC_PR_CTX3_D		0x007114fc
#define NVACC_PR_CTX0_E		0x00711500
#define NVACC_PR_CTX1_E		0x00711504
#define NVACC_PR_CTX2_E		0x00711508
#define NVACC_PR_CTX3_E		0x0071150c
/* used RAMHT registers (hash-table) */
#define NVACC_HT_HANDL_00	0x00710000
#define NVACC_HT_VALUE_00	0x00710004
#define NVACC_HT_HANDL_01	0x00710008
#define NVACC_HT_VALUE_01	0x0071000c
#define NVACC_HT_HANDL_02	0x00710010
#define NVACC_HT_VALUE_02	0x00710014
#define NVACC_HT_HANDL_03	0x00710018
#define NVACC_HT_VALUE_03	0x0071001c
#define NVACC_HT_HANDL_04	0x00710020
#define NVACC_HT_VALUE_04	0x00710024
#define NVACC_HT_HANDL_05	0x00710028
#define NVACC_HT_VALUE_05	0x0071002c
#define NVACC_HT_HANDL_06	0x00710030
#define NVACC_HT_VALUE_06	0x00710034
#define NVACC_HT_HANDL_10	0x00710080
#define NVACC_HT_VALUE_10	0x00710084
#define NVACC_HT_HANDL_11	0x00710088
#define NVACC_HT_VALUE_11	0x0071008c
#define NVACC_HT_HANDL_12	0x00710090
#define NVACC_HT_VALUE_12	0x00710094
#define NVACC_HT_HANDL_13	0x00710098
#define NVACC_HT_VALUE_13	0x0071009c
#define NVACC_HT_HANDL_14	0x007100a0
#define NVACC_HT_VALUE_14	0x007100a4
#define NVACC_HT_HANDL_15	0x007100a8
#define NVACC_HT_VALUE_15	0x007100ac
#define NVACC_HT_HANDL_16	0x007100b0
#define NVACC_HT_VALUE_16	0x007100b4
#define NVACC_HT_HANDL_17	0x007100b8
#define NVACC_HT_VALUE_17	0x007100bc

/* acc engine fifo setup registers (for function_register 'mappings') */
#define	NVACC_FIFO			0x00800000
#define	NVACC_FIFO_CH0		0x00800000
#define	NVACC_FIFO_CH1		0x00802000
#define	NVACC_FIFO_CH2		0x00804000
#define	NVACC_FIFO_CH3		0x00806000
#define	NVACC_FIFO_CH4		0x00808000
#define	NVACC_FIFO_CH5		0x0080a000
#define	NVACC_FIFO_CH6		0x0080c000
#define	NVACC_FIFO_CH7		0x0080e000

/* Nvidia PCI direct registers */
#define NV32_PWRUPCTRL		0x00000200
#define NV32_DUALHEAD_CTRL	0x000010f0//verify!!!
#define NV8_MISCW 			0x000c03c2
#define NV8_MISCR 			0x000c03cc
#define NV8_VSE2			0x000c03c3
#define NV8_SEQIND			0x000c03c4
#define NV16_SEQIND			0x000c03c4
#define NV8_SEQDAT			0x000c03c5
#define NV8_GRPHIND			0x000c03ce
#define NV16_GRPHIND		0x000c03ce
#define NV8_GRPHDAT			0x000c03cf

/* bootstrap info registers */
#define NV32_NV4STRAPINFO	0x00100000
#define NV32_PFB_CONFIG_0	0x00100200
#define NV32_PFB_CONFIG_1	0x00100204
#define NV32_NV10STRAPINFO	0x0010020c
#define NV32_FB_MRS1		0x001002c0
#define NV32_FB_MRS2		0x001002c8
#define NV32_NVSTRAPINFO2	0x00101000

/* registers needed for 'coldstart' */
#define NV32_PFB_DEBUG_0	0x00100080
#define NV32_PFB_REFCTRL	0x00100210
#define NV32_COREPLL		0x00680500
#define NV32_MEMPLL			0x00680504
#define NV32_PLL_CTRL		0x00680510
#define NV32_COREPLL2		0x00680570 /* NV31, NV36 only */
#define NV32_MEMPLL2		0x00680574 /* NV31, NV36 only */
#define NV32_CONFIG         0x00600804

/* primary head */
#define NV8_ATTRINDW		0x006013c0
#define NV8_ATTRDATW		0x006013c0
#define NV8_ATTRDATR		0x006013c1
#define NV8_CRTCIND			0x006013d4
#define NV16_CRTCIND		0x006013d4
#define NV8_CRTCDAT			0x006013d5
#define NV8_INSTAT1			0x006013da
#define NV32_NV10FBSTADD32	0x00600800
#define NV32_RASTER			0x00600808
#define NV32_NV10CURADD32	0x0060080c
#define NV32_CURCONF		0x00600810
#define NV32_PANEL_PWR		0x0060081c
#define NV32_FUNCSEL		0x00600860

/* secondary head */
#define NV8_ATTR2INDW		0x006033c0
#define NV8_ATTR2DATW		0x006033c0
#define NV8_ATTR2DATR		0x006033c1
#define NV8_CRTC2IND		0x006033d4
#define NV16_CRTC2IND		0x006033d4
#define NV8_CRTC2DAT		0x006033d5
#define NV8_2INSTAT1		0x006033da//verify!!!
#define NV32_NV10FB2STADD32	0x00602800
#define NV32_RASTER2		0x00602808
#define NV32_NV10CUR2ADD32	0x0060280c
#define NV32_2CURCONF		0x00602810
#define NV32_2PANEL_PWR		0x0060281c//verify!!!
#define NV32_2FUNCSEL		0x00602860

/* Nvidia DAC direct registers (standard VGA palette RAM registers) */
/* primary head */
#define NV8_PALMASK			0x006813c6
#define NV8_PALINDR			0x006813c7
#define NV8_PALINDW			0x006813c8
#define NV8_PALDATA			0x006813c9
/* secondary head */
#define NV8_PAL2MASK		0x006833c6
#define NV8_PAL2INDR		0x006833c7
#define NV8_PAL2INDW		0x006833c8
#define NV8_PAL2DATA		0x006833c9

/* Nvidia PCI direct DAC registers (32bit) */
/* primary head */
#define NVDAC_CURPOS		0x00680300
#define NVDAC_NV10_CURSYNC	0x00680404
#define NVDAC_PIXPLLC		0x00680508
#define NVDAC_PLLSEL		0x0068050c
#define NVDAC_OUTPUT		0x0068052c
#define NVDAC_PIXPLLC2		0x00680578
#define NVDAC_GENCTRL		0x00680600
#define NVDAC_TSTCTRL		0x00680608
#define NVDAC_TSTDATA		0x00680610
#define NVDAC_TV_SETUP		0x00680700
/* (flatpanel registers: confirmed for TNT2 and up) */
#define NVDAC_FP_VDISPEND	0x00680800
#define NVDAC_FP_VTOTAL		0x00680804
#define NVDAC_FP_VCRTC		0x00680808
#define NVDAC_FP_VSYNC_S	0x0068080c
#define NVDAC_FP_VSYNC_E	0x00680810
#define NVDAC_FP_VVALID_S	0x00680814
#define NVDAC_FP_VVALID_E	0x00680818
#define NVDAC_FP_HDISPEND	0x00680820
#define NVDAC_FP_HTOTAL		0x00680824
#define NVDAC_FP_HCRTC		0x00680828
#define NVDAC_FP_HSYNC_S	0x0068082c
#define NVDAC_FP_HSYNC_E	0x00680830
#define NVDAC_FP_HVALID_S	0x00680834
#define NVDAC_FP_HVALID_E	0x00680838
#define NVDAC_FP_CHKSUM		0x00680840
#define NVDAC_FP_TST_CTRL	0x00680844
#define NVDAC_FP_TG_CTRL	0x00680848
#define NVDAC_FP_DEBUG0		0x00680880
#define NVDAC_FP_DEBUG1		0x00680884
#define NVDAC_FP_DEBUG2		0x00680888
#define NVDAC_FP_DEBUG3		0x0068088c
/* secondary head */
#define NVDAC2_CURPOS		0x00682300
#define NVDAC2_PIXPLLC		0x00680520
#define NVDAC2_OUTPUT		0x0068252c
#define NVDAC2_PIXPLLC2		0x0068057c
#define NVDAC2_GENCTRL		0x00682600
#define NVDAC2_TSTCTRL		0x00682608
#define NVDAC2_TV_SETUP		0x00682700 //verify!!!
/* (flatpanel registers) */
#define NVDAC2_FP_VDISPEND	0x00682800
#define NVDAC2_FP_VTOTAL	0x00682804
#define NVDAC2_FP_VCRTC		0x00682808
#define NVDAC2_FP_VSYNC_S	0x0068280c
#define NVDAC2_FP_VSYNC_E	0x00682810
#define NVDAC2_FP_VVALID_S	0x00682814
#define NVDAC2_FP_VVALID_E	0x00682818
#define NVDAC2_FP_HDISPEND	0x00682820
#define NVDAC2_FP_HTOTAL	0x00682824
#define NVDAC2_FP_HCRTC		0x00682828
#define NVDAC2_FP_HSYNC_S	0x0068282c
#define NVDAC2_FP_HSYNC_E	0x00682830
#define NVDAC2_FP_HVALID_S	0x00682834
#define NVDAC2_FP_HVALID_E	0x00682838
#define NVDAC2_FP_CHKSUM	0x00682840
#define NVDAC2_FP_TST_CTRL	0x00682844
#define NVDAC2_FP_TG_CTRL	0x00682848
#define NVDAC2_FP_DEBUG0	0x00682880
#define NVDAC2_FP_DEBUG1	0x00682884
#define NVDAC2_FP_DEBUG2	0x00682888
#define NVDAC2_FP_DEBUG3	0x0068288c

/* Nvidia CRTC indexed registers */
/* VGA standard registers: */
#define NVCRTCX_HTOTAL		0x00
#define NVCRTCX_HDISPE		0x01
#define NVCRTCX_HBLANKS		0x02
#define NVCRTCX_HBLANKE		0x03
#define NVCRTCX_HSYNCS		0x04
#define NVCRTCX_HSYNCE		0x05
#define NVCRTCX_VTOTAL		0x06
#define NVCRTCX_OVERFLOW	0x07
#define NVCRTCX_PRROWSCN	0x08
#define NVCRTCX_MAXSCLIN	0x09
#define NVCRTCX_VGACURCTRL	0x0a
#define NVCRTCX_FBSTADDH	0x0c
#define NVCRTCX_FBSTADDL	0x0d
#define NVCRTCX_VSYNCS		0x10
#define NVCRTCX_VSYNCE		0x11
#define NVCRTCX_VDISPE		0x12
#define NVCRTCX_PITCHL		0x13
#define NVCRTCX_VBLANKS		0x15
#define NVCRTCX_VBLANKE		0x16
#define NVCRTCX_MODECTL		0x17
#define NVCRTCX_LINECOMP	0x18
/* Nvidia specific registers: */
#define NVCRTCX_REPAINT0	0x19
#define NVCRTCX_REPAINT1	0x1a
#define NVCRTCX_FIFO		0x1b
#define NVCRTCX_LOCK		0x1f
#define NVCRTCX_FIFO_LWM	0x20
#define NVCRTCX_BUFFER		0x21
#define NVCRTCX_LSR			0x25
#define NVCRTCX_PIXEL		0x28
#define NVCRTCX_HEB			0x2d
#define NVCRTCX_CURCTL2		0x2f
#define NVCRTCX_CURCTL1		0x30
#define NVCRTCX_CURCTL0		0x31
#define NVCRTCX_LCD			0x33
#define NVCRTCX_RMA			0x38
#define NVCRTCX_INTERLACE	0x39
#define NVCRTCX_TREG		0x3d
#define NVCRTCX_EXTRA		0x41
#define NVCRTCX_OWNER		0x44
#define NVCRTCX_FP_HTIMING	0x53
#define NVCRTCX_FP_VTIMING	0x54
#define NVCRTCX_0x59		0x59
#define NVCRTCX_0x9f		0x9f

/* Nvidia ATTRIBUTE indexed registers */
/* VGA standard registers: */
#define NVATBX_MODECTL		0x10
#define NVATBX_OSCANCOLOR	0x11
#define NVATBX_COLPLANE_EN	0x12
#define NVATBX_HORPIXPAN	0x13
#define NVATBX_COLSEL		0x14

/* Nvidia SEQUENCER indexed registers */
/* VGA standard registers: */
#define NVSEQX_RESET		0x00
#define NVSEQX_CLKMODE		0x01
#define NVSEQX_MEMMODE		0x04

/* Nvidia GRAPHICS indexed registers */
/* VGA standard registers: */
#define NVGRPHX_ENSETRESET	0x01
#define NVGRPHX_DATAROTATE	0x03
#define NVGRPHX_READMAPSEL	0x04
#define NVGRPHX_MODE		0x05
#define NVGRPHX_MISC		0x06
#define NVGRPHX_BITMASK		0x08

/* Nvidia BES (Back End Scaler) registers (< NV10, including NV03, so RIVA128(ZX)) */
#define NVBES_NV04_INTE		0x00680140
#define NVBES_NV04_ISCALVH	0x00680200
#define NVBES_NV04_CTRL_V	0x00680204
#define NVBES_NV04_CTRL_H	0x00680208
#define NVBES_NV04_OE_STATE	0x00680224
#define NVBES_NV04_SU_STATE	0x00680228
#define NVBES_NV04_RM_STATE	0x0068022c
#define NVBES_NV04_DSTREF	0x00680230
#define NVBES_NV04_DSTSIZE	0x00680234
#define NVBES_NV04_FIFOTHRS	0x00680238
#define NVBES_NV04_FIFOBURL	0x0068023c
#define NVBES_NV04_COLKEY	0x00680240
#define NVBES_NV04_GENCTRL	0x00680244
#define NVBES_NV04_RED_AMP	0x00680280
#define NVBES_NV04_GRN_AMP	0x00680284
#define NVBES_NV04_BLU_AMP	0x00680288
#define NVBES_NV04_SAT		0x0068028c
/* buffer 0 */
#define NVBES_NV04_0BUFADR	0x0068020c
#define NVBES_NV04_0SRCPTCH	0x00680214
#define NVBES_NV04_0OFFSET	0x0068021c
/* buffer 1 */
#define NVBES_NV04_1BUFADR	0x00680210
#define NVBES_NV04_1SRCPTCH	0x00680218
#define NVBES_NV04_1OFFSET	0x00680220

/* Nvidia BES (Back End Scaler) registers (>= NV10) */
#define NVBES_NV10_INTE		0x00008140
#define NVBES_NV10_BUFSEL	0x00008700
#define NVBES_NV10_GENCTRL	0x00008704
#define NVBES_NV10_COLKEY	0x00008b00
/* buffer 0 */
#define NVBES_NV10_0BUFADR	0x00008900
#define NVBES_NV10_0MEMMASK	0x00008908
#define NVBES_NV10_0BRICON	0x00008910
#define NVBES_NV10_0SAT		0x00008918
#define NVBES_NV10_0OFFSET	0x00008920
#define NVBES_NV10_0SRCSIZE	0x00008928
#define NVBES_NV10_0SRCREF	0x00008930
#define NVBES_NV10_0ISCALH	0x00008938
#define NVBES_NV10_0ISCALV	0x00008940
#define NVBES_NV10_0DSTREF	0x00008948
#define NVBES_NV10_0DSTSIZE	0x00008950
#define NVBES_NV10_0SRCPTCH	0x00008958
/* buffer 1 */
#define NVBES_NV10_1BUFADR	0x00008904
#define NVBES_NV10_1MEMMASK	0x0000890c
#define NVBES_NV10_1BRICON	0x00008914
#define NVBES_NV10_1SAT		0x0000891c
#define NVBES_NV10_1OFFSET	0x00008924
#define NVBES_NV10_1SRCSIZE	0x0000892c
#define NVBES_NV10_1SRCREF	0x00008934
#define NVBES_NV10_1ISCALH	0x0000893c
#define NVBES_NV10_1ISCALV	0x00008944
#define NVBES_NV10_1DSTREF	0x0000894c
#define NVBES_NV10_1DSTSIZE	0x00008954
#define NVBES_NV10_1SRCPTCH	0x0000895c
/* Nvidia MPEG2 hardware decoder (GeForce4MX only) */
#define NVBES_DEC_GENCTRL	0x00001588
//new:
#define NV32_NV45_WHAT10	0x00001700
#define NV32_NV45_WHAT11	0x00001704
#define NV32_NV45_WHAT12	0x00001708
#define NV32_NV45_WHAT13	0x0000170c
//end new

//old:
/*MAVEN registers (<= G400) */
#define NVMAV_PGM            0x3E
#define NVMAV_PIXPLLM        0x80
#define NVMAV_PIXPLLN        0x81
#define NVMAV_PIXPLLP        0x82
#define NVMAV_GAMMA1         0x83
#define NVMAV_GAMMA2         0x84
#define NVMAV_GAMMA3         0x85
#define NVMAV_GAMMA4         0x86
#define NVMAV_GAMMA5         0x87
#define NVMAV_GAMMA6         0x88
#define NVMAV_GAMMA7         0x89
#define NVMAV_GAMMA8         0x8A
#define NVMAV_GAMMA9         0x8B
#define NVMAV_MONSET         0x8C
#define NVMAV_TEST           0x8D
#define NVMAV_WREG_0X8E_L    0x8E
#define NVMAV_WREG_0X8E_H    0x8F
#define NVMAV_HSCALETV       0x90
#define NVMAV_TSCALETVL      0x91
#define NVMAV_TSCALETVH      0x92
#define NVMAV_FFILTER        0x93
#define NVMAV_MONEN          0x94
#define NVMAV_RESYNC         0x95
#define NVMAV_LASTLINEL      0x96
#define NVMAV_LASTLINEH      0x97
#define NVMAV_WREG_0X98_L    0x98
#define NVMAV_WREG_0X98_H    0x99
#define NVMAV_HSYNCLENL      0x9A
#define NVMAV_HSYNCLENH      0x9B
#define NVMAV_HSYNCSTRL      0x9C
#define NVMAV_HSYNCSTRH      0x9D
#define NVMAV_HDISPLAYL      0x9E
#define NVMAV_HDISPLAYH      0x9F
#define NVMAV_HTOTALL        0xA0
#define NVMAV_HTOTALH        0xA1
#define NVMAV_VSYNCLENL      0xA2
#define NVMAV_VSYNCLENH      0xA3
#define NVMAV_VSYNCSTRL      0xA4
#define NVMAV_VSYNCSTRH      0xA5
#define NVMAV_VDISPLAYL      0xA6
#define NVMAV_VDISPLAYH      0xA7
#define NVMAV_VTOTALL        0xA8
#define NVMAV_VTOTALH        0xA9
#define NVMAV_HVIDRSTL       0xAA
#define NVMAV_HVIDRSTH       0xAB
#define NVMAV_VVIDRSTL       0xAC
#define NVMAV_VVIDRSTH       0xAD
#define NVMAV_VSOMETHINGL    0xAE
#define NVMAV_VSOMETHINGH    0xAF
#define NVMAV_OUTMODE        0xB0
#define NVMAV_LOCK           0xB3
#define NVMAV_LUMA           0xB9
#define NVMAV_VDISPLAYTV     0xBE
#define NVMAV_STABLE         0xBF
#define NVMAV_HDISPLAYTV     0xC2
#define NVMAV_BREG_0XC6      0xC6
//end old.

/* Macros for convenient accesses to the NV chips */
#define NV_REG8(r_)  ((vuint8  *)regs)[(r_)]
#define NV_REG16(r_) ((vuint16 *)regs)[(r_) >> 1]
#define NV_REG32(r_) ((vuint32 *)regs)[(r_) >> 2]

/* read and write to PCI config space */
#define CFGR(A)   (nv_pci_access.offset=NVCFG_##A, ioctl(fd,NV_GET_PCI, &nv_pci_access,sizeof(nv_pci_access)), nv_pci_access.value)
#define CFGW(A,B) (nv_pci_access.offset=NVCFG_##A, nv_pci_access.value = B, ioctl(fd,NV_SET_PCI,&nv_pci_access,sizeof(nv_pci_access)))

/* read and write from ISA I/O space */
#define ISAWB(A,B)(nv_isa_access.adress=A, nv_isa_access.data = (uint8)B, nv_isa_access.size = 1, ioctl(fd,NV_ISA_OUT, &nv_isa_access,sizeof(nv_isa_access)))
#define ISAWW(A,B)(nv_isa_access.adress=A, nv_isa_access.data = B, nv_isa_access.size = 2, ioctl(fd,NV_ISA_OUT, &nv_isa_access,sizeof(nv_isa_access)))
#define ISARB(A)  (nv_isa_access.adress=A, ioctl(fd,NV_ISA_IN, &nv_isa_access,sizeof(nv_isa_access)), (uint8)nv_isa_access.data)
#define ISARW(A)  (nv_isa_access.adress=A, ioctl(fd,NV_ISA_IN, &nv_isa_access,sizeof(nv_isa_access)), nv_isa_access.data)

/* read and write from the dac registers */
#define DACR(A)   (NV_REG32(NVDAC_##A))
#define DACW(A,B) (NV_REG32(NVDAC_##A)=B)

/* read and write from the secondary dac registers */
#define DAC2R(A)   (NV_REG32(NVDAC2_##A))
#define DAC2W(A,B) (NV_REG32(NVDAC2_##A)=B)

/* read and write from the backend scaler registers */
#define BESR(A)   (NV_REG32(NVBES_##A))
#define BESW(A,B) (NV_REG32(NVBES_##A)=B)

/* read and write from CRTC indexed registers */
#define CRTCW(A,B)(NV_REG16(NV16_CRTCIND) = ((NVCRTCX_##A) | ((B) << 8)))
#define CRTCR(A)  (NV_REG8(NV8_CRTCIND) = (NVCRTCX_##A), NV_REG8(NV8_CRTCDAT))

/* read and write from second CRTC indexed registers */
#define CRTC2W(A,B)(NV_REG16(NV16_CRTC2IND) = ((NVCRTCX_##A) | ((B) << 8)))
#define CRTC2R(A)  (NV_REG8(NV8_CRTC2IND) = (NVCRTCX_##A), NV_REG8(NV8_CRTC2DAT))

/* read and write from ATTRIBUTE indexed registers */
#define ATBW(A,B)(NV_REG8(NV8_INSTAT1), NV_REG8(NV8_ATTRINDW) = ((NVATBX_##A) | 0x20), NV_REG8(NV8_ATTRDATW) = (B))
#define ATBR(A)  (NV_REG8(NV8_INSTAT1), NV_REG8(NV8_ATTRINDW) = ((NVATBX_##A) | 0x20), NV_REG8(NV8_ATTRDATR))

/* read and write from ATTRIBUTE indexed registers */
#define ATB2W(A,B)(NV_REG8(NV8_INSTAT1), NV_REG8(NV8_ATTR2INDW) = ((NVATBX_##A) | 0x20), NV_REG8(NV8_ATTR2DATW) = (B))
#define ATB2R(A)  (NV_REG8(NV8_INSTAT1), NV_REG8(NV8_ATTR2INDW) = ((NVATBX_##A) | 0x20), NV_REG8(NV8_ATTR2DATR))

/* read and write from SEQUENCER indexed registers */
#define SEQW(A,B)(NV_REG16(NV16_SEQIND) = ((NVSEQX_##A) | ((B) << 8)))
#define SEQR(A)  (NV_REG8(NV8_SEQIND) = (NVSEQX_##A), NV_REG8(NV8_SEQDAT))

/* read and write from PCI GRAPHICS indexed registers */
#define GRPHW(A,B)(NV_REG16(NV16_GRPHIND) = ((NVGRPHX_##A) | ((B) << 8)))
#define GRPHR(A)  (NV_REG8(NV8_GRPHIND) = (NVGRPHX_##A), NV_REG8(NV8_GRPHDAT))

/* read and write from the acceleration engine registers */
#define ACCR(A)    (NV_REG32(NVACC_##A))
#define ACCW(A,B)  (NV_REG32(NVACC_##A)=B)

//old:
/* read and write from maven (<= G400) */
#define MAVR(A)     (i2c_maven_read (NVMAV_##A ))
#define MAVW(A,B)   (i2c_maven_write(NVMAV_##A ,B))
#define MAVRW(A)    (i2c_maven_read (NVMAV_##A )|(i2c_maven_read(NVMAV_##A +1)<<8))
#define MAVWW(A,B)  (i2c_maven_write(NVMAV_##A ,B &0xFF),i2c_maven_write(NVMAV_##A +1,B >>8))
