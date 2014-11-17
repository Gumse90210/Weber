<?php

    //.csv files are data read from client
    define("CHANNEL0_LOGFILE", "./arduino_data_chn0.csv");
    define("CHANNEL1_LOGFILE", "./arduino_data_chn1.csv");
    define("CHANNEL2_LOGFILE", "./arduino_data_chn2.csv");
    define("CHANNEL3_LOGFILE", "./arduino_data_chn3.csv");
    
    define("CHANNEL0_LOGFILE_BCK", "./arduino_data_chn0_bck.csv");
    define("CHANNEL1_LOGFILE_BCK", "./arduino_data_chn1_bck.csv");
    define("CHANNEL2_LOGFILE_BCK", "./arduino_data_chn2_bck.csv");
    define("CHANNEL3_LOGFILE_BCK", "./arduino_data_chn3_bck.csv");
    
    define("SETPOINT_LOGFILE", "./arduino_data_setpoint.csv");
    define("FAN_LOGFILE", "./arduino_data_fan.csv");
    define("TIMESTAMP_LOGFILE", "./arduino_timestamp.csv");
    
    define("SETPOINT_LOGFILE_BCK", "./arduino_data_setpoint_bck.csv");
    define("FAN_LOGFILE_BCK", "./arduino_data_fan_bck.csv");
    define("TIMESTAMP_LOGFILE_BCK", "./arduino_timestamp_bck.csv");
    
    define("ALL_LOGFILE", "./arduino_all_data.csv");
    define("ALL_LOGFILE_BCK", "./arduino_all_data_bck.csv");
    
    //.dat files are data written from ui to server
    define("SETPOINT_SETFILE", "./arduino_setpoint.set");
    define("KP_SETFILE", "./arduino_kp.set");
    define("KI_SETFILE", "./arduino_ki.set");
?>