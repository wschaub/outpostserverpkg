<html>
<head>
<title>System Status Page</title>
</head>
<body>
<h1> DHCP Leases </h1>
<table border=1>
<tr>
<th>Expires</th><th>MAC Address</th><th>IP Address</th><th>Hostname</th>
</tr>
<?php
$dhcpfh = fopen("/var/lib/misc/dnsmasq.leases","r");
while($line = fgets($dhcpfh)) {
    $lease = explode(" ",$line);
    $lease[0] = strftime("%A %B %d %Y %I:%M:%S %p",$lease[0]);
    echo "<tr><td>$lease[0]</td><td>$lease[1]</td><td>$lease[2]</td><td>$lease[3]</td></tr>\n";
}
?>
<?php if(is_file("/var/log/sunsaver.csv")) { //Only display solar panel table is log file exists?>
</table>
<h1>Solar Power status</h1>
<?php
$logfh = popen("tail -n 1 /var/log/sunsaver.csv","r");
$ssdata = explode(",",fgets($logfh));
$timestamp = strftime("%A %B %d %Y %I:%M:%S %p",$ssdata[0]);
$adc_vb_f = $ssdata[1];
$adc_va_f = $ssdata[2];
$adc_vl_f = $ssdata[3];
$adc_ic_f = $ssdata[4];
$adc_il_f = $ssdata[5];
$T_hs = $ssdata[6];
$T_batt = $ssdata[7];
$T_amb = $ssdata[8];
$T_rts = $ssdata[9];
$charge_state = $ssdata[10];
$array_fault = $ssdata[11];
$Vb_f = $ssdata[12];
$Vb_ref = $ssdata[13];
$Ahc_r = $ssdata[14];
$Ahc_t = $ssdata[15];
$kWhc = $ssdata[16];
$load_state = $ssdata[17];
$load_fault = $ssdata[18];
$V_lvd = $ssdata[19];
$Ahl_r = $ssdata[20];
$Ahl_t = $ssdata[21];
$hourmeter = $ssdata[22];
$diagalarm = $ssdata[23];
$dip1 = $ssdata[24];
$dip2 = $ssdata[25];
$dip3 = $ssdata[26];
$dip4 = $ssdata[27];
$ledstate = $ssdata[28];
$Power_out = $ssdata[29];
$Sweep_Vmp = $ssdata[30];
$Sweep_Pmax = $ssdata[31];
$Sweep_Voc = $ssdata[32];
$Vb_min_daily = $ssdata[33];
$Vb_max_daily = $ssdata[34];
$Ahc_daily = $ssdata[35];
$Ahl_daily = $ssdata[36];
$array_fault_daily = $ssdata[37];
$load_fault_daily = $ssdata[38];
$alarm_daily = $ssdata[39];
$vb_min = $ssdata[40];
$vb_max = $ssdata[41];
?>
<table width="100%" border=1>
<tr><th>date/time of reading</th><th>Battery voltage(filtered)</th><th>Array Voltage (filtered)</th>
<th>load voltage (filtered)</th><th>charging current (filtered)</th><th>Load currrent (filtered)</th>
<th>Heatsink temp</th><th>Battery Temp</th><th>Ambient Temp</th><th>Probe Temp</th><th>Charge State</th>
</tr>
<tr>
<?php 
echo "<td>$timestamp</td><td>$adc_vb_f</td><td>$adc_va_f</td><td>$adc_vl_f</td><td>$adc_ic_f</td>\n";
echo "<td>$adc_il_f</td><td>$T_hs</td><td>$T_batt</td><td>$T_amb</td><td>$T_rts</td><td>$charge_state</td>\n";
?>
</tr>
<tr>
<th>Array Fault</th><th>Battery voltage (slow filter)</th><th>Battery regulator reference voltage</th>
<th>Ah charge (resetable)</th><th>Ah charge total</th><th>KWh charge</th><th>load state</th>
<th>Load fault</th><th>Load current compensated LVD voltage</th><th>Ah load (resetable)</th><th>Ah load total</th>
</tr>
<tr>
<?php
echo "<td>$array_fault</td><td>$Vb_f</td><td>$Vb_ref</td><td>$Ahc_r</td><td>$Ahc_t</td><td>$kWhc</td>\n";
echo "<td>$load_state</td><td>$load_fault</td><td>$V_lvd</td><td>$Ahl_r</td><td>$Ahl_t</td>\n"
?>
</tr>
<tr>
<th>hourmeter</th><th>Self-diagnostic alarms</th><th>DIP switch 1</th><th>DIP switch 2</th><th>DIP switch 3</th><th>DIP switch 4</th><th>SOC LED State</th><th>Charge output power</th><th>Array Vmp found during sweep</th><th>Array Pmax(output) found during sweep</th><th>Array Voc found during sweep</th>
</tr>
<?php
    echo "<td>$hourmeter</td><td>$diagalarm</td><td>$dip1</td><td>$dip2</td><td>$dip3</td><td>$dip4</td>\n";
    echo "<td>$ledstate</td><td>$Power_out</td><td>$Sweep_Vmp</td><td>$Sweep_Pmax</td><td>$Sweep_Voc</td>\n";
?>
<tr>
<th>battery minimum voltage daily</th><th>battery max voltage daily</th><th>Ah charge daily</th><th>Ah load daily</th>
<th>Array fault daily</th><th>load fault daily</th><th>alarm daily</th><th>Minimum battery voltage</th>
<th>Maximum battery voltage</th>
</tr>
<tr>
<?php
    echo "<td>$Vb_min_daily</td><td>$Vb_max_daily</td><td>$Ahc_daily</td><td>$Ahl_daily</td><td>$array_fault_daily</td>\n";
    echo "<td>$load_fault_daily</td><td>$alarm_daily</td><td>$vb_min</td><td>$vb_max</td>\n";
?>
</tr>
</table>
<?php } //End of conditional for displaying table?>
</body>
</html>
