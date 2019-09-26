<?php
    # IPP - Principles of Programming Languages
    # Project 1
    # Benjamin Kosa
    # XKOSAB00

    function help(){
        echo "Skript typu filtr (parse.php v jazyce PHP 7.3) načte ze\n"; 
        echo "standardního vstupu zdrojový kód v IP-Pcode19, zkontroluje\n"; 
        echo "lexikální a syntaktickou správnost kódu a vypíše na standardní\n";
        echo "výstup XML reprezentaci programu dle specifikace v sekci.\n";
    }

    //ak nenajde komentar tak je to chyba
    //$input string
    function err_unknow_code($input){
        if(preg_match('/(^#.*$)/i', $input) == 0){
            return 22;
        }
        return 0;
    }

    //prevod problematikych znakov pre xml
    function str_to_xml($string){
        $string = preg_replace('/&/', '&amp;', $string);
        $string = preg_replace('/</', '&lt;', $string);
        $string = preg_replace('/>/', '&gt;', $string);
        $string = preg_replace('/"/', '&quot;', $string);
        $string = preg_replace('/\'/', '&apos;', $string);
        return $string;
    }

    //spracuje premennu a vypise na vystup
    //$arg string - premenna na spracovanie
    //$nmb int - poradove cislo pre vypis do xml
    function variable_($arg, $nmb){ //type var
        if(preg_match('/(^GF|LF|TF)(@)([a-z|A-Z|_|\-|$|&|%|*|!|?]*$)/', $arg)){
            $arg = str_to_xml($arg);
            echo "        <arg$nmb type=\"var\">$arg</arg$nmb>\n";
        }else
            return 23;
        return 0;
    }

    //spracuje konstantu a vypise na vystup
    //$arg string - premenna na spracovanie
    //$nmb int - poradove cislo pre vypis do xml
    function constant_($arg, $nmb){ //type int, bool, stringh
        if (preg_match('/(int)(@)([-+]?[0-9]+$)/', $arg, $output_array)){
            echo "        <arg$nmb type=\"int\">$output_array[3]</arg$nmb>\n";
        }else if (preg_match('/(^bool)(@)(true|false$)/', $arg, $output_array)){
            echo "        <arg$nmb type=\"bool\">$output_array[3]</arg$nmb>\n";
        }else if (preg_match('/(^string)(@)(.+?$)/', $arg, $output_array)){
            $arg = str_to_xml($output_array[3]);
            echo "        <arg$nmb type=\"string\">$arg</arg$nmb>\n";
        }else if (preg_match('/(^nil)(@)(nil$)/', $arg, $output_array)){
            echo "        <arg$nmb type=\"nil\">nil</arg$nmb>\n";
        }else
            return 23;
        
        return 0;
    }

    //funkcia na spracovanie <var> argumentu
    function arg_var($arg, $nmb){
        return variable_($arg, $nmb);
    }
    //funkcia na spracovanie <symb> argumentu
    function arg_symb($arg, $nmb){
        if (variable_($arg, $nmb) == 0)
            return 0;
        else 
            return constant_($arg, $nmb);
    }
    //funkcia na spracovanie <label> arumentu
    function arg_label($arg, $nmb){
        if(preg_match('/(^[a-z|A-Z|_|\-|$|&|%|*|!|?]*$)/', $arg)){
            $arg = str_to_xml($arg, $nmb);
            echo "        <arg$nmb type=\"label\">$arg</arg$nmb>\n";
        }else
            return 23;
        return 0;
    }
    //funkcia na spracovanie <type> arumentu
    function arg_type($arg, $nmb){
        if(preg_match('/(^int$|^bool$|^string$)/', $arg)){
            echo "        <arg$nmb type=\"type\">$arg</arg$nmb>\n";
        }else
            return 23;
        return 0;
    }

///////////////////////////////////////////////////////////////////////////

//ak najde --help
if (count($argv) == 2){
    if(preg_match('/(^--help$)/',$argv[1])){
        help();
        return 0;
    }else
        return 10;
//ak nieje zadany argument tak nic nespravi
} else if (count($argv) == 1)
    1+1;
//viac ako jeden argument je chyba
else
    return 10;

$order_counter = 0;
$found_header = 0;
while($f = fgets(STDIN)){
    //rozkuskuje string na slova a ulozi do pola
    $words = preg_split('/[\s]+/', $f, -1, PREG_SPLIT_NO_EMPTY);
    //filter prazdnych riadkov
    if(count($words) == 0)
        continue;
    //hladanie hlalvicky
    if($found_header == 0)
        if(preg_match('/(^\.IPPcode19$)/i', $words[0])){
            if(count($words) > 1)
                if(err_unknow_code($words[1]) == 22)
                    return 21;
            echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            echo "<program language=\"IPPcode19\">\n";
            $found_header = 1;
            continue;
        }else{
            echo $words[0];
            if (err_unknow_code($words[0]) == 22)
                return 21;
            continue;
        }

    $words[0] = strtoupper($words[0]);
    switch($words[0]){
        //no parameter
        case (preg_match('/(^CREATEFRAME$|^PUSHFRAME$|^POPFRAME$|^RETURN$|^BREAK$)/i', $words[0]) ? true : false):
            if(count($words) > 1)
                if(err_unknow_code($words[1]) == 22)
                    return 22;            
            echo "    <instruction order=\"",++$order_counter,"\" opcode=\"$words[0]\"></instruction>\n";
            break;
        //<var>
        case (preg_match('/(^DEFVAR$|^POPS$)/i', $words[0]) ? true : false):
            if(count($words) > 2)
                if (err_unknow_code($words[2]) == 22)
                    return 22;
            echo "    <instruction order=\"",++$order_counter,"\" opcode=\"$words[0]\">\n";
            if(arg_var($words[1], 1))
                return 23;
            echo "    </instruction>\n";
            break;
        //<var><symb>
        case (preg_match('/(^MOVE$|^STRLEN$|^TYPE$|^NOT$|^INT2CHAR$)/i', $words[0]) ? true : false):
            if(count($words) > 3)
                if (err_unknow_code($words[3]) == 22)
                    return 22;
            echo "    <instruction order=\"",++$order_counter,"\" opcode=\"$words[0]\">\n"; 
            if(arg_var($words[1], 1))
                return 23;
            if(arg_symb($words[2], 2))
                return 23;
            echo "    </instruction>\n";
            break;
        //<var><symb><symb>
        case (preg_match('/(^ADD$|^SUB$|^MUL$|^IDIV$|^LT$|^GT$|^EQ$|^AND$|^OR$|^STRI2INT$|^CONCAT$|^GETCHAR$|^SETCHAR$)/i', $words[0]) ? true : false):
            if(count($words) > 4)
                if (err_unknow_code($words[4]) == 22)
                    return 22;
            echo "    <instruction order=\"",++$order_counter,"\" opcode=\"$words[0]\">\n"; 
            if(arg_var($words[1], 1))
                return 23;
            if(arg_symb($words[2], 2))
                return 23;
            if(arg_symb($words[3], 3))
                return 23;
            echo "    </instruction>\n";
            break;
        //<var><type>
        case (preg_match('/(^READ$)/i', $words[0]) ? true : false):
            if(count($words) > 3)
                if (err_unknow_code($words[3]) == 22)
                    return 22;
            echo "    <instruction order=\"",++$order_counter,"\" opcode=\"$words[0]\">\n";
            if (arg_var($words[1], 1))
                return 23;
            if (arg_type($words[2], 2))
                return 23;
            echo "    </instruction>\n";
            break;
        //<label>
        case (preg_match('/(^CALL$|^LABEL$|^JUMP$)/i', $words[0]) ? true : false):
        if(count($words) > 2)
                if (err_unknow_code($words[2]) == 22)
                    return 22;
            echo "    <instruction order=\"",++$order_counter,"\" opcode=\"$words[0]\">\n";
            if (arg_label($words[1], 1))
                return 23;
            echo "    </instruction>\n";
            break;
        //<label><symb><symb>
        case (preg_match('/(^JUMPIFEQ$|^JUMPIFNEQ$)/i', $words[0]) ? true : false):
            if(count($words) > 4)
                if (err_unknow_code($words[4]) == 22)
                    return 22;
            echo "    <instruction order=\"",++$order_counter,"\" opcode=\"$words[0]\">\n"; 
            if (arg_label($words[1], 1))
                return 23;
            if (arg_symb($words[2], 2))
                return 23;
            if (arg_symb($words[3], 3))
                return 23;
            echo "    </instruction>\n";
            break;
        //<symb>    
        case (preg_match('/(^PUSHS$|^WRITE$|^EXIT$|^DPRINT$)/i', $words[0]) ? true : false):
            if(count($words) > 2)
                if (err_unknow_code($words[2]) == 22)
                    return 22;
            echo "    <instruction order=\"",++$order_counter,"\" opcode=\"$words[0]\">\n";
            if (arg_symb($words[1], 1))
                return 23;
            echo "    </instruction>\n";
            break;
        //commentar
        case (preg_match('/(^#.*?$)/i', $words[0]) ? true : false):
            break;
        default:
            return 22;
    }
    //print_r($words);

}
if($found_header == 1){
    echo "</program>\n";
    return 0;
}else
    return 21;
?>
