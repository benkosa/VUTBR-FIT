--************INP - proj1***************--
-- Benjamin Kosa
-- 14.10.2018
--**************************************--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

--**************************************--
entity ledc8x8 is
    port (
        RESET : in  std_logic;
        SMCLK : in  std_logic;
        ROW   : out std_logic_vector (7 downto 0) := (others =>'0'); --(others =>'0'); fill zero
        LED   : out std_logic_vector (0 to 7) := (others =>'0')
    );
end ledc8x8;

--****************main******************--

architecture main of ledc8x8 is

    signal row_switcher :std_logic := '0';
    signal row_counter :std_logic_vector (2 downto 0) := (others =>'0');
    signal row_display :std_logic_vector (7 downto 0) := (others =>'0');

    signal char_switcher :std_logic_vector (20 downto 0) := (others =>'0');  --111000010000000000000 change char activity, len 21
    signal char_type :std_logic_vector (1 downto 0) := (others =>'0');
    
    signal clock_slower :std_logic_vector (11 downto 0) := (others =>'0');  --111000010000 change row activity, len 12
    
    signal led_display :std_logic_vector (7 downto 0) := (others =>'0');

begin

    process (RESET, SMCLK) begin

        if RESET = '1' then
            row_display <= (others =>'0');
            led_display <= (others =>'0');
            row_counter <= (others =>'0');

        elsif rising_edge(SMCLK) then

            --***********char changer***************--
            char_switcher <= char_switcher + 1;
            if char_switcher(20 downto 0) = "111000010000000000000" then
                char_type <= char_type + 1;
                char_switcher <= (others =>'0');
            end if;

            --************row switcher**************--
            clock_slower <= clock_slower + 1;
            if clock_slower(11 downto 0) = "111000010000" then
                row_switcher <= '1';
                clock_slower <= (others =>'0');
            else
                row_switcher <= '0';
            end if;

            --***********row shifter****************--
            if row_switcher = '1' then
                case row_counter is
                    when "000" => row_display <= "00000001";
                    when "001" => row_display <= "00000010";
                    when "010" => row_display <= "00000100";
                    when "011" => row_display <= "00001000";
                    when "100" => row_display <= "00010000";
                    when "101" => row_display <= "00100000";
                    when "110" => row_display <= "01000000";
                    when "111" => row_display <= "10000000";
                    when others =>
                end case;
                row_counter <= row_counter+1;
            end if;

            --***********char display***************--
            --*************char B*******************--
            if char_type = "00" then
                case row_display is
                    when "00000001" => led_display <= "00000011";
                    when "00000010" => led_display <= "10111101";
                    when "00000100" => led_display <= "10111011";
                    when "00001000" => led_display <= "10000001";
                    when "00010000" => led_display <= "10111110";
                    when "00100000" => led_display <= "10111110";
                    when "01000000" => led_display <= "10111110";
                    when "10000000" => led_display <= "00000001";
                    when others =>
                end case;

            --*************char none****************--
            elsif char_type = "01" then
                case row_display is
                    when "00000001" => led_display <= "11111111";
                    when "00000010" => led_display <= "11111111";
                    when "00000100" => led_display <= "11111111";
                    when "00001000" => led_display <= "11111111";
                    when "00010000" => led_display <= "11111111";
                    when "00100000" => led_display <= "11111111";
                    when "01000000" => led_display <= "11111111";
                    when "10000000" => led_display <= "11111111";
                    when others =>
                end case;

            --*************char K*******************--
            elsif char_type = "10" then
                case row_display is
                    when "00000001" => led_display <= "00010000";
                    when "00000010" => led_display <= "10111011";
                    when "00000100" => led_display <= "10110111";
                    when "00001000" => led_display <= "10001111";
                    when "00010000" => led_display <= "10110111";
                    when "00100000" => led_display <= "10111011";
                    when "01000000" => led_display <= "10111101";
                    when "10000000" => led_display <= "00011000";
                    when others =>
                end case;

            --*************char none****************--
            elsif char_type = "11" then
                case row_display is
                    when "00000001" => led_display <= "11111111";
                    when "00000010" => led_display <= "11111111";
                    when "00000100" => led_display <= "11111111";
                    when "00001000" => led_display <= "11111111";
                    when "00010000" => led_display <= "11111111";
                    when "00100000" => led_display <= "11111111";
                    when "01000000" => led_display <= "11111111";
                    when "10000000" => led_display <= "11111111";
                    when others =>
                end case;
            end if;
        end if ;
    end process;

    ROW <= row_display;
    LED <= led_display;

end main;