--------------------------------------------------------------------
-- ********* "FleaFPGA Ohm" Platform top-level module ***********
-- This file basically serves as a wrapper for connecting any VHDL 
-- signals to be broken out of the ECP5 FPGA with the rest of 
-- FleaFPGA Ohm's on-board hardware
-- 
-- Creation Date: 13th January 2018
-- PCB Revision: A5
-- Author: Valentin Angelovski
--
--------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_unsigned.ALL;
use IEEE.numeric_std.all;


entity FleaFPGA_Ohm_A5 is
 
	port(
	-- System clock and reset
	sys_clock		: in		std_logic;	-- 25MHz clock input from external xtal oscillator.
	sys_reset		: in		std_logic;	-- master reset input from reset header.

	-- On-board user buttons and status LED
	n_led1			: buffer	std_logic;
 
	-- Digital video out
	LVDS_Red		: out		std_logic_vector(0 downto 0);	-- 
	LVDS_Green		: out		std_logic_vector(0 downto 0);	-- 
	LVDS_Blue		: out		std_logic_vector(0 downto 0);	-- 
	LVDS_ck			: out		std_logic_vector(0 downto 0);	-- 
	
	-- USB Slave (FT230x) debug interface 
	slave_tx_o 		: out		std_logic;
	slave_rx_i 		: in		std_logic;
	slave_cts_i 	: in		std_logic;	-- Receives signal from #RTS pin on FT230x, where applicable.

	-- SDRAM interface (For use with 16Mx16bit or 32Mx16bit SDR DRAM, depending on version)
	Dram_Clk		: out		std_logic;	-- clock to SDRAM
	Dram_CKE		: out		std_logic;	-- clock to SDRAM
	Dram_n_Ras		: out		std_logic;	-- SDRAM RAS
	Dram_n_Cas		: out		std_logic;	-- SDRAM CAS
	Dram_n_We		: out		std_logic;	-- SDRAM write-enable
	Dram_BA			: out		std_logic_vector(1 downto 0);	-- SDRAM bank-address
	Dram_Addr		: out		std_logic_vector(12 downto 0);	-- SDRAM address bus
	Dram_Data		: inout		std_logic_vector(15 downto 0);	-- data bus to/from SDRAM
	Dram_n_cs		: out		std_logic;
	--Dram_dqm		: out		std_logic_vector(1 downto 0);
	Dram_DQMH		: out		std_logic;
	Dram_DQML		: out		std_logic;

    -- GPIO Header pins declaration (RasPi compatible GPIO format)
	GPIO_2			: inout		std_logic;
	GPIO_3			: inout		std_logic;
	GPIO_4			: inout		std_logic;
	GPIO_5			: inout		std_logic;
	GPIO_6			: inout		std_logic;	
	GPIO_7			: inout		std_logic;	
	GPIO_8			: inout		std_logic;	
	GPIO_9			: inout		std_logic;	
	GPIO_10			: inout		std_logic;
	GPIO_11			: inout		std_logic;	
	GPIO_12			: inout		std_logic;	
	GPIO_13			: inout		std_logic;	
	GPIO_14			: inout		std_logic;	
	GPIO_15			: inout		std_logic;	
	GPIO_16			: inout		std_logic;	
	GPIO_17			: inout		std_logic;
	GPIO_18			: inout		std_logic;	
	GPIO_19			: inout		std_logic;	
	GPIO_20			: inout		std_logic;
	GPIO_21			: inout		std_logic;	
	GPIO_22			: inout		std_logic;	
	GPIO_23			: inout		std_logic;
	GPIO_24			: inout		std_logic;
	GPIO_25			: inout		std_logic;	
	GPIO_26			: inout		std_logic;	
	GPIO_27			: inout		std_logic;
	GPIO_IDSD		: inout		std_logic;
	GPIO_IDSC		: inout		std_logic;

	-- Sigma Delta ADC ('Enhanced' Ohm-specific GPIO functionality)	
	-- NOTE: Must comment out GPIO_5, GPIO_7, GPIO_10 AND GPIO_24 as instructed in the pin constraints file (.LPF) in order to use
	--ADC0_input	: in		std_logic;
	--ADC0_error	: buffer	std_logic;
	--ADC1_input	: in		std_logic;
	--ADC1_error	: buffer	std_logic;
	--ADC2_input	: in		std_logic;
	--ADC2_error	: buffer	std_logic;
	--ADC3_input	: in		std_logic;
	--ADC3_error	: buffer	std_logic;

	-- SD/MMC Interface (Support either SPI or nibble-mode)
	mmc_dat1		: in		std_logic;
	mmc_dat2		: in		std_logic;
	mmc_n_cs		: out		std_logic;
	mmc_clk			: out		std_logic;
	mmc_mosi		: out		std_logic; 
	mmc_miso		: in		std_logic;

	-- PS/2 Mode enable, keyboard and Mouse interfaces
	PS2_enable		: out		std_logic;
	PS2_clk1		: inout		std_logic;
	PS2_data1		: inout		std_logic;
	
	PS2_clk2		: inout		std_logic;
	PS2_data2		: inout		std_logic
	);
end FleaFPGA_Ohm_A5;
  
architecture arch of FleaFPGA_Ohm_A5 is

 -- module-level signal and/or module declarations go here: 
 signal   internal_wire:      std_logic;
 
begin

 
 -- Housekeeping logic for unwanted peripherals on FleaFPGA Ohm board goes here..
 -- (Note: comment out any of the following code lines if peripheral is required)

 n_led1 <= internal_wire;

 Dram_CKE <= '0';  	-- DRAM Clock disable.
 Dram_n_cs <= '1'; 	-- DRAM Chip disable.
 PS2_enable <= '1'; -- Configures both USB host ports for legacy PS/2 mode.
 mmc_n_cs <= '1'; 	-- Micro SD card chip disable.
 

 
 -- User HDL component/module entitites go here (per example, 'blinky'(.vhd) module):

 user_module1 : entity work.blinky

    port map(
		clk => sys_clock,
		blink_LED => internal_wire
    ); 
	
	
 -- module-level sequential and/or combinatorial logic goes here (if any):


end arch;
