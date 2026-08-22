
;CodeVisionAVR C Compiler V3.12 Advanced
;(C) Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Build configuration    : Debug
;Chip type              : ATmega328P
;Program type           : Application
;Clock frequency        : 16,000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External RAM size      : 0
;Data Stack size        : 512 byte(s)
;Heap size              : 0 byte(s)
;Promote 'char' to 'int': Yes
;'char' is unsigned     : Yes
;8 bit enums            : Yes
;Global 'const' stored in FLASH: Yes
;Enhanced function parameter passing: Yes
;Enhanced core instructions: On
;Automatic register allocation for global variables: On
;Smart register allocation: On

	#define _MODEL_SMALL_

	#pragma AVRPART ADMIN PART_NAME ATmega328P
	#pragma AVRPART MEMORY PROG_FLASH 32768
	#pragma AVRPART MEMORY EEPROM 1024
	#pragma AVRPART MEMORY INT_SRAM SIZE 2048
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100

	#define CALL_SUPPORTED 1

	.LISTMAC
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU EECR=0x1F
	.EQU EEDR=0x20
	.EQU EEARL=0x21
	.EQU EEARH=0x22
	.EQU SPSR=0x2D
	.EQU SPDR=0x2E
	.EQU SMCR=0x33
	.EQU MCUSR=0x34
	.EQU MCUCR=0x35
	.EQU WDTCSR=0x60
	.EQU UCSR0A=0xC0
	.EQU UDR0=0xC6
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU GPIOR0=0x1E

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0100
	.EQU __SRAM_END=0x08FF
	.EQU __DSTACK_SIZE=0x0200
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	PUSH R26
	PUSH R27
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMRDW
	POP  R27
	POP  R26
	ICALL
	.ENDM

	.MACRO __CALL2EX
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	CALL __EEPROMRDD
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z
	MOVW R30,R0
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _demXung=R3
	.DEF _demXung_msb=R4
	.DEF _rx_wr_index0=R6
	.DEF _rx_rd_index0=R5
	.DEF _rx_counter0=R8
	.DEF _tx_wr_index0=R7
	.DEF _tx_rd_index0=R10
	.DEF _tx_counter0=R9
	.DEF _nutSW1=R12
	.DEF _nutSW2=R11
	.DEF _nutSW1truoc=R14
	.DEF _nutSW2truoc=R13

;GPIOR0 INITIALIZATION VALUE
	.EQU __GPIOR0_INIT=0x00

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x00
	JMP  _ext_int1_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _usart_rx_isr
	JMP  0x00
	JMP  _usart_tx_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00

_tbl10_G100:
	.DB  0x10,0x27,0xE8,0x3,0x64,0x0,0xA,0x0
	.DB  0x1,0x0
_tbl16_G100:
	.DB  0x0,0x10,0x0,0x1,0x10,0x0,0x1,0x0

;GLOBAL REGISTER VARIABLES INITIALIZATION
__REG_VARS:
	.DB  0x0,0x0,0x0,0x0
	.DB  0x0,0x0


__GLOBAL_INI_TBL:
	.DW  0x06
	.DW  0x05
	.DW  __REG_VARS*2

_0xFFFFFFFF:
	.DW  0

#define __GLOBAL_INI_TBL_PRESENT 1

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  MCUCR,R31
	OUT  MCUCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,LOW(__SRAM_START)
	LDI  R27,HIGH(__SRAM_START)
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;GPIOR0 INITIALIZATION
	LDI  R30,__GPIOR0_INIT
	OUT  GPIOR0,R30

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x300

	.CSEG
;#include <mega328p.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif
;#include <delay.h>
;
;
;//------------External Interrupts----------
;// External Interrupt 1 service routine
;unsigned int demXung; // Variable to hold the count of button presses for switch 1 and switch 2
;interrupt [EXT_INT1] void ext_int1_isr(void)
; 0000 0009 {

	.CSEG
_ext_int1_isr:
; .FSTART _ext_int1_isr
	CALL SUBOPT_0x0
; 0000 000A     // Place your code here
; 0000 000B     demXung++; // Increment the count of button presses for switch 1
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	__ADDWRR 3,4,30,31
; 0000 000C }
	RJMP _0xF3
; .FEND
;
;//----------ADC----------
;// Voltage Reference: AVCC pin
;#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))
;
;// Read the AD conversion result
;unsigned int read_adc(unsigned char adc_input)
; 0000 0014 {
_read_adc:
; .FSTART _read_adc
; 0000 0015 ADMUX=adc_input | ADC_VREF_TYPE;
	ST   -Y,R26
;	adc_input -> Y+0
	LD   R30,Y
	ORI  R30,0x40
	STS  124,R30
; 0000 0016 // Delay needed for the stabilization of the ADC input voltage
; 0000 0017 delay_us(10);
	__DELAY_USB 53
; 0000 0018 // Start the AD conversion
; 0000 0019 ADCSRA|=(1<<ADSC);
	LDS  R30,122
	ORI  R30,0x40
	STS  122,R30
; 0000 001A // Wait for the AD conversion to complete
; 0000 001B while ((ADCSRA & (1<<ADIF))==0);
_0x3:
	LDS  R30,122
	ANDI R30,LOW(0x10)
	BREQ _0x3
; 0000 001C ADCSRA|=(1<<ADIF);
	LDS  R30,122
	ORI  R30,0x10
	STS  122,R30
; 0000 001D return ADCW;
	LDS  R30,120
	LDS  R31,120+1
	RJMP _0x2060001
; 0000 001E }
; .FEND
;
;// ------------UART-----------
;#define DATA_REGISTER_EMPTY (1<<UDRE0)
;#define RX_COMPLETE (1<<RXC0)
;#define FRAMING_ERROR (1<<FE0)
;#define PARITY_ERROR (1<<UPE0)
;#define DATA_OVERRUN (1<<DOR0)
;
;// USART Receiver buffer
;#define RX_BUFFER_SIZE0 64
;char rx_buffer0[RX_BUFFER_SIZE0];
;
;#if RX_BUFFER_SIZE0 <= 256
;unsigned char rx_wr_index0=0,rx_rd_index0=0;
;#else
;unsigned int rx_wr_index0=0,rx_rd_index0=0;
;#endif
;
;#if RX_BUFFER_SIZE0 < 256
;unsigned char rx_counter0=0;
;#else
;unsigned int rx_counter0=0;
;#endif
;
;// This flag is set on USART Receiver buffer overflow
;bit rx_buffer_overflow0;
;
;// USART Receiver interrupt service routine
;interrupt [USART_RXC] void usart_rx_isr(void)
; 0000 003C {
_usart_rx_isr:
; .FSTART _usart_rx_isr
	CALL SUBOPT_0x0
; 0000 003D char status,data;
; 0000 003E status=UCSR0A;
	ST   -Y,R17
	ST   -Y,R16
;	status -> R17
;	data -> R16
	LDS  R17,192
; 0000 003F data=UDR0;
	LDS  R16,198
; 0000 0040 if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	MOV  R30,R17
	ANDI R30,LOW(0x1C)
	BRNE _0x6
; 0000 0041    {
; 0000 0042    rx_buffer0[rx_wr_index0++]=data;
	MOV  R30,R6
	INC  R6
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer0)
	SBCI R31,HIGH(-_rx_buffer0)
	ST   Z,R16
; 0000 0043 #if RX_BUFFER_SIZE0 == 256
; 0000 0044    // special case for receiver buffer size=256
; 0000 0045    if (++rx_counter0 == 0) rx_buffer_overflow0=1;
; 0000 0046 #else
; 0000 0047    if (rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
	LDI  R30,LOW(64)
	CP   R30,R6
	BRNE _0x7
	CLR  R6
; 0000 0048    if (++rx_counter0 == RX_BUFFER_SIZE0)
_0x7:
	INC  R8
	LDI  R30,LOW(64)
	CP   R30,R8
	BRNE _0x8
; 0000 0049       {
; 0000 004A       rx_counter0=0;
	CLR  R8
; 0000 004B       rx_buffer_overflow0=1;
	SBI  0x1E,0
; 0000 004C       }
; 0000 004D #endif
; 0000 004E    }
_0x8:
; 0000 004F }
_0x6:
	LD   R16,Y+
	LD   R17,Y+
	RJMP _0xF3
; .FEND
;
;#ifndef _DEBUG_TERMINAL_IO_
;// Get a character from the USART Receiver buffer
;#define _ALTERNATE_GETCHAR_
;#pragma used+
;char getchar(void)
; 0000 0056 {
; 0000 0057 char data;
; 0000 0058 while (rx_counter0==0);
;	data -> R17
; 0000 0059 data=rx_buffer0[rx_rd_index0++];
; 0000 005A #if RX_BUFFER_SIZE0 != 256
; 0000 005B if (rx_rd_index0 == RX_BUFFER_SIZE0) rx_rd_index0=0;
; 0000 005C #endif
; 0000 005D #asm("cli")
; 0000 005E --rx_counter0;
; 0000 005F #asm("sei")
; 0000 0060 return data;
; 0000 0061 }
;#pragma used-
;#endif
;
;// USART Transmitter buffer
;#define TX_BUFFER_SIZE0 64
;char tx_buffer0[TX_BUFFER_SIZE0];
;
;#if TX_BUFFER_SIZE0 <= 256
;unsigned char tx_wr_index0=0,tx_rd_index0=0;
;#else
;unsigned int tx_wr_index0=0,tx_rd_index0=0;
;#endif
;
;#if TX_BUFFER_SIZE0 < 256
;unsigned char tx_counter0=0;
;#else
;unsigned int tx_counter0=0;
;#endif
;
;// USART Transmitter interrupt service routine
;interrupt [USART_TXC] void usart_tx_isr(void)
; 0000 0077 {
_usart_tx_isr:
; .FSTART _usart_tx_isr
	CALL SUBOPT_0x0
; 0000 0078 if (tx_counter0)
	TST  R9
	BREQ _0xF
; 0000 0079    {
; 0000 007A    --tx_counter0;
	DEC  R9
; 0000 007B    UDR0=tx_buffer0[tx_rd_index0++];
	MOV  R30,R10
	INC  R10
	LDI  R31,0
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R30,Z
	STS  198,R30
; 0000 007C #if TX_BUFFER_SIZE0 != 256
; 0000 007D    if (tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
	LDI  R30,LOW(64)
	CP   R30,R10
	BRNE _0x10
	CLR  R10
; 0000 007E #endif
; 0000 007F    }
_0x10:
; 0000 0080 }
_0xF:
_0xF3:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
; .FEND
;
;#ifndef _DEBUG_TERMINAL_IO_
;// Write a character to the USART Transmitter buffer
;#define _ALTERNATE_PUTCHAR_
;#pragma used+
;void putchar(char c)
; 0000 0087 {
_putchar:
; .FSTART _putchar
; 0000 0088 while (tx_counter0 == TX_BUFFER_SIZE0);
	ST   -Y,R26
;	c -> Y+0
_0x11:
	LDI  R30,LOW(64)
	CP   R30,R9
	BREQ _0x11
; 0000 0089 #asm("cli")
	cli
; 0000 008A if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
	TST  R9
	BRNE _0x15
	LDS  R30,192
	ANDI R30,LOW(0x20)
	BRNE _0x14
_0x15:
; 0000 008B    {
; 0000 008C    tx_buffer0[tx_wr_index0++]=c;
	MOV  R30,R7
	INC  R7
	LDI  R31,0
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R26,Y
	STD  Z+0,R26
; 0000 008D #if TX_BUFFER_SIZE0 != 256
; 0000 008E    if (tx_wr_index0 == TX_BUFFER_SIZE0) tx_wr_index0=0;
	LDI  R30,LOW(64)
	CP   R30,R7
	BRNE _0x17
	CLR  R7
; 0000 008F #endif
; 0000 0090    ++tx_counter0;
_0x17:
	INC  R9
; 0000 0091    }
; 0000 0092 else
	RJMP _0x18
_0x14:
; 0000 0093    UDR0=c;
	LD   R30,Y
	STS  198,R30
; 0000 0094 #asm("sei")
_0x18:
	sei
; 0000 0095 }
	RJMP _0x2060001
; .FEND
;#pragma used-
;#endif
;
;// Standard Input/Output functions
;#include <stdio.h>
;//---------------------------------------------
;
;
;//--7 SEGMENT LED--
;#define LEDa PORTD.6  // Segment a -> PORTD pin 6
;#define LEDb PORTB.0  // Segment b -> PORTB pin 0
;#define LEDc PORTB.1  // Segment c -> PORTB pin 1
;#define LEDd PORTD.5  // Segment d -> PORTD pin 5
;#define LEDe PORTB.2  // Segment e -> PORTB pin 2
;#define LEDf PORTB.3  // Segment f -> PORTB pin 3
;#define LEDg PORTB.4  // Segment g -> PORTB pin 4
;
;// Segment patterns for digits 0..9 (order: a,b,c,d,e,f,g)
;const unsigned char digit[10][7] = {
;    {1,1,1,1,1,1,0}, // 0
;    {0,1,1,0,0,0,0}, // 1
;    {1,1,0,1,1,0,1}, // 2
;    {1,1,1,1,0,0,1}, // 3
;    {0,1,1,0,0,1,1}, // 4
;    {1,0,1,1,0,1,1}, // 5
;    {1,0,1,1,1,1,1}, // 6
;    {1,1,1,0,0,0,0}, // 7
;    {1,1,1,1,1,1,1}, // 8
;    {1,1,1,1,0,1,1}, // 9
;};
;//------------------------------------------------------------------------------------------
;unsigned char nutSW1, nutSW2, nutSW1truoc, nutSW2truoc;  // Variables to hold the state of the switches
;unsigned char demNut1; // Variable to hold the count of button presses for switch 1 and switch 2
;unsigned char nutSW3, nutSW3truoc; // Switch 3 (PORTC.0) state + previous state
;unsigned char trangThai = 0;           // 0 = system OFF, 1 = system ON
;unsigned char nhietdo = 0; // Variable to hold the temperature value received from UART
;unsigned int kqADC6, kqADC7; // Variables to hold the ADC conversion results for channels 6 and 7
;//------------------------------------------------------------------------------------------
;
;// Function to display a digit on the 7-segment LED
;void Display7SEGMENT(unsigned char num)
; 0000 00BF {
_Display7SEGMENT:
; .FSTART _Display7SEGMENT
; 0000 00C0 //     LEDa = digit[num][0];
; 0000 00C1 //     LEDb = digit[num][1];
; 0000 00C2 //     LEDc = digit[num][2];
; 0000 00C3 //     LEDd = digit[num][3];
; 0000 00C4 //     LEDe = digit[num][4];
; 0000 00C5 //     LEDf = digit[num][5];
; 0000 00C6 //     LEDg = digit[num][6];
; 0000 00C7     if (num == 0 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 1; LEDf = 1; LEDg = 0; }
	ST   -Y,R26
;	num -> Y+0
	LD   R30,Y
	CPI  R30,0
	BRNE _0x19
	CALL SUBOPT_0x1
	SBI  0x5,2
	SBI  0x5,3
	CBI  0x5,4
; 0000 00C8     if (num == 1 ) { LEDa = 0; LEDb = 1; LEDc = 1; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0; }
_0x19:
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x28
	CBI  0xB,6
	CALL SUBOPT_0x2
	CBI  0x5,3
	CBI  0x5,4
; 0000 00C9     if (num == 2 ) { LEDa = 1; LEDb = 1; LEDc = 0; LEDd = 1; LEDe = 1; LEDf = 0; LEDg = 1; }
_0x28:
	LD   R26,Y
	CPI  R26,LOW(0x2)
	BRNE _0x37
	SBI  0xB,6
	SBI  0x5,0
	CBI  0x5,1
	SBI  0xB,5
	SBI  0x5,2
	CBI  0x5,3
	SBI  0x5,4
; 0000 00CA     if (num == 3 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 0; LEDf = 0; LEDg = 1; }
_0x37:
	LD   R26,Y
	CPI  R26,LOW(0x3)
	BRNE _0x46
	CALL SUBOPT_0x1
	CBI  0x5,2
	CBI  0x5,3
	SBI  0x5,4
; 0000 00CB     if (num == 4 ) { LEDa = 0; LEDb = 1; LEDc = 1; LEDd = 0; LEDe = 0; LEDf = 1; LEDg = 1; }
_0x46:
	LD   R26,Y
	CPI  R26,LOW(0x4)
	BRNE _0x55
	CBI  0xB,6
	CALL SUBOPT_0x2
	SBI  0x5,3
	SBI  0x5,4
; 0000 00CC     if (num == 5 ) { LEDa = 1; LEDb = 0; LEDc = 1; LEDd = 1; LEDe = 0; LEDf = 1; LEDg = 1; }
_0x55:
	LD   R26,Y
	CPI  R26,LOW(0x5)
	BRNE _0x64
	SBI  0xB,6
	CBI  0x5,0
	SBI  0x5,1
	SBI  0xB,5
	CBI  0x5,2
	SBI  0x5,3
	SBI  0x5,4
; 0000 00CD     if (num == 6 ) { LEDa = 1; LEDb = 0; LEDc = 1; LEDd = 1; LEDe = 1; LEDf = 1; LEDg = 1; }
_0x64:
	LD   R26,Y
	CPI  R26,LOW(0x6)
	BRNE _0x73
	SBI  0xB,6
	CBI  0x5,0
	SBI  0x5,1
	SBI  0xB,5
	SBI  0x5,2
	SBI  0x5,3
	SBI  0x5,4
; 0000 00CE     if (num == 7 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0; }
_0x73:
	LD   R26,Y
	CPI  R26,LOW(0x7)
	BRNE _0x82
	SBI  0xB,6
	CALL SUBOPT_0x2
	CBI  0x5,3
	CBI  0x5,4
; 0000 00CF     if (num == 8 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 1; LEDf = 1; LEDg = 1; }
_0x82:
	LD   R26,Y
	CPI  R26,LOW(0x8)
	BRNE _0x91
	CALL SUBOPT_0x1
	SBI  0x5,2
	SBI  0x5,3
	SBI  0x5,4
; 0000 00D0     if (num == 9 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 0; LEDf = 1; LEDg = 1; }
_0x91:
	LD   R26,Y
	CPI  R26,LOW(0x9)
	BRNE _0xA0
	CALL SUBOPT_0x1
	CBI  0x5,2
	SBI  0x5,3
	SBI  0x5,4
; 0000 00D1 
; 0000 00D2     if (num == 16) { LEDa = 0; LEDb = 0; LEDc = 0; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0; }
_0xA0:
	LD   R26,Y
	CPI  R26,LOW(0x10)
	BRNE _0xAF
	CBI  0xB,6
	CBI  0x5,0
	CBI  0x5,1
	CBI  0xB,5
	CBI  0x5,2
	CBI  0x5,3
	CBI  0x5,4
; 0000 00D3     if (num == 20) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 1; LEDf = 1; LEDg = 1; }
_0xAF:
	LD   R26,Y
	CPI  R26,LOW(0x14)
	BRNE _0xBE
	CALL SUBOPT_0x1
	SBI  0x5,2
	SBI  0x5,3
	SBI  0x5,4
; 0000 00D4 }
_0xBE:
_0x2060001:
	ADIW R28,1
	RET
; .FEND
;
;//  Turn off all segments of the 7-segment LED
;// void turn_off_segments()
;// {
;//       LEDa = 0; LEDb = 0; LEDc = 0; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0;
;// }
;
;// Function to send the temperature value over UART
;void guinhietdo(unsigned char nhietdo) {
; 0000 00DD void guinhietdo(unsigned char nhietdo) {
; 0000 00DE     putchar(nhietdo/10 + 48); // Send the tens digit as a character over UART
;	nhietdo -> Y+0
; 0000 00DF     putchar(nhietdo%10 + 48); // Send the ones digit as a character over UART
; 0000 00E0     putchar(186); // Degree symbol
; 0000 00E1     putchar('C'); // Celsius symbol
; 0000 00E2 }
;
;// Function to send an unsigned char value over UART
;void gui_unchar(unsigned char so) {
; 0000 00E5 void gui_unchar(unsigned char so) {
; 0000 00E6     putchar(so/100 + 48); // Send the hundreds digit as a character over UART
;	so -> Y+0
; 0000 00E7     putchar((so/10)%10 + 48); // Send the tens digit as a character over UART
; 0000 00E8     putchar(so%10 + 48); // Send the ones digit as a character over UART
; 0000 00E9 
; 0000 00EA }
;
;// Function to send an unsigned int value over UART
;void gui_unint(unsigned int so) {
; 0000 00ED void gui_unint(unsigned int so) {
_gui_unint:
; .FSTART _gui_unint
; 0000 00EE     putchar(so/10000 + 48); // Send the ten-thousands digit as a character over UART
	ST   -Y,R27
	ST   -Y,R26
;	so -> Y+0
	LD   R26,Y
	LDD  R27,Y+1
	LDI  R30,LOW(10000)
	LDI  R31,HIGH(10000)
	CALL __DIVW21U
	CALL SUBOPT_0x3
; 0000 00EF     putchar((so/1000)%10 + 48); // Send the thousands digit as a character over UART
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	CALL SUBOPT_0x4
; 0000 00F0     putchar((so/100)%10 + 48); // Send the hundreds digit as a character over UART
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	CALL SUBOPT_0x4
; 0000 00F1     putchar((so/10)%10 + 48); // Send the tens digit as a character over UART
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL SUBOPT_0x4
; 0000 00F2     putchar(so%10 + 48); // Send the ones digit as a character over UART
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MODW21U
	SUBI R30,-LOW(48)
	MOV  R26,R30
	RCALL _putchar
; 0000 00F3 }
	ADIW R28,2
	RET
; .FEND
;
;void main(void)
; 0000 00F6 {
_main:
; .FSTART _main
; 0000 00F7     // Crystal Oscillator division factor: 1
; 0000 00F8     #pragma optsize-
; 0000 00F9     CLKPR = (1<<CLKPCE);
	LDI  R30,LOW(128)
	STS  97,R30
; 0000 00FA     CLKPR = (0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
	LDI  R30,LOW(0)
	STS  97,R30
; 0000 00FB     #ifdef _OPTIMIZE_SIZE_
; 0000 00FC     #pragma optsize+
; 0000 00FD     #endif
; 0000 00FE 
; 0000 00FF     // USART initialization
; 0000 0100     // Communication Parameters: 8 Data, 1 Stop, No Parity
; 0000 0101     // USART Receiver: On
; 0000 0102     // USART Transmitter: On
; 0000 0103     // USART0 Mode: Asynchronous
; 0000 0104     // USART Baud Rate: 9600
; 0000 0105     UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
	STS  192,R30
; 0000 0106     UCSR0B=(1<<RXCIE0) | (1<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
	LDI  R30,LOW(216)
	STS  193,R30
; 0000 0107     UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
	LDI  R30,LOW(6)
	STS  194,R30
; 0000 0108     UBRR0H=0x00;
	LDI  R30,LOW(0)
	STS  197,R30
; 0000 0109     UBRR0L=0x67;
	LDI  R30,LOW(103)
	STS  196,R30
; 0000 010A 
; 0000 010B     // ADC initialization
; 0000 010C     // ADC Clock frequency: 1000,000 kHz
; 0000 010D     // ADC Voltage Reference: AVCC pin
; 0000 010E     // ADC Auto Trigger Source: ADC Stopped
; 0000 010F     // Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
; 0000 0110     // ADC4: On, ADC5: On
; 0000 0111     DIDR0=(0<<ADC5D) | (0<<ADC4D) | (0<<ADC3D) | (0<<ADC2D) | (0<<ADC1D) | (0<<ADC0D);
	LDI  R30,LOW(0)
	STS  126,R30
; 0000 0112     ADMUX=ADC_VREF_TYPE;
	LDI  R30,LOW(64)
	STS  124,R30
; 0000 0113     ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);
	LDI  R30,LOW(132)
	STS  122,R30
; 0000 0114     ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
	LDI  R30,LOW(0)
	STS  123,R30
; 0000 0115 
; 0000 0116     // External Interrupt(s) initialization
; 0000 0117     // INT0: Off
; 0000 0118     // INT1: On
; 0000 0119     // INT1 Mode: Falling Edge
; 0000 011A     // Interrupt on any change on pins PCINT0-7: Off
; 0000 011B     // Interrupt on any change on pins PCINT8-14: Off
; 0000 011C     // Interrupt on any change on pins PCINT16-23: Off
; 0000 011D     EICRA=(1<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
	LDI  R30,LOW(8)
	STS  105,R30
; 0000 011E     EIMSK=(1<<INT1) | (0<<INT0);
	LDI  R30,LOW(2)
	OUT  0x1D,R30
; 0000 011F     EIFR=(1<<INTF1) | (0<<INTF0);
	OUT  0x1C,R30
; 0000 0120     PCICR=(0<<PCIE2) | (0<<PCIE1) | (0<<PCIE0);
	LDI  R30,LOW(0)
	STS  104,R30
; 0000 0121 
; 0000 0122 
; 0000 0123 
; 0000 0124     DDRD.6 = 1; // Set PORTD pin 6 as output (Segment a)
	SBI  0xA,6
; 0000 0125     DDRB.0 = 1; // Set PORTB pin 0 as output (Segment b)
	SBI  0x4,0
; 0000 0126     DDRB.1 = 1; // Set PORTB pin 1 as output (Segment c)
	SBI  0x4,1
; 0000 0127     DDRD.5 = 1; // Set PORTD pin 5 as output (Segment d)
	SBI  0xA,5
; 0000 0128     DDRB.2 = 1; // Set PORTB pin 2 as output (Segment e)
	SBI  0x4,2
; 0000 0129     DDRB.3 = 1; // Set PORTB pin 3 as output (Segment f)
	SBI  0x4,3
; 0000 012A     DDRB.4 = 1; // Set PORTB pin 4 as output (Segment g)
	SBI  0x4,4
; 0000 012B 
; 0000 012C     DDRD.4 = 0; // Set PORTD pin 4 as input (LED ON/OFF control)
	CBI  0xA,4
; 0000 012D     PORTD.4 = 1; // Enable pull-up resistor on PORTD pin 4
	SBI  0xB,4
; 0000 012E     DDRD.7 = 0; // Set PORTD pin 7 as input (LED ON/OFF control)
	CBI  0xA,7
; 0000 012F     PORTD.7 = 1; // Enable pull-up resistor on PORTD pin 7
	SBI  0xB,7
; 0000 0130 
; 0000 0131     DDRC.0 = 0; // Set PORTC pin 0 as input (power button)
	CBI  0x7,0
; 0000 0132     PORTC.0 = 1; // Enable pull-up resistor on PORTC pin 0
	SBI  0x8,0
; 0000 0133 
; 0000 0134     Display7SEGMENT(20); // Turn on all segments
	LDI  R26,LOW(20)
	RCALL _Display7SEGMENT
; 0000 0135     delay_ms(1000);     // Wait for 1000 ms
	LDI  R26,LOW(1000)
	LDI  R27,HIGH(1000)
	CALL _delay_ms
; 0000 0136     Display7SEGMENT(16); // Turn off all segments
	LDI  R26,LOW(16)
	RCALL _Display7SEGMENT
; 0000 0137     delay_ms(500);     // Wait for 500 ms
	LDI  R26,LOW(500)
	LDI  R27,HIGH(500)
	CALL _delay_ms
; 0000 0138 
; 0000 0139     nutSW1 = PIND.4; // Read the state of switch 1 (PORTD pin 4)
	CALL SUBOPT_0x5
; 0000 013A     nutSW2 = PIND.7; // Read the state of switch 2 (PORTD pin 7)
; 0000 013B     nutSW1truoc = nutSW1; // Store the initial state of switch 1
	MOV  R14,R12
; 0000 013C     nutSW2truoc = nutSW2; // Store the initial state of switch 2
	MOV  R13,R11
; 0000 013D     nutSW3 = PINC.0; // Read the initial state of switch 3 (PORTC pin 0)
	LDI  R30,0
	SBIC 0x6,0
	LDI  R30,1
	STS  _nutSW3,R30
; 0000 013E     nutSW3truoc = nutSW3; // Store the initial state of switch 3
	STS  _nutSW3truoc,R30
; 0000 013F 
; 0000 0140     // Global enable interrupts
; 0000 0141     #asm("sei")
	sei
; 0000 0142 
; 0000 0143     putchar('S');
	LDI  R26,LOW(83)
	RCALL _putchar
; 0000 0144     putchar('E');
	LDI  R26,LOW(69)
	RCALL _putchar
; 0000 0145     putchar('T');
	LDI  R26,LOW(84)
	RCALL _putchar
; 0000 0146     putchar('U');
	LDI  R26,LOW(85)
	RCALL _putchar
; 0000 0147     putchar('P');
	LDI  R26,LOW(80)
	CALL SUBOPT_0x6
; 0000 0148     putchar(10);
; 0000 0149     putchar(13);
; 0000 014A 
; 0000 014B 
; 0000 014C     while (1)
_0xE7:
; 0000 014D     {
; 0000 014E         nutSW1truoc = nutSW1; // Store the previous state of switch 1
	MOV  R14,R12
; 0000 014F         nutSW2truoc = nutSW2; // Store the previous state of switch 2
	MOV  R13,R11
; 0000 0150         nutSW1 = PIND.4; // Read the state of switch 1 (PORTD pin 4)
	CALL SUBOPT_0x5
; 0000 0151         nutSW2 = PIND.7; // Read the state of switch 2 (PORTD pin 7)
; 0000 0152 
; 0000 0153         nutSW3truoc = nutSW3; // Store the previous state of switch 3
	LDS  R30,_nutSW3
	STS  _nutSW3truoc,R30
; 0000 0154         nutSW3 = PINC.0; // Read the state of switch 3 (PORTC pin 0)
	LDI  R30,0
	SBIC 0x6,0
	LDI  R30,1
	STS  _nutSW3,R30
; 0000 0155 
; 0000 0156         kqADC6 = read_adc(6); // Read the ADC value from channel 6 (PC6)
	LDI  R26,LOW(6)
	RCALL _read_adc
	STS  _kqADC6,R30
	STS  _kqADC6+1,R31
; 0000 0157         kqADC7 = read_adc(7); // Read the ADC value from channel 7 (PC7)
	LDI  R26,LOW(7)
	RCALL _read_adc
	STS  _kqADC7,R30
	STS  _kqADC7+1,R31
; 0000 0158 
; 0000 0159         if (nutSW3 == 0 && nutSW3truoc == 1) // Falling edge = switch 3 pressed
	LDS  R26,_nutSW3
	CPI  R26,LOW(0x0)
	BRNE _0xEB
	LDS  R26,_nutSW3truoc
	CPI  R26,LOW(0x1)
	BREQ _0xEC
_0xEB:
	RJMP _0xEA
_0xEC:
; 0000 015A         {
; 0000 015B             trangThai = !trangThai;              // Latch ON <-> OFF
	LDS  R30,_trangThai
	CALL __LNEGB1
	STS  _trangThai,R30
; 0000 015C             if (trangThai)
	CPI  R30,0
	BRNE _0xEE
; 0000 015D             {
; 0000 015E                 // Display7SEGMENT(nhietdo % 10); // ON: show the ones digit of the last temperature
; 0000 015F                 // putchar('O');
; 0000 0160                 // putchar('N');
; 0000 0161                 // putchar(10);
; 0000 0162                 // putchar(13);
; 0000 0163                 // putchar(7); // Bell sound
; 0000 0164 
; 0000 0165             }
; 0000 0166             else
; 0000 0167             {
; 0000 0168                 Display7SEGMENT(16); // OFF: blank display
	LDI  R26,LOW(16)
	RCALL _Display7SEGMENT
; 0000 0169                 // putchar('O');
; 0000 016A                 // putchar('F');
; 0000 016B                 // putchar('F');
; 0000 016C                 // putchar(10);
; 0000 016D                 // putchar(13);
; 0000 016E                 // putchar(7); // Bell sound
; 0000 016F             }
_0xEE:
; 0000 0170         }
; 0000 0171 
; 0000 0172         // if (trangThai) // SW1/SW2 only work when the system is ON
; 0000 0173         // {
; 0000 0174         //     if (nutSW2 == 0 && nutSW2truoc == 1) // If PORTD pin 7 is LOW (button pressed) and was HIGH before
; 0000 0175         //     {
; 0000 0176         //         if (demNut1 < 10) // Only act if below 30 degrees C
; 0000 0177         //         {
; 0000 0178         //             demNut1++; // Increase, stops at 10 (30 degrees C)
; 0000 0179         //             nhietdo = demNut1 + 20; // Convert the count to a temperature value
; 0000 017A         //             putchar(nhietdo/10 + 48); // Send the count as a character over UART
; 0000 017B         //             putchar(nhietdo%10 + 48); // Send the count as a character over UART
; 0000 017C         //             putchar(186); // Degree symbol
; 0000 017D         //             putchar('C'); // Celsius symbol
; 0000 017E         //             putchar(10); // New line
; 0000 017F         //             putchar(13); // Carriage return
; 0000 0180         //             Display7SEGMENT(nhietdo % 10); // Display the ones digit of the temperature on the 7-segment LED
; 0000 0181         //         }
; 0000 0182         //     }
; 0000 0183         //     if (nutSW1 == 0 && nutSW1truoc == 1) // If PORTD pin 4 is LOW (button pressed) and was HIGH before
; 0000 0184         //     {
; 0000 0185         //         if (demNut1 > 0) // Only act if above 20 degrees C
; 0000 0186         //         {
; 0000 0187         //             demNut1--; // Decrease, stops at 0 (20 degrees C)
; 0000 0188         //             nhietdo = demNut1 + 20; // Convert the count to a temperature value
; 0000 0189         //             putchar(nhietdo/10 + 48); // Send the count as a character over UART
; 0000 018A         //             putchar(nhietdo%10 + 48); // Send the count as a character over UART
; 0000 018B         //             putchar(186); // Degree symbol
; 0000 018C         //             putchar('C'); // Celsius symbol
; 0000 018D         //             putchar(10); // New line
; 0000 018E         //             putchar(13); // Carriage return
; 0000 018F         //             Display7SEGMENT(nhietdo % 10); // Display the ones digit of the temperature on the 7-segment LED
; 0000 0190         //         }
; 0000 0191         //     }
; 0000 0192         // }
; 0000 0193 
; 0000 0194         gui_unint(kqADC6); // Send the ADC value from channel 6 over UART
_0xEA:
	LDS  R26,_kqADC6
	LDS  R27,_kqADC6+1
	RCALL _gui_unint
; 0000 0195         putchar(' '); // Send a space character
	LDI  R26,LOW(32)
	RCALL _putchar
; 0000 0196         gui_unint(kqADC7); // Send the ADC value from channel 7 over UART
	LDS  R26,_kqADC7
	LDS  R27,_kqADC7+1
	RCALL _gui_unint
; 0000 0197         putchar(' '); // Send a space character
	LDI  R26,LOW(32)
	RCALL _putchar
; 0000 0198         gui_unint(demXung); // Send the count of button presses over UART
	__GETW2R 3,4
	RCALL _gui_unint
; 0000 0199         putchar(' '); // Send a space character
	LDI  R26,LOW(32)
	CALL SUBOPT_0x6
; 0000 019A         putchar(10); // New line
; 0000 019B         putchar(13); // Carriage return
; 0000 019C 
; 0000 019D         // Live display: ones digit of pulse count while ON, blank while OFF
; 0000 019E         if (trangThai)
	LDS  R30,_trangThai
	CPI  R30,0
	BREQ _0xEF
; 0000 019F         {
; 0000 01A0             Display7SEGMENT(demXung % 10);
	__GETW2R 3,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MODW21U
	MOV  R26,R30
	RJMP _0xF2
; 0000 01A1         }
; 0000 01A2         else
_0xEF:
; 0000 01A3         {
; 0000 01A4             Display7SEGMENT(16);
	LDI  R26,LOW(16)
_0xF2:
	RCALL _Display7SEGMENT
; 0000 01A5         }
; 0000 01A6 
; 0000 01A7         delay_ms(200); // Wait for 200 ms
	LDI  R26,LOW(200)
	LDI  R27,0
	CALL _delay_ms
; 0000 01A8     }
	RJMP _0xE7
; 0000 01A9 }
_0xF1:
	RJMP _0xF1
; .FEND
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif

	.CSEG

	.CSEG

	.CSEG

	.DSEG
_rx_buffer0:
	.BYTE 0x40
_tx_buffer0:
	.BYTE 0x40
_nutSW3:
	.BYTE 0x1
_nutSW3truoc:
	.BYTE 0x1
_trangThai:
	.BYTE 0x1
_kqADC6:
	.BYTE 0x2
_kqADC7:
	.BYTE 0x2

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x0:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x1:
	SBI  0xB,6
	SBI  0x5,0
	SBI  0x5,1
	SBI  0xB,5
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2:
	SBI  0x5,0
	SBI  0x5,1
	CBI  0xB,5
	CBI  0x5,2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x3:
	SUBI R30,-LOW(48)
	MOV  R26,R30
	CALL _putchar
	LD   R26,Y
	LDD  R27,Y+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x4:
	CALL __DIVW21U
	MOVW R26,R30
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MODW21U
	RJMP SUBOPT_0x3

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x5:
	LDI  R30,0
	SBIC 0x9,4
	LDI  R30,1
	MOV  R12,R30
	LDI  R30,0
	SBIC 0x9,7
	LDI  R30,1
	MOV  R11,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x6:
	CALL _putchar
	LDI  R26,LOW(10)
	CALL _putchar
	LDI  R26,LOW(13)
	JMP  _putchar


	.CSEG
_delay_ms:
	adiw r26,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0xFA0
	wdr
	sbiw r26,1
	brne __delay_ms0
__delay_ms1:
	ret

__LNEGB1:
	TST  R30
	LDI  R30,1
	BREQ __LNEGB1F
	CLR  R30
__LNEGB1F:
	RET

__DIVW21U:
	CLR  R0
	CLR  R1
	LDI  R25,16
__DIVW21U1:
	LSL  R26
	ROL  R27
	ROL  R0
	ROL  R1
	SUB  R0,R30
	SBC  R1,R31
	BRCC __DIVW21U2
	ADD  R0,R30
	ADC  R1,R31
	RJMP __DIVW21U3
__DIVW21U2:
	SBR  R26,1
__DIVW21U3:
	DEC  R25
	BRNE __DIVW21U1
	MOVW R30,R26
	MOVW R26,R0
	RET

__MODW21U:
	RCALL __DIVW21U
	MOVW R30,R26
	RET

;END OF CODE MARKER
__END_OF_CODE:
