/*
 * File:   main.c
 * Author: Advanced Digital Systems Laboratory
 *
 * Created on August 5, 2020, 10:48 AM
 */

// <editor-fold defaultstate="collapsed" desc="Configuration bits">

// CONFIG1
// External Oscillator Selection (Oscillator not enabled)
#pragma config FEXTOSC = OFF    

// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)
#pragma config RSTOSC = HFINTOSC_64MHZ

// CONFIG2
// Clock out Enable bit (CLKOUT function is disabled)
#pragma config CLKOUTEN = OFF
// PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be cleared and
// set only once)
#pragma config PR1WAY = ON
// Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed
// by user software)
#pragma config CSWEN = OFF
// Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config FCMEN = OFF

// CONFIG3
// MCLR Enable bit
// (If LVP = 0, MCLR pin is MCLR;
// If LVP = 1, RE3 pin function is MCLR)
#pragma config MCLRE = EXTMCLR
// Power-up timer selection bits (PWRT is disabled)
#pragma config PWRTS = PWRT_OFF
// Multi-vector enable bit (Multi-vector enabled,
// Vector table used for interrupts)
#pragma config MVECEN = ON
// IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared
// and set only once)
#pragma config IVT1WAY = ON
// Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config LPBOREN = OFF
// Brown-out Reset Enable bits (Brown-out Reset disabled)
#pragma config BOREN = OFF

// CONFIG4
// Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR)
// set to 1.9V)
#pragma config BORV = VBOR_1P9
// ZCD Disable bit (ZCD module is disabled.
// ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config ZCD = OFF
// PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be cleared
// and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config PPS1WAY = ON
// Stack Full/Underflow Reset Enable bit (Stack full/underflow
// will cause Reset)
#pragma config STVREN = ON
// Low Voltage Programming Enable bit (HV on MCLR/VPP must be used
// for programming)
#pragma config LVP = OFF
// Extended Instruction Set Enable bit (Extended Instruction Set and
// Indexed Addressing Mode disabled)
#pragma config XINST = OFF

// CONFIG5
// WDT Period selection bits (Divider ratio 1:65536;
// software control of WDTPS)
#pragma config WDTCPS = WDTCPS_31
// WDT operating mode (WDT Disabled; SWDTEN is ignored)
#pragma config WDTE = OFF

// CONFIG6
// WDT Window Select bits (window always open (100%);
// software control; keyed access not required)
#pragma config WDTCWS = WDTCWS_7
// WDT input clock selector (Software Control)
#pragma config WDTCCS = SC

// CONFIG7
// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBSIZE = BBSIZE_512
// Boot Block enable bit (Boot block disabled)
#pragma config BBEN = OFF
// Storage Area Flash enable bit (SAF disabled)
#pragma config SAFEN = OFF
// Background Debugger (Background Debugger disabled)
#pragma config DEBUG = OFF

// CONFIG8
// Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTB = OFF
// Configuration Register Write Protection bit (Configuration registers
// not Write protected)
#pragma config WRTC = OFF
// Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTD = OFF
// SAF Write protection bit (SAF not Write Protected)
#pragma config WRTSAF = OFF
// Application Block write protection bit (Application Block not write
// protected)
#pragma config WRTAPP = OFF

// CONFIG10
// PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code
// protection disabled)
#pragma config CP = OFF

// </editor-fold>

#include <stdint.h>
#include <xc.h>

// Καθορισμός διεύθυνσης πίνακα ανυσμάτων διακοπών
#define IVT                     0x00008

// Δήλωση γενικής μεταβλητής a
int8_t a = 20;

void main(void)
{
    asm("nop");
    
    // Ρύθμιση θύρας PORTD ως ψηφιακή έξοδος και καθορισμός στο 0
    ANSELD = 0;
    TRISD = 0;
    LATD = 0;
    
    // Ρύθμιση ακροδεκτών RB0, RB1 και RB2 ως ψηφιακές είσοδοι
    // με ενεργοποιημένες τις pull-up αντιστάσεις.
    ANSELB = ANSELB & 0b11000000;
    TRISB = TRISB | 0b00111111;
    WPUB = WPUB | 0b00111111;
    
    // Ρύθμιση ακροδεκτών RB0, RB1 και RB2 να ανιχνεύουν κατερχόμενα μέτωπα
    IOCBF = IOCBF & 0b11111000;
    IOCBN = IOCBN | 0b00000111;
    // Καθορισμός χαμηλής προτεραιτότητας για το IOC
    IPR0bits.IOCIP = 0;
    PIE0bits.IOCIE = 1;
    
    // Ρύθμιση ακροδεκτών RB3, RB4 και RB5 για να ανίχνευση των
    // εξωτερικών διακοπών INT0, INT1 και INT2.
    INT0PPS = 0b00001011;
    INT1PPS = 0b00001100;
    INT2PPS = 0b00001101;
    
    // Ρύθμιση εξωτερικών διακοπών INT0, INT1 και INT2
    // να σηματοδοτούνται στο κατερχόμενο μέτωπο.
    INTCON0bits.INT0EDG = 0;
    INTCON0bits.INT1EDG = 0;
    INTCON0bits.INT2EDG = 0;
    
    // Ενεργοποίηση εξωτερικών διακοπών και καθορισμός
    // υψηλής προτεραιτότητας
    IPR1bits.INT0IP = 1;
    PIR1bits.INT0IF = 0;
    PIE1bits.INT0IE = 1;
    IPR6bits.INT1IP = 1;
    PIR6bits.INT1IF = 0;
    PIE6bits.INT1IE = 1;
    IPR10bits.INT2IP = 0;
    PIR10bits.INT2IF = 0;
    PIE10bits.INT2IE = 1;
    
    // Γενική ενεργοποίηση διακοπών
    INTCON0bits.IPEN = 1;
    INTCON0bits.GIEL = 1;
    INTCON0bits.GIEH = 1;
    
    while(1) asm("nop");
    return;
}

// Ρουτίνα διαχείρισης εξωτερικής διακοπής INT0
void __interrupt(irq(IRQ_INT0), base(IVT), high_priority) INT0_ISR()
{
    asm("nop");
    PIR1bits.INT0IF = 0;
    a = a + 12;
    LATD = a;
}

// Ρουτίνα διαχείρισης εξωτερικής διακοπής INT1
void __interrupt(irq(IRQ_INT1), base(IVT), high_priority) INT1_ISR()
{
    asm("nop");
    PIR6bits.INT1IF = 0;
    a = a * 5;
    LATD = a;
}

// Ρουτίνα διαχείρισης εξωτερικής διακοπής INT2
void __interrupt(irq(IRQ_INT2), base(IVT), low_priority) INT2_ISR()
{
    asm("nop");
    PIR10bits.INT2IF = 0;
    a = a % 10;
    LATD = a;
}

// Ρουτίνα διαχείρισης εξωτερικής διακοπής IOC
void __interrupt(irq(IRQ_IOC), base(IVT), low_priority) IOC_ISR()
{
    asm("nop");
    if (IOCBFbits.IOCBF0 == 1)
    {
        IOCBFbits.IOCBF0 = 0;
        a = a * 3;
    }
    else if (IOCBFbits.IOCBF1 == 1)
    {
        IOCBFbits.IOCBF1 = 0;
        a = a >> 2;
    }
    else if (IOCBFbits.IOCBF2 == 1)
    {
        IOCBFbits.IOCBF2 = 0;
        a = a << 1;
    }
    LATD = a;
}

// Ρουτίνα διαχείρισης οποιασφήποτε άλλης διακοπής
void __interrupt(irq(default), base(IVT), high_priority) DEFAULT_ISR()
{
    asm("nop");
}
