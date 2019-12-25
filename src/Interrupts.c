//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB3_Register_Enums.h>

// SI_SBIT is a macro that helps with setting up access to pin input/output.
// The first one is the name of your "variable", the second which port it's on, and the third which bit in that port.
// For example, the first push button (PB0), is located on I/O port 0, and is bit 2 on that port, hence SFR_P0 and 2.
SI_SBIT(PB0, SFR_P0, 2);
SI_SBIT(PB1, SFR_P0, 3);

// The "OUT_" here is only because its easier for me to see the difference between inputs and outputs, it's not necessary.
SI_SBIT(OUT_LED_G, SFR_P1, 6);
SI_SBIT(OUT_LED_B, SFR_P1, 7);
SI_SBIT(OUT_LED_R, SFR_P2, 0);

volatile uint8_t ledIntensity = 0; // variables accessed from inside interrupts must be volatile, otherwise the compiler MIGHT do optimize them away, your code will work in debug builds but not otherwise.

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN0::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN0::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER2_ISR, TIMER2_IRQn)
{
	SFRPAGE = 0x0; // TIMER2 control 0 register and PCA channel 0 registers are both located on SFPRAGE 0x0 (and 0x10).
	TMR2CN0_TF2H = 0; // Reset the timer 2 interrupt, if not, the timer interrupt will trigger again and again

	if(PB1 == 0) // PB1 == 0 means that PB1 is pressed down.
	{
		if(ledIntensity < 255) // Make sure uint8 does not go over 255, since it would wrap around back to 0
			ledIntensity++;
	}
	else
	{
		if(ledIntensity > 0)
			ledIntensity--;
	}

	// PCA0CPH0 is located on SFRPAGE 0x0 which we are already on, so we don't need to type "SFRPAGE = 0x0;" again.
	PCA0CPH0 = (ledIntensity << PCA0CPH0_PCA0CPH0__SHIFT); // Set duty cycle to ledIntensity. With current configuration the LED will be brighter with higher values in ledIntensity.

	OUT_LED_G = PB0; // Have the green LED be on while you hold PB0.
}

