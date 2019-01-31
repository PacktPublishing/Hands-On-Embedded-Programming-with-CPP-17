-- ################ FleaFPGA Digital Oscilloscope Example ################
-- #######################################################################
-- Module Name: 		FleaFPGA_DSO.vhd
-- Module Version: 		1.0 
-- Module author: 		Valentin Angelovski
-- Module date: 		17/02/2018
-- Project IDE: 		Lattice Diamond ver. 3.9
--
-- Disclamer:
-- 	This module was created specifically for use with the FleaFPGA Ohm platform 
-- 	and provided AS-IS with no warranties implied. 
-- 	Although this module has been tested with the FleaFPGA Ohm platform (PCB rev 
-- 	A5)and is known to work, Valentin Angelovski accepts NO RESPONSIBILITY 
-- 	for any consequences arising from it's use.
-- 
-- Happy Experimenting! :-)
--
-- Version release history
-- 17/02/2018

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;


entity FleaFPGA_DSO is

port(
	clk: in  STD_LOGIC;	-- FleaFPGA 50MHz master clock
	rst: in  STD_LOGIC; -- FleaFPGA system reset
			
	Sampler_Q: BUFFER  STD_LOGIC; 	-- Sigma Delta ADC - Comparator result
	Sampler_D: in   STD_LOGIC; 		-- Sigma Delta ADC - RC circuit driver out

	ADC_1: BUFFER  STD_LOGIC; -- Gray code output of selected user sample rate
	ADC_2: BUFFER  STD_LOGIC; -- to user LEDs on FleaFPGA
	ADC_3: BUFFER  STD_LOGIC; 
	ADC_4: BUFFER  STD_LOGIC;

	samplerate_adj: in  STD_LOGIC; 	-- DSO timebase select (PB#1 on FleaFPGA)
	
	trigger_adj: in  STD_LOGIC; 	-- Trigger level adjust (PB#2 on FleaFPGA)
	ADC_lowspeed_raw: out  std_logic_vector(7 downto 0);
	Green_out: out   std_logic_vector(3 downto 0); -- VGA output signals
	Red_out: out   std_logic_vector(3 downto 0);   -- (Green = Signal trace)
	Blue_out: out   std_logic_vector(3 downto 0);  -- (White = Measurement Grid)
	VGA_HS: out STD_LOGIC;						   -- (Red = Trigger Level)
	VGA_VS: out STD_LOGIC;
	blank: out STD_LOGIC
	);

END FleaFPGA_DSO;  
  
  
ARCHITECTURE behavior OF FleaFPGA_DSO IS

-- Declare registers and signals needed for the sigma-delta ADC (Analog-to-Digital Converter)
  signal ADC_bufout: std_logic_vector (8 downto 0);
  signal sampler_raw: signed (13 downto 0); 
  signal adc_rawout: signed (13 downto 0);
  signal adc_currentraw: signed (13 downto 0);
  signal ADC_clk : std_logic;  
  signal ADC_filter_shift : std_logic:='1';  
  
-- Declare Oscilloscope signal trigger level threshold registers and signals
  signal trig_row : unsigned(8 downto 0);
  signal trig_upper : unsigned(8 downto 0);
  signal trig_lower : unsigned(8 downto 0);
  signal trigger_flag : std_logic;  

-- Declare Oscilloscope timebase related registers and signals
  signal sample_rate : unsigned(13 downto 0):="10000000000000";
  signal ADC_sample_divider: unsigned(13 downto 0);
  signal sample_LED : unsigned(3 downto 0):="0000"; 
  signal sample_buttons : std_logic;  
  signal sample_button_count : unsigned(3 downto 0):="0000";

-- Declare signals needed for trace buffer, VGA Display objects etc
  signal ADC_buffer_addr: unsigned (9 downto 0);
  signal set_pixel1 : std_logic;
  signal set_pixel2 : std_logic;  
  signal VGA_25MHz : std_logic;
  signal vga_row : std_logic_vector(9 downto 0);
  signal vga_column : std_logic_vector(9 downto 0);
  signal vga_disp_en : std_logic; 
  signal gridrow: unsigned(8 downto 0);
  signal grid_column: unsigned(9 downto 0); 
  signal trig_pixel_count : unsigned(2 downto 0); 

-- Setup our VGA pixel Y-Position plot/scale registers
  signal vga_bufout: std_logic_vector (8 downto 0);  
  signal vga_bufout_scaled: unsigned (17 downto 0);
  signal vga_trigger_scaled: unsigned (17 downto 0);  
  signal addpixel1: unsigned (8 downto 0);
  signal addpixel2: unsigned (8 downto 0);
  signal trig_offset: unsigned (8 downto 0);

  
  
BEGIN

-- Need to generate separate clocks for the ADC (200MHz) and VGA (25MHz)
-- So a PLL module is empolyed for this purpose
U1 : entity work.ADC_PLL
    port map (CLKI=>clk, CLKOP=>ADC_clk, CLKOS=>VGA_25MHz);  

-- Declaration of a VGA timing controller begins here
U2 : entity work.vga_controller
    port map (
    reset  => rst,
    h_sync => VGA_HS,
    v_sync => sample_buttons,
    disp_ena  => vga_disp_en,
	pixel_clk=>VGA_25MHz,
    column => vga_column,    --horizontal pixel coordinate
    row => vga_row    		 --vertical pixel coordinate	
	);  
	
-- Declare a dual-port 1024-sample buffer for storing the captured ADC samples
-- (Dual port is required, as the buffer will also be read by the VGA at the
-- same time as ).
U3 : entity work.DSO_RAMBUFFER_CH1
    port map (
	-- Buffer channel-A: ADC Data sample (Note: Write-only)
	ClockA=>ADC_clk,
	ClockEnA=>'1', 
	ResetA=>rst, 
	WrA=>'1', 	
	AddressA => std_logic_vector(ADC_buffer_addr(9 downto 0)), 
	DataInA => std_logic_vector(adc_rawout(11 downto 3)), 
	QA => ADC_bufout, 
	
	-- Buffer channel-B: VGA Pixel plot data out (Note: Read-only)
	ClockB => VGA_25MHz, 
    ClockEnB => '1', 	
	ResetB => rst, 
	WrB => '0', 	
    AddressB => vga_column, 	
	DataInB => "000000000",     
	QB => vga_bufout
	);	
	

	 
-- Our combinatorial logic goes here:
-- 
ADC_1 <= NOT sample_LED(0);
ADC_2 <= NOT sample_LED(1);
ADC_3 <= NOT sample_LED(2);
ADC_4 <= NOT sample_LED(3);

VGA_VS <= sample_buttons;

	-- *** Sigma-Delta ADC sampling and processing loop ***
	-- Sampled data is stored in the Dual port RAM buffer for 
	-- display onto a VGA monitor
	-- RAM buffer address counter is reset to zero upon successful
	-- detection of positive-going signal that crosses the user-settable
	-- trigger level.
		PROCESS (ADC_clk, rst) 
		begin
		
			if rst = '1' then -- ADC and sample buffer reset
				ADC_buffer_addr <= "0000000000";
				adc_rawout <= "00000000000000";
				ADC_sample_divider <= "00000000000000";
				Sampler_Q <='0';
				trigger_flag <= '0';
				
			else -- Sigma-delta ADC sampling loop clocked @ 204.8MHz
				if(ADC_clk'event and ADC_clk='1') then 
				
					ADC_sample_divider <= ADC_sample_divider + 1; 
					
					Sampler_Q <= Sampler_D; -- sample ADC comparator value

				-- Increment the RAM buffer address based on the selected user sample rate
					if(ADC_sample_divider = sample_rate) then 
						ADC_buffer_addr <= ADC_buffer_addr + 1;
						ADC_sample_divider <= "00000000000000";	
					end if;
					
				-- Signal trigger level processing occurs here
					if((unsigned(adc_rawout(11 downto 3)) = trig_row) and (trigger_flag = '0')) then -- Rising-edge
						trigger_flag <= '1';
					end if;		
					
					if((unsigned(adc_rawout(11 downto 3)) > trig_lower) and (trigger_flag = '1')) then -- Rising-edge

						 if(ADC_buffer_addr > 639) then
							ADC_buffer_addr <= "0000000000";
						 end if;
						 trigger_flag <='0';
					end if;					
					
					if((unsigned(adc_rawout(11 downto 3)) < trig_upper) and (trigger_flag = '1')) then -- Rising-edge
							trigger_flag <='0';
					end if;		
					ADC_lowspeed_raw <= std_logic_vector(adc_rawout(11 downto 4));	-- RAW ADC OUTPUT TO UART (quick little hack... :-)
				-- RC filter model section i.e. sample resolution/accuracy is 
				-- intentionally dropped by 1 bit when a sample rate above 
				-- 128Ksamples/sec is selected. This allows a reasonable 
				-- tradeoff in ADC capture rate vs resolution.	
					if(ADC_filter_shift = '1') then
						adc_rawout <= adc_rawout + shift_right((("00"& Sampler_Q & "00000000000") - adc_rawout), 6) ;
					end if;
					if(ADC_filter_shift = '0') then
						adc_rawout <= adc_rawout + shift_right((("00"& Sampler_Q & "00000000000") - adc_rawout), 5) ;
					end if;
 
				end if;
			end if;
	end process; 								
 
 
 
	-- Setting of the scope timebase and trigger level via FleaFPGA user 
	-- buttons #1 and #2 happens here:
	PROCESS (sample_buttons) -- Note: 'sample_buttons' signal = 60Hz
	
	begin 
		if (sample_buttons'event and sample_buttons='1') then	
		
		sample_button_count <= sample_button_count + 1; --Button debounce counter

			-- Was the 'Samplerate_adj' button (PB#1) pressed?
			if ((samplerate_adj = '0') and (sample_button_count = 15)) then	
				sample_rate <= sample_rate + 1;
				if(sample_rate(0) = '1') then
					sample_rate <= "10000000000000";
					sample_LED <= "0000";
					ADC_filter_shift <= '1';
				else
					if(sample_LED(3) = '1') then
						ADC_filter_shift <= '0';
					else
						ADC_filter_shift <= '1';
					end if;
					sample_rate <= shift_right(sample_rate,1);
					sample_LED <= sample_LED + 1;
				end if;	
				
			-- Was the 'Trigger_adj' button (PB#2) pressed?
			end if; 		
			if (trigger_adj = '0') then
				if(trig_row > 360) then
					trig_row <= "000000000";
				else
					trig_row <= trig_row + 1;
				end if;
				trig_upper <= trig_row - 3;
				trig_lower <= trig_row + 3;	
				
			end if;				
		end if;
	end process; 	
	
	blank <= not vga_disp_en;
	
 -- Rendering of captured waveform data onto the VGA display happens here:
	PROCESS (VGA_25MHz)
	begin 
		if (VGA_25MHz'event and VGA_25MHz='1') then		
		
			if (vga_disp_en = '1') then
		
					-- Crude scaling of the Y-Axis (Signal Amplitude) happens here:
					vga_bufout_scaled <= ((unsigned(vga_bufout) * 20) / 15);				
					addpixel1 <= NOT (vga_bufout_scaled(8 downto 0) + 128);		
					addpixel2 <= NOT (vga_bufout_scaled(8 downto 0) + 128);		
					vga_trigger_scaled <= ((unsigned(trig_row) * 20) / 15);		
					trig_offset <= NOT (vga_trigger_scaled(8 downto 0) + 128);
					
					-- Reset the row count for drawing the measurement grid
					if(unsigned(vga_row(8 downto 0)) = 479) then
						gridrow <= "000110000";	
					end if;
					
					-- Reset the VGA color outputs
					Green_out <= "0000"; 
					Red_out <= "0000";
					Blue_out <= "0000";			

					-- Draw our measurement grid row
					if(unsigned(vga_row(8 downto 0)) = gridrow) then
						Green_out <= "0111";
						Red_out <= "0111";
						Blue_out <= "0111";	
						if(unsigned(vga_column) = 639) then -- Draw our Grid here
							gridrow <= gridrow + 64;
						end if;								
					end if;

					-- Draw our measurement grid column
					if (unsigned(vga_column) = grid_column)then
						Green_out <= "0111";
						Red_out <= "0111";
						Blue_out <= "0111";	
						grid_column <= grid_column + 50;
						if(grid_column = 600) then
							grid_column <= "0000000000";
						end if;
					end if;
					
					--Draw our trigger level as a dashed-line
					if((unsigned(vga_row(8 downto 0)) = trig_offset) AND (trig_pixel_count > 2)) then
						Green_out <= "0000"; 
						Red_out <= "1111";
						Blue_out <= "0000";	
						trig_pixel_count <= "000";
					end if;

					trig_pixel_count <= trig_pixel_count + 1;

					-- Draw our signal trace (with ypixel+0)
					if(unsigned(vga_row(8 downto 0)) = addpixel1) then
						Green_out <= "1111";
						set_pixel1 <= '1';
					end if;
					
					if(set_pixel1='1') then
						Green_out <= "1111";
						set_pixel1<='0';
					end if;
		
					-- Draw our signal trace (with ypixel+1)
					if(unsigned(vga_row(8 downto 0)) = addpixel2) then
						Green_out <= "1111";
						set_pixel2 <= '1';
					end if;
					
					if(set_pixel2='1') then
						Green_out <= "1111";
						set_pixel2<='0';
					end if;						
			else
					-- Reset the VGA color outputs when display is meant to be blanked
					Green_out <= "0000"; 
					Red_out <= "0000";
					Blue_out <= "0000";			
			end if;		
			
		end if;		
	end process;  
 
end architecture;