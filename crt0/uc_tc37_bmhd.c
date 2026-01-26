/**************************************************************************************************
 * FILE: uc_tc37_bmhd.c
 *
 * ARCHITECTURE:
 *
 *    TC3xx
 *
 * UC FAMILY:
 *
 *    TC37x
 *
 * DESCRIPTION:
 *
 *    Boot Header Mode read by uC firmware after the reset at fixed location.
 *    Each uC has fixed structure how BMHD array looks like
 *
 * FILE HISTORY:
 *
 *    V1  05.2020  RO
 *    - Base reference example
 *
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* uC derivative protection guard */
#ifdef __TC37XX__

#include <stdint.h>

/* Boot Mode Header Structure type */
typedef struct
{
	uint16_t bmi;				/**< \brief Boot Mode Index (BMI)*/
	uint16_t bmhdid;			/**< \brief Boot Mode Header ID (CODE) = B359H*/
	uint32_t stad;			    /**< \brief User Code start address*/
	uint32_t crc;				/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	uint32_t crcInv;			/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	uint32_t reserved[120];	    /**< \brief Reserved area till the offset 0x1F0*/
	uint32_t confirmation;	    /**< \brief 32-bit CODE, (always same)*/
} IfxCpu_Bmhd;

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = OFF
 */
#pragma clang section rodata=".bmhd_0_orig"
const IfxCpu_Bmhd bmhd_0_orig=
{
	.bmi=				0x007E,		/**< \brief Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/**< \brief Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/**< \brief User Code start address*/
	.crc=			0x809FE5E2,		/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x7F601A1D,		/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/**< \brief 32-bit CODE, (always same)*/
};

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = OFF
 */
#pragma clang section rodata=".bmhd_1_orig"
const IfxCpu_Bmhd bmhd_1_orig=
{
	.bmi=				0x007E,		/**< \brief Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/**< \brief Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/**< \brief User Code start address*/
	.crc=			0x809FE5E2,		/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x7F601A1D,		/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/**< \brief 32-bit CODE, (always same)*/
};

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = ON
 */
#pragma clang section rodata=".bmhd_2_orig"
const IfxCpu_Bmhd bmhd_2_orig=
{
	.bmi=				0x007E,		/**< \brief Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/**< \brief Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/**< \brief User Code start address*/
	.crc=			0x809FE5E2,		/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x7F601A1D,		/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/**< \brief 32-bit CODE, (always same)*/
};

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = ON
 */
#pragma clang section rodata=".bmhd_3_orig"
const IfxCpu_Bmhd bmhd_3_orig=
{
	.bmi=				0x007E,		/**< \brief Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/**< \brief Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/**< \brief User Code start address*/
	.crc=			0x809FE5E2,		/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x7F601A1D,		/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/**< \brief 32-bit CODE, (always same)*/
};

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = OFF
 */
#pragma clang section rodata=".bmhd_0_copy"
const IfxCpu_Bmhd bmhd_0_copy=
{
	.bmi=				0x007E,		/**< \brief Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/**< \brief Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/**< \brief User Code start address*/
	.crc=			0x809FE5E2,		/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x7F601A1D,		/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/**< \brief 32-bit CODE, (always same)*/
};

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = OFF
 */
#pragma clang section rodata=".bmhd_1_copy"
const IfxCpu_Bmhd bmhd_1_copy=
{
	.bmi=				0x007E,		/**< \brief Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/**< \brief Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/**< \brief User Code start address*/
	.crc=			0x809FE5E2,		/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x7F601A1D,		/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/**< \brief 32-bit CODE, (always same)*/
};

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = ON
 */
#pragma clang section rodata=".bmhd_2_copy"
const IfxCpu_Bmhd bmhd_2_copy=
{
	.bmi=				0x007E,		/**< \brief Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/**< \brief Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/**< \brief User Code start address*/
	.crc=			0x809FE5E2,		/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x7F601A1D,		/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/**< \brief 32-bit CODE, (always same)*/
};

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = ON
 */
#pragma clang section rodata=".bmhd_3_copy"
const IfxCpu_Bmhd bmhd_3_copy=
{
	.bmi=				0x007E,		/**< \brief Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/**< \brief Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/**< \brief User Code start address*/
	.crc=			0x809FE5E2,		/**< \brief Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x7F601A1D,		/**< \brief Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/**< \brief 32-bit CODE, (always same)*/
};

#endif /* uC protection guard */
