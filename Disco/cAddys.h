#ifndef _CADDYS_H
#define _CADDYS_H

//===== - ENCRYPT FUNCTIONS - =====
#define ENCRYPT(X) (X)
#define DECRYPT(x) (X)

DWORD* dwDecypt(DWORD ADDRESS)
{
    return (DWORD*)( ADDRESS );
}

//==================================

#define ADR_PLAYERBASE		ENCRYPT(0x00C62388) //
#define ADR_SERVERBASE		ENCRYPT(0x00B5D0F8)	//
#define ADR_BASEPOINTER		ENCRYPT(0x00B97560)	//
#define ADR_STRUCTSIZE		ENCRYPT(0x00001CE8)	//
#define ADR_SPAWN1			ENCRYPT(0x00B165C0)	//
#define ADR_SPAWN2			ENCRYPT(0x00B165C4)	//
#define ADR_SPAWN3			ENCRYPT(0x00B165C8) //
#define ADR_SPEED			ENCRYPT(0x00984924)	//
#define ADR_SPREAD			ENCRYPT(0x00977FB8)	//
#define ADR_NOBOUNDS1		ENCRYPT(0x00B0D9E0)	//
#define ADR_NOBOUNDS2		ENCRYPT(0x00B0D9E4)	//
#define ADR_SCOPE			ENCRYPT(0x00A07238) //
#define ADR_VJUMP			ENCRYPT(0x00974CC4) //
#define OFS_5SLOT			ENCRYPT(0x001021A4) //
#define OFS_Y				ENCRYPT(0x000102D8) //
#define OFS_NFD				ENCRYPT(0x000103A4) //
#define OFS_RECOIL1			ENCRYPT(0x0000001C) //
#define OFS_RECOIL2			ENCRYPT(0x00000020) //
#define OFS_RECOIL3			ENCRYPT(0x00000024) //

#endif