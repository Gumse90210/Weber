<?php
    include 'arduino_globvar.php';
    
    
    SetSetpoint($_POST['setpoint_tb']);
    
    SetPIDKp($_POST['kp_tb']);
    
    SetPIDKi($_POST['ki_tb']);
    
    header('Location: arduino_temp.php');
    
    exit;
?>