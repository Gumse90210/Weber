<?PHP
    
    include 'arduino_globvar.php';
    include 'arduino_defines.php';

    $CHN0 = $_GET['CHN0'];
    $CHN1 = $_GET['CHN1'];
    $CHN2 = $_GET['CHN2'];
    $CHN3 = $_GET['CHN3'];
    
    $SP = $_GET['SP'];
    $FAN = $_GET['FAN'];
    
    //update timestamp file
    
    $date = date_create();
    
    $savestring = date_format($date, 'Y.m.d H:i:s') . ',' . "\r\n";
    $file = fopen(TIMESTAMP_LOGFILE,'a+');
    if ($file)
    {
        fwrite($file, $savestring);
        fclose($file);
    }
    
    //

    $savestring = date_format($date, 'Y.m.d H:i:s') . ',' . ($CHN0 . ',' . $CHN1 . ',' . $CHN2 . ',' . $CHN3 . ',' . $SP . ',' . $FAN . ',' . "\r\n");
    $file = fopen(ALL_LOGFILE,'a+');
    if ($file)
    {
        if (0 == filesize(ALL_LOGFILE))
        {
            fwrite($file, "TIME, CHN0, CHN1, CHN2, CHN3, SP, FAN,\r\n");
        }
        fwrite($file, $savestring);
        fclose($file);
    }

    $savestring = $CHN0 . ',' . "\r\n";
    $file = fopen(CHANNEL0_LOGFILE,'a+');
    if ($file)
    {
        fwrite($file, $savestring);
        fclose($file);
    }

    $savestring = $CHN1 . ',' . "\r\n";
    $file = fopen(CHANNEL1_LOGFILE,'a+');
    if ($file)
    {
        fwrite($file, $savestring);
        fclose($file);
    }

    $savestring = $CHN2 . ',' . "\r\n";
    $file = fopen(CHANNEL2_LOGFILE,'a+');
    if ($file)
    {
        fwrite($file, $savestring);
        fclose($file);
    }

    $savestring = $CHN3 . ',' . "\r\n";
    $file = fopen(CHANNEL3_LOGFILE,'a+');
    if ($file)
    {
        fwrite($file, $savestring);
        fclose($file);
    }

    $savestring = $SP . ',' . "\r\n";
    $file = fopen(SETPOINT_LOGFILE,'a+');
    if ($file)
    {
        fwrite($file, $savestring);
        fclose($file);
    }

    $savestring = $FAN . ',' . "\r\n";
    $file = fopen(FAN_LOGFILE,'a+');
    if ($file)
    {
        fwrite($file, $savestring);
        fclose($file);
    }

    $setpoint = GetSetpoint();
    $pidkp = GetPIDKp();
    $pidki = GetPIDKi();
    
    $txt = sprintf("{%03d%03d%03d}",$setpoint,$pidkp,$pidki);
    echo $txt;

?>