----------------------------------------------------------------------------------
-- Engineer:       Mike Field <hamster@snap.net.nz>
-- Module Name:    ColourTest - Behavioral 
-- Description:    Generates an 640x480 VGA showing all colours
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity vga is
   generic (
	-- Timings for 640x480@60Hz, 25MHz pixel clock  
	
	
	
     -- hRez       : natural := 640;	
     -- hStartSync : natural := 656;
     -- hEndSync   : natural := 752;
     -- hMaxCount  : natural := 800;
     -- hsyncActive : std_logic := '0';
		
     -- vRez       : natural := 480;
     -- vStartSync : natural := 490;
     -- vEndSync   : natural := 492;
     -- vMaxCount  : natural := 525;
     -- vsyncActive : std_logic := '1'   
   
     -- 720x576 (576p) -- 27MHz 720 732 796 864,  576 581 586 625
      hRez       : natural := 720;	
      hStartSync : natural := 732;
      hEndSync   : natural := 796;
      hMaxCount  : natural := 864;
      hsyncActive : std_logic := '0';
		
      vRez       : natural := 576;
      vStartSync : natural := 581;
      vEndSync   : natural := 586;
      vMaxCount  : natural := 625;
      vsyncActive : std_logic := '0'      
   
    -- Timings for 1280x720@60Hz, 75Mhz pixel clock
    --  hRez       : natural := 1280;	
    --  hStartSync : natural := 1352;
    --  hEndSync   : natural := 1432;
    --  hMaxCount  : natural := 1647;
    --  hsyncActive : std_logic := '1';
		
    --  vRez       : natural := 720;
    --  vStartSync : natural := 723;
    --  vEndSync   : natural := 728;
    --  vMaxCount  : natural := 750;
    --  vsyncActive : std_logic := '1'
   );

    Port ( pixelClock : in  STD_LOGIC;
           Red        : out STD_LOGIC_VECTOR (7 downto 0);
           Green      : out STD_LOGIC_VECTOR (7 downto 0);
           Blue       : out STD_LOGIC_VECTOR (7 downto 0);
           hSync      : buffer STD_LOGIC;
           vSync      : buffer STD_LOGIC;
           blank      : out STD_LOGIC);
end vga;

architecture rtl of vga is

     signal hCounter : STD_LOGIC_VECTOR(11 downto 0) := X"000";
     signal vCounter : STD_LOGIC_VECTOR(11 downto 0) := X"000";
	  
begin 
   -- Assign the outputs
--   hSync <= r.hSync;
--   vSync <= r.vSync;
 --  Red   <= r.red;
 --  Green <= r.green;
 --  Blue  <= r.blue;
 --  blank <= r.blank;
 

	    
   process(pixelClock)
   begin

      if rising_edge(pixelClock) then

		  -- Count the lines and rows      
		  if hCounter = hMaxCount-1 then
			 hCounter <= (others => '0');
			 if vCounter = vMaxCount-1 then
				vCounter <= (others => '0');
			 else
				vCounter <= vCounter+1;
			 end if;
		  else
			 hCounter <= hCounter+1;
		  end if;

		  if hCounter  < hRez and vCounter  < vRez then
			 red   <= hCounter(5 downto 0) & hCounter(5 downto 4);
			 green <= hCounter(7 downto 0);
			 blue  <= vCounter(7 downto 0);
			 blank <= '0';
		  else
			 red   <= (others => '0');
			 green <= (others => '0');
			 blue  <= (others => '0');
			 blank <= '1';
		  end if;
		  
		  -- Are we in the hSync pulse?
		  if hCounter >= hStartSync and hCounter < hEndSync then
				hSync <= hSyncActive;
			else
				hSync <= not hSyncActive;      
		  end if;

		  -- Are we in the vSync pulse?
		  if vCounter >= vStartSync and vCounter < vEndSync then
				vSync <= vSyncActive; 
			else
				vSync <= not vSyncActive;      
		  end if;
		end if;	  
   end process;
end architecture;
