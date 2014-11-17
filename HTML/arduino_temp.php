<?php
    
  include 'arduino_globvar.php';
  include 'arduino_defines.php';

    
 if($_GET['button1']) { RestartDeleteCsv(); }

 function RestartDeleteCsv()
 {
   copy (ALL_LOGFILE, ALL_LOGFILE_BCK);
   $file = fopen(ALL_LOGFILE,'r+');
   ftruncate($file, 0);
   
   copy (CHANNEL0_LOGFILE, CHANNEL0_LOGFILE_BCK);
   $file = fopen(CHANNEL0_LOGFILE,'r+');
   ftruncate($file, 0);

   copy (CHANNEL1_LOGFILE, CHANNEL1_LOGFILE_BCK);
   $file = fopen(CHANNEL1_LOGFILE,'r+');
   ftruncate($file, 0);
  
   copy (CHANNEL2_LOGFILE, CHANNEL2_LOGFILE_BCK);
   $file = fopen(CHANNEL2_LOGFILE,'r+');
   ftruncate($file, 0);
     
   copy (CHANNEL3_LOGFILE, CHANNEL3_LOGFILE_BCK);
   $file = fopen(CHANNEL3_LOGFILE,'r+');
   ftruncate($file, 0);
   
   copy (SETPOINT_LOGFILE, SETPOINT_LOGFILE_BCK);
   $file = fopen(SETPOINT_LOGFILE,'r+');
   ftruncate($file, 0);

   copy (FAN_LOGFILE, FAN_LOGFILE_BCK);
   $file = fopen(FAN_LOGFILE,'r+');
   ftruncate($file, 0);
   
   copy (TIMESTAMP_LOGFILE, TIMESTAMP_LOGFILE_BCK);
   $file = fopen(TIMESTAMP_LOGFILE,'r+');
   ftruncate($file, 0);

 }
    
?>


<meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;"/>
<meta name="HandheldFriendly" content="True" />
<meta name="apple-mobile-web-app-capable" content="yes" />
<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent" />

<!DOCTYPE html>
<html lang="en">
  <head>
    <link rel="apple-touch-icon"
    href="images/apple-touch-icon.png" />
    <title></title>
    <style>

caption {
    text-align: center;
    background-color: #808080;
}

table#BBQTemp tr {
text-align: center;
background-color:#A0A0A0;
}




table#BBQSet tr  {
text-align: center;
background-color: #B0B0B0;
}




    table#BBQHist tr  {
      text-align: center;
      background-color: #C0C0C0;
    }


    </style>
  </head>
  <body>

<!-- <img src="WeberLogo.jpg"> -->

<font face="verdana" size="12">
<table id="BBQTemp" border="0" style="width:568px">
<caption>BBQ Temperatures</caption>
<tr>
</tr>
<tr>
</tr>
<th>Setpoint</th>
<th>Pit</th>
<th>Food 1</th>
<th>Food 2</th>
<th>Food 3</th>
</tr>
<tr>
<td><?php echo GetSetpoint(); ?>&#176;C</td>
<td><?php echo GetPitTemperature(); ?>&#176;C</td>
<td>-&#176;C</td>
<td>-&#176;C</td>
<td>-&#176;C</td>
</tr>
</table>


<form action="arduino_getinput.php" method="post">
<table id="BBQSet" border="0" style="width:568px">
<caption>BBQ Settings</caption>
<tr>
</tr>
<tr>
</tr>
<th>Setpoint</th>
<th>PID Kp</th>
<th>PID Ki</th>
</tr>
<tr>
<td><input type="text" VALUE = "<?php echo GetSetpoint(); ?>"  STYLE="font-size: 12px; background-color: #F0F0F0;" maxlength="3" size="4" name="setpoint_tb" />&#176;C</td>
<td><input type="text" VALUE = "<?php echo GetPIDKp(); ?>"     STYLE="font-size: 12px; background-color: #F0F0F0;" maxlength="3" size="4" name="kp_tb" /></td>
<td><input type="text" VALUE = "<?php echo GetPIDKi(); ?>"     STYLE="font-size: 12px; background-color: #F0F0F0;" maxlength="3" size="4" name="ki_tb" /></td>
</tr>
<tr>
<td><input type="Submit" name="submit" value="Set Setpoint"></td>
<td><input type="Submit" name="submit" value="Set Kp"></td>
<td><input type="Submit" name="submit" value="Set Ki"></td>
</tr>
</table>
</form>

<img src="arduino_graph.php">
<br>

<table id="BBQHist" border="0" style="width:568px">
<caption>History Settings</caption>
<tr>
</tr>
<tr>
</tr>
<th>History</th>
<th>...</th>
<th>...</th>
</tr>
<tr>
<td><button id="btnfun1" name="btnfun1" onClick='location.href="?button1=1"'>Delete</button></td>
<td>-</td>
<td>-</td>

</tr>

</table>



</font>
  </body>


</html>

