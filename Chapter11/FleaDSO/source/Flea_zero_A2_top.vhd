----------------------------------------------------------------------------------
-- ********* Fleatiny-FPGA Platform top-level module ***********
-- This is basically a wrapper allows connection of user projects to
-- Fleatiny-FPGA's on-board hardware
--
-- Creation Date: 4th August 2013
-- Author: Valentin Angelovski
--
-- ©2013 - Valentin Angelovski
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL; 
use IEEE.numeric_std.ALL;
--use work.zpu_config.all; 
--use work.zpupkg.ALL;
 
entity FleaFPGA_2v4 is

	port(
	-- System clock and reset
	sys_clock		: in		std_logic;	-- main clock input from external clock source
	sys_reset		: in		std_logic;	-- main clock input from external RC reset circuit
	
	vSync			: buffer		std_logic;    
	hSync			: buffer 		std_logic;   

	-- On-board user buttons and status LEDs
	LVDS_Red		: out		std_logic_vector(0 downto 0);	-- main clock input from external clock source
	LVDS_Green		: out		std_logic_vector(0 downto 0);	-- main clock input from external RC reset circuit
	LVDS_Blue		: out		std_logic_vector(0 downto 0);
	LVDS_ck			: out		std_logic_vector(0 downto 0);
	
--	clk200		: out		std_logic;
	
	n_led1		: out		std_logic
	   
	);  
end FleaFPGA_2v4;

architecture arch of FleaFPGA_2v4 is

   signal clk_dvi  : std_logic := '0';
   signal clk_dvin : std_logic := '0';
   signal clk_vga  : std_logic := '0';

   signal clk_pcs   : std_logic := '0';

   signal red     : std_logic_vector(7 downto 0) := (others => '0');
   signal green   : std_logic_vector(7 downto 0) := (others => '0');
   signal blue    : std_logic_vector(7 downto 0) := (others => '0');
  -- signal hsync   : std_logic := '0';
  -- signal vsync   : std_logic := '0';
   signal blank   : std_logic := '0';
   signal red_s   : std_logic_vector(0 downto 0);
   signal green_s : std_logic_vector(0 downto 0);
   signal blue_s  : std_logic_vector(0 downto 0);
   signal clock_s : std_logic_vector(0 downto 0);
	 
   signal counter : unsigned(23 downto 0) := x"000000";	 
	
begin
	-- Housekeeping logic for unwanted peripherals on FleaFPGA board goes here..
	-- (Note: comment out any of the following code lines if peripheral is required)


n_led1 <= '1';
--clk200 <= clk_pcs;
 
	-- User HDL project modules and port mappings go here..

	u0 : entity work.DVI_clkgen
	port map(
		CLKI			=>	sys_clock,
		CLKOP			=>	clk_dvi,
		CLKOS 			=>  clk_dvin,
		CLKOS2 			=>  clk_vga
		); 

	u100 : entity work.dvid PORT MAP(
      clk       => clk_dvi,
      clk_n     => clk_dvin, 
      clk_pixel => clk_vga,
      red_p     => red,
      green_p   => green,
      blue_p    => blue,
      blank     => blank,
      hsync     => hsync,
      vsync     => vsync, 
      -- outputs to TMDS drivers
      red_s     => LVDS_Red,
      green_s   => LVDS_Green,
      blue_s    => LVDS_Blue, 
      clock_s   => LVDS_ck
   ); 
   

         
--	dram_addr(13) <= '0'; -- Only testing within 64MByte range 
 
 -- Serdes Transmitter PLL clock
 
	u101 : entity work.vga GENERIC MAP (
	
	--  148.50	6.73	1920	88	44	148	2200	+	1080	4	5	36	1125 +
	
	--  74.25	13.47	1280	72	80	216	1648	+	720		3	5	22	750	+
	
	  -- 1280x720 @ 60.7Hz, 75MHz pixel clock
      --hRez       => 1280,	
      --hStartSync       => 1352,
      --hEndSync       => 1432,
      --hMaxCount       => 1647,
      --hsyncActive       => '1',
		
      --vRez       => 720,
      --vStartSync       => 723,
      --vEndSync       => 728,
      --vMaxCount       => 750,
      --vsyncActive       => '1'
	  -- 1980x1080 @ 30.35Hz, 75MHz pixel clock
      --hRez       => 1920,	
     -- hStartSync       => 2008,
     -- hEndSync       => 2052,
     -- hMaxCount       => 2199,
     -- hsyncActive       => '1',
		
     -- vRez       => 1080,
     -- vStartSync       => 1084,
     -- vEndSync       => 1089,
     -- vMaxCount       => 1125,
     -- vsyncActive       => '1'
	 
	  -- 640x480 @60Hz, 25MHz pixel clock
      hRez       => 640,
      hStartSync       => 656,
      hEndSync       => 752,
      hMaxCount       => 800,
      hsyncActive       => '0',
		
      vRez       => 480,
      vStartSync       => 490,
      vEndSync       => 492,
      vMaxCount       => 525,
      vsyncActive       => '1'

	  -- 720x576 @50Hz, 27MHz pixel clock
      --hRez       => 720,	
      --hStartSync       => 732,
      --hEndSync       => 796,
      --hMaxCount       => 864,
      --hsyncActive       => '0',
		
      --vRez       => 576,
      --vStartSync       => 581,
      --vEndSync       => 586,
      --vMaxCount       => 625,
      --vsyncActive       => '0'	
	  
	  -- 1980x1080 @ 30.35Hz, 75MHz pixel clock
	  
      --1024x768x60		65.00	20.67	1024	24	136	160	1344	-	768	3	6	29	806	-	  
	  -- 1024x768 @ 60Hz, 65MHz pixel clock
      --hRez       	=> 1024,	
      --hStartSync       => 1048,
      --hEndSync       => 1184,
      --hMaxCount       => 1343,
      --hsyncActive       => '0',
		
      --vRez       => 768,
      --vStartSync       => 771,
      --vEndSync       => 777,
      --vMaxCount       => 806,
      --vsyncActive       => '0'
	  
   ) PORT MAP(
      pixelClock => clk_vga,
      Red        => red,
      Green      => green,
      Blue       => blue,
      hSync      => hSync,
      vSync      => vSync,
      blank      => blank
   );


 


end architecture;


