/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright 2008-2013 Code Red Technologies Ltd,
 * Copyright 2013-2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 * Generated linker script file for LPC845
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v24.12 [Build 148] [2025-01-10] on 16 jul. 2025 15:13:12
 */

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x10000 /* 64K bytes (alias Flash) */  
  SRAM (rwx) : ORIGIN = 0x10000000, LENGTH = 0x3fe0 /* 16352 bytes (alias RAM) */  
  IAP_SRAM (rwx) : ORIGIN = 0x10003fe0, LENGTH = 0x20 /* 32 bytes (alias RAM2) */  
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_PROGRAM_FLASH = 0x0 + 0x10000 ; /* 64K bytes */  
  __top_Flash = 0x0 + 0x10000 ; /* 64K bytes */  
  __base_SRAM = 0x10000000  ; /* SRAM */  
  __base_RAM = 0x10000000 ; /* RAM */  
  __top_SRAM = 0x10000000 + 0x3fe0 ; /* 16352 bytes */  
  __top_RAM = 0x10000000 + 0x3fe0 ; /* 16352 bytes */  
  __base_IAP_SRAM = 0x10003fe0  ; /* IAP_SRAM */  
  __base_RAM2 = 0x10003fe0 ; /* RAM2 */  
  __top_IAP_SRAM = 0x10003fe0 + 0x20 ; /* 32 bytes */  
  __top_RAM2 = 0x10003fe0 + 0x20 ; /* 32 bytes */  
