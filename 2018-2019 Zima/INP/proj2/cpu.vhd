-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xkosab00 (Benjamin Kosa)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

   signal pc_reg : std_logic_vector(11 downto 0);
   signal pc_reg_inc : std_logic;
   signal pc_reg_dec : std_logic;

   signal ptr_reg : std_logic_vector(9 downto 0);
   signal ptr_reg_inc : std_logic;
   signal ptr_reg_dec : std_logic;

   signal cnt_reg : std_logic_vector(7 downto 0);
   signal cnt_reg_inc : std_logic;
   signal cnt_reg_dec : std_logic;

   signal mx_memory : std_logic_vector(7 downto 0);

   signal FSM_sel : std_logic_vector(4 downto 0);
   type state_type is (
      init, fetch, decode,      
      ptr_inc, ptr_dec, -- > <
      data_inc, data_inc_2, -- + -      
      data_dec, data_dec_2,
      loop_start, loop_start_2, loop_start_3, loop_start_4,  --[
      loop_end, loop_end_2, loop_end_3, loop_end_4, loop_end_5, --]
      data_print, data_print_2, -- .      
      data_load, data_load_2, -- ,
      commentar, commentar_fetch, commentar_decode, --#
      V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF, --0xV0-F
      halt, -- null
      other                  
   );
   signal actual_state : state_type;
   signal next_state : state_type;

begin


-- PROGRAM COUNTER --
PC_program_counter: process (RESET, CLK, pc_reg_inc, pc_reg_dec, pc_reg)
begin
   if (RESET = '1') then
      pc_reg <= (others => '0');
   elsif (rising_edge(CLK)) then
      if (pc_reg_inc = '1') then
         pc_reg <= pc_reg + 1;
      elsif (pc_reg_dec = '1') then
         pc_reg <= pc_reg - 1;
      end if;
   end if;
end process;
CODE_ADDR <= pc_reg;


-- Counter --
CNT_counter: process (RESET, CLK, cnt_reg_inc, cnt_reg_dec, cnt_reg)
begin
   if (RESET = '1') then
      cnt_reg <= (others => '0');
   elsif (rising_edge(CLK)) then
      if (cnt_reg_inc = '1') then
         cnt_reg <= cnt_reg + 1;
      elsif (cnt_reg_dec = '1') then
         cnt_reg <= cnt_reg - 1;
      end if;
   end if;
end process;


-- Pointer --
PTR_pointer: process(RESET, CLK, ptr_reg_inc, ptr_reg_dec, ptr_reg)
begin
   if (RESET = '1') then
      ptr_reg <= (others => '0');
   elsif (rising_edge(CLK)) then
      if (ptr_reg_inc = '1') then
         ptr_reg <= ptr_reg + 1;
      elsif (ptr_reg_dec = '1') then
         ptr_reg <= ptr_reg - 1;
      end if;
   end if;
end process;
DATA_ADDR <= ptr_reg;


-- Multiplexor --
MX_multiplexor: process(FSM_sel, DATA_RDATA, IN_DATA, mx_memory)
begin
   mx_memory <= DATA_RDATA;
   case(FSM_sel) is
      when "00000" => DATA_WDATA <= IN_DATA;
      when "00010" => DATA_WDATA <= (mx_memory - 1);
      when "00011" => DATA_WDATA <= (mx_memory + 1);

      when "00100" => DATA_WDATA <= X"00";
      when "00101" => DATA_WDATA <= X"10";
      when "00110" => DATA_WDATA <= X"20";      
      when "00111" => DATA_WDATA <= X"30";

      when "01000" => DATA_WDATA <= X"40";
      when "01001" => DATA_WDATA <= X"50";
      when "01010" => DATA_WDATA <= X"60";
      when "01011" => DATA_WDATA <= X"70";

      when "01100" => DATA_WDATA <= X"80";
      when "01101" => DATA_WDATA <= X"90";
      when "01110" => DATA_WDATA <= X"A0";
      when "01111" => DATA_WDATA <= X"B0";

      when "10000" => DATA_WDATA <= X"C0";
      when "10001" => DATA_WDATA <= X"D0";
      when "10010" => DATA_WDATA <= X"E0";
      when "10011" => DATA_WDATA <= X"F0";
      when others => null;
   end case;
end process;


-- FSM: finite-state machine -- actual_state
FSM_finite_state_machine_PresentState: process(RESET, CLK, EN, actual_state, next_state)
begin
   if (RESET = '1') then
      actual_state <= init;
   elsif (rising_edge(CLK)) then
      if (EN = '1') then
         actual_state <= next_state;
      end if;
   end if;
end process;


-- finite-state machine -- next_state
FSM_finite_state_machine_NextState: process(actual_state, next_state, DATA_RDATA, CODE_DATA, cnt_reg, OUT_BUSY, IN_VLD)
begin
   -- inicializacia 
   next_state <= init;

   pc_reg_inc   <= '0';
   pc_reg_dec   <= '0';

   cnt_reg_inc  <= '0';
   cnt_reg_dec  <= '0';

   ptr_reg_inc  <= '0';
   ptr_reg_dec  <= '0';

   CODE_EN   <= '0';

   DATA_RDWR <= '0';
   DATA_EN   <= '0';

   IN_REQ    <= '0';
   OUT_WE    <= '0';

   FSM_sel <= "00000";

   OUT_DATA <= DATA_RDATA;

   -- zaciatok FSM
   case actual_state is

      when init =>
         next_state <= fetch;

      when fetch =>
         CODE_EN <= '1';
         next_state <= decode;

      when decode =>
         case(CODE_DATA) is
            when X"3E"  => next_state <= ptr_inc;       -- > inkrementace hodnoty ukazatele
            when X"3C"  => next_state <= ptr_dec;       -- < dekrementace hodnoty ukazatele
            when X"2B"  => next_state <= data_inc;    -- + inkrementace hodnoty aktualnı bunky 
            when X"2D"  => next_state <= data_dec;    -- - dekrementace hodnoty aktualnı bunky 
            when X"5B"  => next_state <= loop_start;  -- [ while (*ptr) { 
            when X"5D"  => next_state <= loop_end;    -- ] }
            when X"2E"  => next_state <= data_print;    -- . vytiskni hodnotu aktualnı bunky
            when X"2C"  => next_state <= data_load;    -- , nacti hodnotu a uloz ji do aktualnı bunky
            when X"23"  => next_state <= commentar;     -- # blokovy komentar
            when X"30"  => next_state <= V0;            -- # *ptr = 0xV0;
            when X"31"  => next_state <= V1;            -- # *ptr = 0xV1;
            when X"32"  => next_state <= V2;            -- # *ptr = 0xV2;
            when X"33"  => next_state <= V3;            -- # *ptr = 0xV3;
            when X"34"  => next_state <= V4;            -- # *ptr = 0xV4;
            when X"35"  => next_state <= V5;            -- # *ptr = 0xV5;
            when X"36"  => next_state <= V6;            -- # *ptr = 0xV6;
            when X"37"  => next_state <= V7;            -- # *ptr = 0xV7;
            when X"38"  => next_state <= V8;            -- # *ptr = 0xV8;
            when X"39"  => next_state <= V9;            -- # *ptr = 0xV9;
            when X"41"  => next_state <= VA;            -- # *ptr = 0xVA;
            when X"42"  => next_state <= VB;            -- # *ptr = 0xVB;
            when X"43"  => next_state <= VC;            -- # *ptr = 0xVC;
            when X"44"  => next_state <= VD;            -- # *ptr = 0xVD;
            when X"45"  => next_state <= VE;            -- # *ptr = 0xVE;
            when X"46"  => next_state <= VF;            -- # *ptr = 0xVF;
            when X"00"  => next_state <= halt;          -- null zastav vykonavanı program
            when others => next_state <= other;         -- ostatne
         end case;

-- ----------------------------------------------------------------------------
      when ptr_inc => -- >
         next_state <= init;
         ptr_reg_inc <= '1';
         pc_reg_inc  <= '1';
-- ----------------------------------------------------------------------------
      when ptr_dec => -- <
         next_state <= init;
         ptr_reg_dec <= '1';
         pc_reg_inc  <= '1';
-- ----------------------------------------------------------------------------
      when data_inc => -- +
         next_state <= data_inc_2;
         DATA_EN <= '1';
         DATA_RDWR <= '1';

      when data_inc_2 =>
         next_state <= init;
         FSM_sel <= "00011";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
-- ----------------------------------------------------------------------------
      when data_dec => -- -
         next_state <= data_dec_2;
         DATA_EN <= '1';
         DATA_RDWR <= '1';

      when data_dec_2 =>
         next_state <= init;
         FSM_sel <= "00010";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
-- ----------------------------------------------------------------------------
      when loop_start => -- [
         next_state <= loop_start_2;
         pc_reg_inc <= '1';
         DATA_EN <= '1';
         DATA_RDWR <= '1';

      when loop_start_2 =>
         case(DATA_RDATA) is
            when "00000000" =>
               next_state <= loop_start_3;
               cnt_reg_inc <= '1';
            when others => next_state <= init;
         end case;

      when loop_start_3 =>
         case(cnt_reg) is
            when "00000000" => next_state <= init;
            when others =>
               next_state <= loop_start_4;
               CODE_EN <= '1';
         end case;

      when loop_start_4 =>
         if (CODE_DATA = X"5B") then -- [
            cnt_reg_inc <= '1';
         elsif (CODE_DATA = X"5D") then -- ]
             cnt_reg_dec <= '1';
         end if;
         pc_reg_inc <= '1';
         next_state <= loop_start_3;
-- ----------------------------------------------------------------------------
      when loop_end => -- ]
         next_state <= loop_end_2;
         DATA_EN <= '1';
         DATA_RDWR <= '1';

      when loop_end_2 =>
         case(DATA_RDATA) is
            when "00000000" =>
               next_state <= init;
               pc_reg_inc <= '1';
            when others =>
               next_state <= loop_end_3;
               cnt_reg_inc <= '1';
               pc_reg_dec <= '1';
         end case;

      when loop_end_3 =>
         case(cnt_reg) is
            when "00000000" => next_state <= init;
            when others =>
               next_state <= loop_end_4;
               CODE_EN <= '1';
         end case;

      when loop_end_4 =>
         if (CODE_DATA = X"5D") then -- ]
            cnt_reg_inc <= '1';
         elsif (CODE_DATA = X"5B") then -- [
            cnt_reg_dec <= '1';
         end if;
         next_state <= loop_end_5;

      when loop_end_5 =>
         next_state <= loop_end_3;
         case(cnt_reg) is
            when "00000000" => pc_reg_inc <= '1';
            when others => pc_reg_dec <= '1';
         end case;
-- ----------------------------------------------------------------------------
      when data_print => -- .
         next_state <= data_print_2;
         DATA_EN <= '1';
         DATA_RDWR <= '1';

      when data_print_2 =>
         case(OUT_BUSY) is
            when '1' => next_state <= data_print_2;
            when others =>
               next_state <= init;
               OUT_WE <= '1';
               pc_reg_inc <= '1';
         end case;
-- ----------------------------------------------------------------------------
      when data_load => -- ,
         IN_REQ <= '1';
         case(IN_VLD) is
            when '1' =>
               next_state <= data_load_2;               
               FSM_sel <= "00000";
            when others => next_state <= data_load;
         end case;

      when data_load_2 =>
         next_state <= init;
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
-- ----------------------------------------------------------------------------
      when commentar => -- #
         next_state <= commentar_fetch;
         pc_reg_inc  <= '1';

      when commentar_fetch =>
         next_state <= commentar_decode;
         CODE_EN <= '1';

      when commentar_decode =>
         case(CODE_DATA) is
            when X"23"  =>
               next_state <= init;                    -- # koniec komentara
               pc_reg_inc  <= '1'; 
            when X"00"  => next_state <= halt;        -- neukonceny komentar
            when others => next_state <= commentar;   -- filtrovanie vsetkeho v komentari
         end case;
-- ----------------------------------------------------------------------------
      when V0 =>
         next_state <= init;
         FSM_sel <= "00100";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V1 =>
         next_state <= init;
         FSM_sel <= "00101";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V2 =>
         next_state <= init;
         FSM_sel <= "00110";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V3 =>
         next_state <= init;
         FSM_sel <= "00111";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V4 =>
         next_state <= init;
         FSM_sel <= "01000";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V5 =>
         next_state <= init;
         FSM_sel <= "01001";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V6 =>
         next_state <= init;
         FSM_sel <= "01010";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V7 =>
         next_state <= init;
         FSM_sel <= "01011";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V8 =>
         next_state <= init;
         FSM_sel <= "01100";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when V9 =>
         next_state <= init;
         FSM_sel <= "01101";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when VA =>
         next_state <= init;
         FSM_sel <= "01110";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when VB =>
         next_state <= init;
         FSM_sel <= "01111";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when VC =>
         next_state <= init;
         FSM_sel <= "10000";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when VD =>
         next_state <= init;
         FSM_sel <= "10001";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when VE =>
         next_state <= init;
         FSM_sel <= "10010";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
      
      when VF =>
         next_state <= init;
         FSM_sel <= "10011";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         pc_reg_inc <= '1';
-- ----------------------------------------------------------------------------
      when halt => next_state <= halt; -- null
-- ----------------------------------------------------------------------------
      when other => -- OTHER
         next_state <= init;
         pc_reg_inc <= '1';
-- ----------------------------------------------------------------------------
      when others =>
         null;

   end case;
end process;

end behavioral;
 
