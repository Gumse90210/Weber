<?php
    
    include 'arduino_defines.php';
    
    
    function SetSetpoint($value)
    {
        $filename = SETPOINT_SETFILE;
        
        WriteToFile($filename, $value);
    }
    
    function GetSetpoint()
    {
        $filename = SETPOINT_SETFILE;
        
        $default = 100;
        
        return ReadFromFile($filename, $default);
        
        
    }
    
    function SetPIDKp($value)
    {
        $filename = KP_SETFILE;
        
        WriteToFile($filename, $value);
    }
    
    function GetPIDKp()
    {
        $filename = KP_SETFILE;
        
        $default = 75;
        
        return ReadFromFile($filename, $default);
    }
    
    function SetPIDKi($value)
    {
        $filename = KI_SETFILE;
        
        WriteToFile($filename, $value);
    }
    
    function GetPIDKi()
    {
        $filename = KI_SETFILE;
        
        $default = 5;
        
        return ReadFromFile($filename, $default);
    }
    
    
    //gets the last entry in file
    function GetPitTemperature()
    {
        $return_value = "-";
        
        if (file_exists(CHANNEL0_LOGFILE))
        {
          $lines =file(CHANNEL0_LOGFILE);
        
          foreach($lines as $data)
          {
            list($temp[]) = explode(',',$data);
          }
          $return_value = $temp[sizeof($temp) - 1];
        }
        
        return $return_value;
    }
    
    
    function WriteToFile($filename, $value)
    {
      $file = fopen($filename,'w+');
      if ($file)
      {
        fwrite($file, $value);
        fclose($file);
      }
    }
    
    function ReadFromFile($filename, $default_value)
    {
        $value = $default_value;
        
        if (file_exists($filename))
        {
          $file = fopen($filename,'r');
          if ($file)
          {
            if (filesize($filename) > 0)
            {
                $value = fread($file, filesize($filename));
                
                fclose($file);
                $setpoint = $value;
            }
            else
            {
                fclose($file);
                WriteToFile($filename, $value);
            }
          }
          else
          {
            fclose($file);
            WriteToFile($filename, $value);
          }
        }
        else
        {
          WriteToFile($filename, $value);
        }

        return $value;
    }
    
    

?>
