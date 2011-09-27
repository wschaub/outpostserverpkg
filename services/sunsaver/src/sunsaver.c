//$Id: sunsaver.c,v 1.7 2011/06/21 23:11:22 wschaub Exp wschaub $
/*
 *  sunsaver.c - This program reads all the RAM registers on a Moringstar SunSaver MPPT and prints the results.
 *  

Copyright 2010 Tom Rinehart.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/* Modified for Genesi by William Schaub <wschaub@steubentech.com> */
/* Compile with: cc sunsaver.c -o sunsaver -lmodbus */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <modbus/modbus.h>

#define SUNSAVERMPPT    0x01	/* MODBUS Address of the SunSaver MPPT */
#define MAXSIZE 100 /* Not used right now */
#define INTERVAL 900 /* interval in seconds to poll the sunsaver for data */

/* Write out sunsaver state information to a text file every INTERVAL seconds */
int main(int argc, char **argv)
{
    FILE *fp;
    int opt;
    int foreground = 0; 

    while ((opt = getopt(argc,argv,"f")) != -1) {
        switch(opt) {
            case 'f':
                foreground = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [ -f ] /path/to/logfile.csv\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }
        if(optind >= argc) {
            fprintf(stderr,"Usage: %s [ -f ] /path/to/logfile.csv\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }

    if(!foreground){
        printf("forking into background\n");
        daemon(0,0);
    }

     while(1){
        fp = fopen(argv[1],"a");
        if(fp == NULL) {
            perror(argv[1]);
            exit(1);
        }
        write_log(fp);
        fflush(fp);
        fclose(fp);
        sleep(INTERVAL);
    }
} 

/* Open the serial device , grab the data off modbus and write a record to the log file */
int write_log(FILE *fp) 
{
    time_t timestamp;
	modbus_param_t mb_param;
	int ret;
	float adc_vb_f,adc_va_f,adc_vl_f,adc_ic_f,adc_il_f, Vb_f, Vb_ref, Ahc_r, Ahc_t, kWhc;
	float V_lvd, Ahl_r, Ahl_t, Power_out, Sweep_Vmp, Sweep_Pmax, Sweep_Voc, Vb_min_daily;
	float Vb_max_daily, Ahc_daily, Ahl_daily, vb_min, vb_max;
	short T_hs, T_batt, T_amb, T_rts;
	unsigned short charge_state, load_state, led_state;
	unsigned int hourmeter;
	unsigned short array_fault, load_fault, dip_switch, array_fault_daily, load_fault_daily;
	unsigned int alarm, alarm_daily;
	uint16_t data[50];
    char charge_state_text[100];
    char array_fault_text[100];
    char load_fault_text[100];
	
	/* Setup the serial port parameters */
	modbus_init_rtu(&mb_param, "/dev/ttyUSB0", 9600, "none", 8, 2);	/* Add the appropriate path to your serial port */
	
	/* Open the MODBUS connection */
	if (modbus_connect(&mb_param) == -1) {
		fprintf(stderr,"ERROR Connection failed\n");
		return(1);
	}
	
	/* Read the RAM Registers */
	ret = read_input_registers(&mb_param, SUNSAVERMPPT, 0x0008, 45, data);
    if(ret < 0 ) {
        fprintf(stderr,"ERROR Failed to read registers\n");
        return(1);
    }
	
	/* Close the MODBUS connection */
	modbus_close(&mb_param);
    
    timestamp = time(NULL);

    /* if time is < Jan 1st 2000 don't log because the RTC is messed up */
    if(timestamp < 946702800 ) {
        fprintf(stderr,"ERROR time is not even in the 21st century refusing to log\n");
        return(1);
    }

    fprintf(fp,"%lu,",timestamp);//write timestamp field.
    fprintf(stderr,"%lu\n",timestamp);
	
	/* Convert the results to their proper values and print them out */
	//printf("RAM Registers\n\n");
	
	adc_vb_f=data[0]*100.0/32768.0;
	//printf("adc_vb_f = %.2f V\n",adc_vb_f);
	fprintf(fp,"%.2f V,",adc_vb_f);
	
	adc_va_f=data[1]*100.0/32768.0;
	//printf("adc_va_f = %.2f V\n",adc_va_f);
	fprintf(fp,"%.2f V,",adc_va_f);
	
	adc_vl_f=data[2]*100.0/32768.0;
	//printf("adc_vl_f = %.2f V\n",adc_vl_f);
	fprintf(fp,"%.2f V,",adc_vl_f);
	
	adc_ic_f=data[3]*79.16/32768.0;
	//printf("adc_ic_f = %.2f A\n",adc_ic_f);
	fprintf(fp,"%.2f A,",adc_ic_f);
	
	adc_il_f=data[4]*79.16/32768.0;
	//printf("adc_il_f = %.2f A\n",adc_il_f);
	fprintf(fp,"%.2f A,",adc_il_f);
	
	T_hs=data[5];
	//printf("T_hs = %d °C\n",T_hs);
	fprintf(fp,"%d &deg;C,",T_hs);
	
	T_batt=data[6];
	//printf("T_batt = %d °C\n",T_batt);
	fprintf(fp,"%d &deg;C,",T_batt);
	
	T_amb=data[7];
	//printf("T_amb = %d °C\n",T_amb);
	fprintf(fp,"%d &deg;C,",T_amb);
	
	T_rts=data[8];
	//printf("T_rts = %d °C\n",T_rts);
	fprintf(fp,"%d &deg;C,",T_rts);
	
	charge_state=data[9];
	switch (charge_state) {
		case 0:
			//printf("charge_state = %d START\n",charge_state);
			fprintf(fp,"%d START,",charge_state);
			break;
		case 1:
			//printf("charge_state = %d NIGHT_CHECK\n",charge_state);
			fprintf(fp,"%d NIGHT_CHECK,",charge_state);
			break;
		case 2:
			//printf("charge_state = %d DISCONNECT\n",charge_state);
			fprintf(fp,"%d DISCONNECT,",charge_state);
			break;
		case 3:
			//printf("charge_state = %d NIGHT\n",charge_state);
			fprintf(fp,"%d NIGHT,",charge_state);
			break;
		case 4:
			//printf("charge_state = %d FAULT\n",charge_state);
			fprintf(fp,"%d FAULT,",charge_state);
			break;
		case 5:
			//printf("charge_state = %d BULK_CHARGE\n",charge_state);
			fprintf(fp,"%d BULK_CHARGE,",charge_state);
			break;
		case 6:
			//printf("charge_state = %d ABSORPTION\n",charge_state);
			fprintf(fp,"%d ABSORPTION,",charge_state);
			break;
		case 7:
			//printf("charge_state = %d FLOAT\n",charge_state);
			fprintf(fp,"%d FLOAT,",charge_state);
			break;
		case 8:
			//printf("charge_state = %d EQUALIZE\n",charge_state);
			fprintf(fp,"%d EQUALIZE,",charge_state);
			break;
            fprintf(fp,",");
	}
	
	array_fault=data[10];
	//printf("array_fault = Solar input self-diagnostic faults:\n");
	if (array_fault == 0) {
		//printf("\tNo faults\n");
		fprintf(fp,"No faults,");
	} else {
		//if (array_fault & 1) printf("\tOvercurrent\n");
		if (array_fault & 1) fprintf(fp,"Overcurrent:");
		//if ((array_fault & (1 << 1)) >> 1) printf("\tFETs shorted\n");
		if ((array_fault & (1 << 1)) >> 1) fprintf(fp,"FETs shorted:");
		//if ((array_fault & (1 << 2)) >> 2) printf("\tSoftware bug\n");
		if ((array_fault & (1 << 2)) >> 2) fprintf(fp,"Software bug:");
		//if ((array_fault & (1 << 3)) >> 3) printf("\tBattery HVD\n");
		if ((array_fault & (1 << 3)) >> 3) fprintf(fp,"Battery HVD:");
		//if ((array_fault & (1 << 4)) >> 4) printf("\tArray HVD\n");
		if ((array_fault & (1 << 4)) >> 4) fprintf(fp,"Array HVD:");
		//if ((array_fault & (1 << 5)) >> 5) printf("\tEEPROM setting edit (reset required)\n");
		if ((array_fault & (1 << 5)) >> 5) fprintf(fp,"EEPROM setting edit (reset required):");
		//if ((array_fault & (1 << 6)) >> 6) printf("\tRTS shorted\n");
		if ((array_fault & (1 << 6)) >> 6) fprintf(fp,"RTS shorted:");
		//if ((array_fault & (1 << 7)) >> 7) printf("\tRTS was valid, now disconnected\n");
		if ((array_fault & (1 << 7)) >> 7) fprintf(fp,"RTS was valid now disconnected:");
		//if ((array_fault & (1 << 8)) >> 8) printf("\tLocal temperature sensor failed\n");
		if ((array_fault & (1 << 8)) >> 8) fprintf(fp,"Local temperature sensor failed:");
		//if ((array_fault & (1 << 9)) >> 9) printf("\tFault 10\n");
		if ((array_fault & (1 << 9)) >> 9) fprintf(fp,"Fault 10:");
		//if ((array_fault & (1 << 10)) >> 10) printf("\tFault 11\n");
		if ((array_fault & (1 << 10)) >> 10) fprintf(fp,"Fault 11:");
		//if ((array_fault & (1 << 11)) >> 11) printf("\tFault 12\n");
		if ((array_fault & (1 << 11)) >> 11) fprintf(fp,"Fault 12:");
		//if ((array_fault & (1 << 12)) >> 12) printf("\tFault 13\n");
		if ((array_fault & (1 << 12)) >> 12) fprintf(fp,"Fault 13:");
		//if ((array_fault & (1 << 13)) >> 13) printf("\tFault 14\n");
		if ((array_fault & (1 << 13)) >> 13) fprintf(fp,"Fault 14:");
		//if ((array_fault & (1 << 14)) >> 14) printf("\tFault 15\n");
		if ((array_fault & (1 << 14)) >> 14) fprintf(fp,"Fault 15:");
		//if ((array_fault & (1 << 15)) >> 15) printf("\tFault 16\n");
		if ((array_fault & (1 << 15)) >> 15) fprintf(fp,"Fault 16");
        fprintf(fp,",");
	}
	
	Vb_f=data[11]*100.0/32768.0;
	//printf("Vb_f = %.2f V\n",Vb_f);
	fprintf(fp,"%.2f V,",Vb_f);
	
	Vb_ref=data[12]*96.667/32768.0;
	//printf("Vb_ref = %.2f V\n",Vb_ref);
	fprintf(fp,"%.2f V,",Vb_ref);
	
	Ahc_r=((data[13] << 16) + data[14])*0.1;
	//printf("Ahc_r = %.2f Ah\n",Ahc_r);
	fprintf(fp,"%.2f Ah,",Ahc_r);
	
	Ahc_t=((data[15] << 16) + data[16])*0.1;
	//printf("Ahc_t = %.2f Ah\n",Ahc_t);
	fprintf(fp,"%.2f Ah,",Ahc_t);
	
	kWhc=data[17]*0.1;
	//printf("kWhc = %.2f kWh\n",kWhc);
	fprintf(fp,"%.2f kWh,",kWhc);
	
	load_state=data[18];
	switch (load_state) {
		case 0:
			//printf("load_state = %d START\n",load_state);
			fprintf(fp,"%d START,",load_state);
			break;
		case 1:
			//printf("load_state = %d LOAD_ON\n",load_state);
			fprintf(fp,"%d LOAD_ON,",load_state);
			break;
		case 2:
			//printf("load_state = %d LVD_WARNING\n",load_state);
			fprintf(fp,"%d LVD_WARNING,",load_state);
			break;
		case 3:
			//printf("load_state = %d LVD\n",load_state);
			fprintf(fp,"%d LVD,",load_state);
			break;
		case 4:
			//printf("load_state = %d FAULT\n",load_state);
			fprintf(fp,"%d FAULT,",load_state);
			break;
		case 5:
			//printf("load_state = %d DISCONNECT\n",load_state);
			fprintf(fp,"%d DISCONNECT,",load_state);
			break;
	}
	
	load_fault=data[19];
	//printf("load_fault = Load output self-diagnostic faults:\n");
	if (load_fault == 0) {
		//printf("\tNo faults\n");
		fprintf(fp,"No faults,");
	} else {
		//if (load_fault & 1) printf("\tExternal short circuit\n");
		if (load_fault & 1) fprintf(fp,"External short circuit:");
		//if ((load_fault & (1 << 1)) >> 1) printf("\tOvercurrent\n");
		if ((load_fault & (1 << 1)) >> 1) fprintf(fp,"Overcurrent:");
		//if ((load_fault & (1 << 2)) >> 2) printf("\tFETs shorted\n");
		if ((load_fault & (1 << 2)) >> 2) fprintf(fp,"FETs shorted:");
		//if ((load_fault & (1 << 3)) >> 3) printf("\tSoftware bug\n");
		if ((load_fault & (1 << 3)) >> 3) fprintf(fp,"Software bug:");
		//if ((load_fault & (1 << 4)) >> 4) printf("\tHVD\n");
		if ((load_fault & (1 << 4)) >> 4) fprintf(fp,"HVD:");
		//if ((load_fault & (1 << 5)) >> 5) printf("\tHeatsink over-temperature\n");
		if ((load_fault & (1 << 5)) >> 5) fprintf(fp,"Heatsink over-temperature:");
		//if ((load_fault & (1 << 6)) >> 6) printf("\tEEPROM setting edit (reset required)\n");
		if ((load_fault & (1 << 6)) >> 6) fprintf(fp,"EEPROM setting edit (reset required):");
		//if ((load_fault & (1 << 7)) >> 7) printf("\tFault 8\n");
		if ((load_fault & (1 << 7)) >> 7) fprintf(fp,"Fault 8");
        fprintf(fp,",");
	}
	
	V_lvd=data[20]*100.0/32768.0;
	//printf("V_lvd = %.2f V\n",V_lvd);
	fprintf(fp,"%.2f V,",V_lvd);
	
	Ahl_r=((data[21] << 16) + data[22])*0.1;
	//printf("Ahl_r = %.2f Ah\n",Ahl_r);
	fprintf(fp,"%.2f Ah,",Ahl_r);
	
	Ahl_t=((data[23] << 16) + data[24])*0.1;
	//printf("Ahl_t = %.2f Ah\n",Ahl_t);
	fprintf(fp,"%.2f Ah,",Ahl_t);
	
	hourmeter=(data[25] << 16) + data[26];
	//printf("hourmeter = %d h\n",hourmeter);
	fprintf(fp,"%d h,",hourmeter);
	
	alarm=(data[27] << 16) + data[28];
	//printf("alarm = Controller self-diagnostic alarms:\n");
	if (alarm == 0) {
		//printf("\tNo alarms\n");
		fprintf(fp,"No alarms,");
	} else {
		//if (alarm & 1) printf("\tRTS open\n");
		if (alarm & 1) fprintf(fp,"RTS open:");
		//if ((alarm & (1 << 1)) >> 1) printf("\tRTS shorted\n");
		if ((alarm & (1 << 1)) >> 1) fprintf(fp,"RTS shorted:");
		//if ((alarm & (1 << 2)) >> 2) printf("\tRTS disconnected\n");
		if ((alarm & (1 << 2)) >> 2) fprintf(fp,"RTS disconnected:");
		//if ((alarm & (1 << 3)) >> 3) printf("\tThs open\n");
		if ((alarm & (1 << 3)) >> 3) fprintf(fp,"Ths open:");
		//if ((alarm & (1 << 4)) >> 4) printf("\tThs shorted\n");
		if ((alarm & (1 << 4)) >> 4) fprintf(fp,"Ths shorted:");
		//if ((alarm & (1 << 5)) >> 5) printf("\tSSMPPT hot\n");
		if ((alarm & (1 << 5)) >> 5) fprintf(fp,"SSMPPT hot:");
		//if ((alarm & (1 << 6)) >> 6) printf("\tCurrent limit\n");
		if ((alarm & (1 << 6)) >> 6) fprintf(fp,"Current limit:");
		//if ((alarm & (1 << 7)) >> 7) printf("\tCurrent offset\n");
		if ((alarm & (1 << 7)) >> 7) fprintf(fp,"Current offset:");
		//if ((alarm & (1 << 8)) >> 8) printf("\tUndefined\n");
		if ((alarm & (1 << 8)) >> 8) fprintf(fp,"8 Undefined:");
		//if ((alarm & (1 << 9)) >> 9) printf("\tUndefined\n");
		if ((alarm & (1 << 9)) >> 9) fprintf(fp,"9 Undefined:");
		//if ((alarm & (1 << 10)) >> 10) printf("\tUncalibrated\n");
		if ((alarm & (1 << 10)) >> 10) fprintf(fp,"Uncalibrated:");
		//if ((alarm & (1 << 11)) >> 11) printf("\tRTS miswire\n");
		if ((alarm & (1 << 11)) >> 11) fprintf(fp,"RTS miswire:");
		//if ((alarm & (1 << 12)) >> 12) printf("\tUndefined\n");
		if ((alarm & (1 << 12)) >> 12) fprintf(fp,"12 Undefined:");
		//if ((alarm & (1 << 13)) >> 13) printf("\tUndefined\n");
		if ((alarm & (1 << 13)) >> 13) fprintf(fp,"13 Undefined:");
		//if ((alarm & (1 << 14)) >> 14) printf("\tMiswire\n");
		if ((alarm & (1 << 14)) >> 14) fprintf(fp,"Miswire:");
		//if ((alarm & (1 << 15)) >> 15) printf("\tFET open\n");
		if ((alarm & (1 << 15)) >> 15) fprintf(fp,"FET open:");
		//if ((alarm & (1 << 16)) >> 16) printf("\tP12\n");
		if ((alarm & (1 << 16)) >> 16) fprintf(fp,"P12:");
		//if ((alarm & (1 << 17)) >> 17) printf("\tHigh Va current limit\n");
		if ((alarm & (1 << 17)) >> 17) fprintf(fp,"High Va current limit:");
		//if ((alarm & (1 << 18)) >> 18) printf("\tAlarm 19\n");
		if ((alarm & (1 << 18)) >> 18) fprintf(fp,"Alarm 19:");
		//if ((alarm & (1 << 19)) >> 19) printf("\tAlarm 20\n");
		if ((alarm & (1 << 19)) >> 19) fprintf(fp,"Alarm 20:");
		//if ((alarm & (1 << 20)) >> 20) printf("\tAlarm 21\n");
		if ((alarm & (1 << 20)) >> 20) fprintf(fp,"Alarm 21:");
		//if ((alarm & (1 << 21)) >> 21) printf("\tAlarm 22\n");
		if ((alarm & (1 << 21)) >> 21) fprintf(fp,"Alarm 22:");
		//if ((alarm & (1 << 22)) >> 22) printf("\tAlarm 23\n");
		if ((alarm & (1 << 22)) >> 22) fprintf(fp,"Alarm 23:");
		//if ((alarm & (1 << 23)) >> 23) printf("\tAlarm 24\n");
		if ((alarm & (1 << 23)) >> 23) fprintf(fp,"Alarm 24");
        fprintf(fp,",");
	}
	
	dip_switch=data[29];
	//printf("dip_switch = DIP switch settings:\n");
	if (dip_switch & 1) {
		//printf("\tSwitch 1 ON - Battery Type: Gel or AGM\n");
		fprintf(fp,"Switch 1 ON - Battery Type: Gel or AGM,");
	} else {
		//printf("\tSwitch 1 OFF - Battery Type: Sealed or Flooded\n");
		fprintf(fp,"Switch 1 OFF - Battery Type: Sealed or Flooded,");
	}
	if ((dip_switch & (1 << 1)) >> 1) {
		//printf("\tSwitch 2 ON - LVD = 11.00 V, LVR = 12.10 V or custom load settings\n");
		fprintf(fp,"Switch 2 ON - LVD = 11.00 V LVR = 12.10 V or custom load settings,");
	} else {
		//printf("\tSwitch 2 OFF - LVD = 11.50 V, LVR = 12.60 V\n");
		fprintf(fp,"Switch 2 OFF - LVD = 11.50 V LVR = 12.60 V,");
	}
	if ((dip_switch & (1 << 2)) >> 2) {
		//printf("\tSwitch 3 ON - Auto-Equalize On\n");
		fprintf(fp,"Switch 3 ON - Auto-Equalize On,");
	} else {
		//printf("\tSwitch 3 OFF - Auto-Equalize Off\n");
		fprintf(fp,"Switch 3 OFF - Auto-Equalize Off,");
	}
	if ((dip_switch & (1 << 3)) >> 3) {
		//printf("\tSwitch 4 ON - MODBUS Protocol\n");
		fprintf(fp,"Switch 4 ON - MODBUS Protocol,");
	} else {
		//printf("\tSwitch 4 OFF - Meterbus Protocol\n");
		fprintf(fp,"Switch 4 OFF - Meterbus Protocol,");
	}
	
	led_state=data[30];
	switch (led_state) {
		case 0:
			//printf("led_state = %d LED_START\n",led_state);
			fprintf(fp,"%d LED_START,",led_state);
			break;
		case 1:
			//printf("led_state = %d LED_START2\n",led_state);
			fprintf(fp,"%d LED_START2,",led_state);
			break;
		case 2:
			//printf("led_state = %d LED_BRANCH\n",led_state);
			fprintf(fp,"%d LED_BRANCH,",led_state);
			break;
		case 3:
			//printf("led_state = %d EQUALIZE (FAST GREEN BLINK)\n",led_state);
			fprintf(fp,"%d EQUALIZE (FAST GREEN BLINK),",led_state);
			break;
		case 4:
			//printf("led_state = %d FLOAT (SLOW GREEN BLINK)\n",led_state);
			fprintf(fp,"%d FLOAT (SLOW GREEN BLINK),",led_state);
			break;
		case 5:
			//printf("led_state = %d ABSORPTION (GREEN BLINK, 1HZ)\n",led_state);
			fprintf(fp,"led_state = %d ABSORPTION (GREEN BLINK 1HZ),",led_state);
			break;
		case 6:
			//printf("led_state = %d GREEN_LED\n",led_state);
			fprintf(fp,"%d GREEN_LED,",led_state);
			break;
		case 7:
			//printf("led_state = %d UNDEFINED\n",led_state);
			fprintf(fp,"%d UNDEFINED,",led_state);
			break;
		case 8:
			//printf("led_state = %d YELLOW_LED\n",led_state);
			fprintf(fp,"%d YELLOW_LED,",led_state);
			break;
		case 9:
			fprintf(fp,"%d UNDEFINED,",led_state);
			break;
		case 10:
			//printf("led_state = %d BLINK_RED_LED\n",led_state);
			fprintf(fp,"%d BLINK_RED_LED,",led_state);
			break;
		case 11:
			//printf("led_state = %d RED_LED\n",led_state);
			fprintf(fp,"%d RED_LED,",led_state);
			break;
		case 12:
			//printf("led_state = %d R-Y-G ERROR\n",led_state);
			fprintf(fp,"%d R-Y-G ERROR,",led_state);
			break;
		case 13:
			//printf("led_state = %d R/Y-G ERROR\n",led_state);
			fprintf(fp,"%d R/Y-G ERROR,",led_state);
			break;
		case 14:
			//printf("led_state = %d R/G-Y ERROR\n",led_state);
			fprintf(fp,"led_state = %d R/G-Y ERROR,",led_state);
			break;
		case 15:
			//printf("led_state = %d R-Y ERROR (HTD)\n",led_state);
			fprintf(fp,"%d R-Y ERROR (HTD),",led_state);
			break;
		case 16:
			//printf("led_state = %d R-G ERROR (HVD)\n",led_state);
			fprintf(fp,"%d R-G ERROR (HVD),",led_state);
			break;
		case 17:
			//printf("led_state = %d R/Y-G/Y ERROR\n",led_state);
			fprintf(fp,"%d R/Y-G/Y ERROR,",led_state);
			break;
		case 18:
			//printf("led_state = %d G/Y/R ERROR\n",led_state);
			fprintf(fp,"%d G/Y/R ERROR,",led_state);
			break;
		case 19:
			//printf("led_state = %d G/Y/R x 2\n",led_state);
			fprintf(fp,"led_state = %d G/Y/R x 2,",led_state);
			break;
	}
	
	Power_out=data[31]*989.5/65536.0;
	//printf("Power_out = %.2f W\n",Power_out);
	fprintf(fp,"%.2f W,",Power_out);
	
	Sweep_Vmp=data[32]*100.0/32768.0;
	//printf("Sweep_Vmp = %.2f V\n",Sweep_Vmp);
	fprintf(fp,"%.2f V,",Sweep_Vmp);
	
	Sweep_Pmax=data[33]*989.5/65536.0;
	//printf("Sweep_Pmax = %.2f W\n",Sweep_Pmax);
	fprintf(fp,"%.2f W,",Sweep_Pmax);
	
	Sweep_Voc=data[34]*100.0/32768.0;
	//printf("Sweep_Voc = %.2f V\n",Sweep_Voc);
	fprintf(fp,"%.2f V,",Sweep_Voc);
	
	Vb_min_daily=data[35]*100.0/32768.0;
	//printf("Vb_min_daily = %.2f V\n",Vb_min_daily);
	fprintf(fp,"%.2f V,",Vb_min_daily);
	
	Vb_max_daily=data[36]*100.0/32768.0;
	//printf("Vb_max_daily = %.2f V\n",Vb_max_daily);
	fprintf(fp,"%.2f V,",Vb_max_daily);
	
	Ahc_daily=data[37]*0.1;
	//printf("Ahc_daily = %.2f Ah\n",Ahc_daily);
	fprintf(fp,"%.2f Ah,",Ahc_daily);
	
	Ahl_daily=data[38]*0.1;
	//printf("Ahl_daily = %.2f Ah\n",Ahl_daily);
	fprintf(fp,"%.2f Ah,",Ahl_daily);
	
	array_fault_daily=data[39];
	//printf("array_fault_daily = Today's solar input self-diagnostic faults:\n");
	if (array_fault_daily == 0) {
		//printf("\tNo faults\n");
		fprintf(fp,"No faults,");
	} else {
		//if (array_fault_daily & 1) printf("\tOvercurrent\n");
		if (array_fault_daily & 1) fprintf(fp,"Overcurrent:");
		//if ((array_fault_daily & (1 << 1)) >> 1) printf("\tFETs shorted\n");
		if ((array_fault_daily & (1 << 1)) >> 1) fprintf(fp,"FETs shorted:");
		//if ((array_fault_daily & (1 << 2)) >> 2) printf("\tSoftware bug\n");
		if ((array_fault_daily & (1 << 2)) >> 2) fprintf(fp,"Software bug:");
		//if ((array_fault_daily & (1 << 3)) >> 3) printf("\tBattery HVD\n");
		if ((array_fault_daily & (1 << 3)) >> 3) fprintf(fp,"Battery HVD:");
		//if ((array_fault_daily & (1 << 4)) >> 4) printf("\tArray HVD\n");
		if ((array_fault_daily & (1 << 4)) >> 4) fprintf(fp,"Array HVD:");
		//if ((array_fault_daily & (1 << 5)) >> 5) printf("\tEEPROM setting edit (reset required)\n");
		if ((array_fault_daily & (1 << 5)) >> 5) fprintf(fp,"EEPROM setting edit (reset required):");
		//if ((array_fault_daily & (1 << 6)) >> 6) printf("\tRTS shorted\n");
		if ((array_fault_daily & (1 << 6)) >> 6) fprintf(fp,"RTS shorted:");
		//if ((array_fault_daily & (1 << 7)) >> 7) printf("\tRTS was valid, now disconnected\n");
		if ((array_fault_daily & (1 << 7)) >> 7) fprintf(fp,"RTS was valid now disconnected:");
		//if ((array_fault_daily & (1 << 8)) >> 8) printf("\tLocal temperature sensor failed\n");
		if ((array_fault_daily & (1 << 8)) >> 8) fprintf(fp,"Local temperature sensor failed:");
		//if ((array_fault_daily & (1 << 9)) >> 9) printf("\tFault 10\n");
		if ((array_fault_daily & (1 << 9)) >> 9) fprintf(fp,"Fault 10:");
		//if ((array_fault_daily & (1 << 10)) >> 10) printf("\tFault 11\n");
		if ((array_fault_daily & (1 << 10)) >> 10) fprintf(fp,"Fault 11:");
		//if ((array_fault_daily & (1 << 11)) >> 11) printf("\tFault 12\n");
		if ((array_fault_daily & (1 << 11)) >> 11) fprintf(fp,"Fault 12:");
		//if ((array_fault_daily & (1 << 12)) >> 12) printf("\tFault 13\n");
		if ((array_fault_daily & (1 << 12)) >> 12) fprintf(fp,"Fault 13:");
		//if ((array_fault_daily & (1 << 13)) >> 13) printf("\tFault 14\n");
		if ((array_fault_daily & (1 << 13)) >> 13) fprintf(fp,"\tFault 14\n");
		//if ((array_fault_daily & (1 << 14)) >> 14) printf("\tFault 15\n");
		if ((array_fault_daily & (1 << 14)) >> 14) fprintf(fp,"Fault 15:");
		//if ((array_fault_daily & (1 << 15)) >> 15) printf("\tFault 16\n");
		if ((array_fault_daily & (1 << 15)) >> 15) fprintf(fp,"\tFault 16");
        fprintf(fp,",");
	}
	
	load_fault_daily=data[40];
	//printf("load_fault_daily = Today's load output self-diagnostic faults:\n");
	if (load_fault_daily == 0) {
		//printf("\tNo faults\n");
		fprintf(fp,"No faults,");
	} else {
		//if (load_fault_daily & 1) printf("\tExternal short circuit\n");
		if (load_fault_daily & 1) fprintf(fp,"External short circuit:");
		//if ((load_fault_daily & (1 << 1)) >> 1) printf("\tOvercurrent\n");
		if ((load_fault_daily & (1 << 1)) >> 1) fprintf(fp,"Overcurrent:");
		//if ((load_fault_daily & (1 << 2)) >> 2) printf("\tFETs shorted\n");
		if ((load_fault_daily & (1 << 2)) >> 2) fprintf(fp,"FETs shorted:");
		//if ((load_fault_daily & (1 << 3)) >> 3) printf("\tSoftware bug\n");
		if ((load_fault_daily & (1 << 3)) >> 3) fprintf(fp,"Software bug:");
		//if ((load_fault_daily & (1 << 4)) >> 4) printf("\tHVD\n");
		if ((load_fault_daily & (1 << 4)) >> 4) fprintf(fp,"HVD:");
		//if ((load_fault_daily & (1 << 5)) >> 5) printf("\tHeatsink over-temperature\n");
		if ((load_fault_daily & (1 << 5)) >> 5) fprintf(fp,"Heatsink over-temperature:");
		//if ((load_fault_daily & (1 << 6)) >> 6) printf("\tEEPROM setting edit (reset required)\n");
		if ((load_fault_daily & (1 << 6)) >> 6) fprintf(fp,"EEPROM setting edit (reset required):");
		//if ((load_fault_daily & (1 << 7)) >> 7) printf("\tFault 8\n");
		if ((load_fault_daily & (1 << 7)) >> 7) fprintf(fp,"Fault 8:");
        fprintf(fp,",");
	}
	
	alarm_daily=(data[41] << 16) + data[42];
	//printf("alarm_daily = Today's controller self-diagnostic alarms:\n");
	if (alarm_daily == 0) {
		//printf("\tNo alarms\n");
		fprintf(fp,"No alarms,");
	} else { /* XXX! these alarms look like they are the same, we should split this out into a function call */
		//if (alarm_daily & 1) printf("\tRTS open\n");
		if (alarm_daily & 1) fprintf(fp,"RTS open:");
		//if ((alarm_daily & (1 << 1)) >> 1) printf("\tRTS shorted\n");
		if ((alarm_daily & (1 << 1)) >> 1) fprintf(fp,"RTS shorted:");
		//if ((alarm_daily & (1 << 2)) >> 2) printf("\tRTS disconnected\n");
		if ((alarm_daily & (1 << 2)) >> 2) fprintf(fp,"RTS disconnected:");
		//if ((alarm_daily & (1 << 3)) >> 3) printf("\tThs open\n");
		if ((alarm_daily & (1 << 3)) >> 3) fprintf(fp,"Ths open:");
		//if ((alarm_daily & (1 << 4)) >> 4) printf("\tThs shorted\n");
		if ((alarm_daily & (1 << 4)) >> 4) fprintf(fp,"Ths shorted:");
		//if ((alarm_daily & (1 << 5)) >> 5) printf("\tSSMPPT hot\n");
		if ((alarm_daily & (1 << 5)) >> 5) fprintf(fp,"SSMPPT hot:");
		//if ((alarm_daily & (1 << 6)) >> 6) printf("\tCurrent limit\n");
		if ((alarm_daily & (1 << 6)) >> 6) fprintf(fp,"Current limit:");
		//if ((alarm_daily & (1 << 7)) >> 7) printf("\tCurrent offset\n");
		if ((alarm_daily & (1 << 7)) >> 7) fprintf(fp,"Current offset:");
		//if ((alarm_daily & (1 << 8)) >> 8) printf("\tUndefined\n");
		if ((alarm_daily & (1 << 8)) >> 8) fprintf(fp,"8 Undefined:");
		//if ((alarm_daily & (1 << 9)) >> 9) printf("\tUndefined\n");
		if ((alarm_daily & (1 << 9)) >> 9) fprintf(fp,"9 Undefined:");
		//if ((alarm_daily & (1 << 10)) >> 10) printf("\tUncalibrated\n");
		if ((alarm_daily & (1 << 10)) >> 10) fprintf(fp,"Uncalibrated:");
		//if ((alarm_daily & (1 << 11)) >> 11) printf("\tRTS miswire\n");
		if ((alarm_daily & (1 << 11)) >> 11) fprintf(fp,"RTS miswire:");
		//if ((alarm_daily & (1 << 12)) >> 12) printf("\tUndefined\n");
		if ((alarm_daily & (1 << 12)) >> 12) fprintf(fp,"12 Undefined:");
		//if ((alarm_daily & (1 << 13)) >> 13) printf("\tUndefined\n");
		if ((alarm_daily & (1 << 13)) >> 13) fprintf(fp,"13 Undefined:");
		//if ((alarm_daily & (1 << 14)) >> 14) printf("\tMiswire\n");
		if ((alarm_daily & (1 << 14)) >> 14) fprintf(fp,"Miswire:");
		//if ((alarm_daily & (1 << 15)) >> 15) printf("\tFET open\n");
		if ((alarm_daily & (1 << 15)) >> 15) fprintf(fp,"FET open:");
		//if ((alarm_daily & (1 << 16)) >> 16) printf("\tP12\n");
		if ((alarm_daily & (1 << 16)) >> 16) fprintf(fp,"P12:");
		//if ((alarm_daily & (1 << 17)) >> 17) printf("\tHigh Va current limit\n");
		if ((alarm_daily & (1 << 17)) >> 17) fprintf(fp,"High Va current limit:");
		//if ((alarm_daily & (1 << 18)) >> 18) printf("\tAlarm 19\n");
		if ((alarm_daily & (1 << 18)) >> 18) fprintf(fp,"Alarm 19:");
		//if ((alarm_daily & (1 << 19)) >> 19) printf("\tAlarm 20\n");
		if ((alarm_daily & (1 << 19)) >> 19) fprintf(fp,"Alarm 20:");
		//if ((alarm_daily & (1 << 20)) >> 20) printf("\tAlarm 21\n");
		if ((alarm_daily & (1 << 20)) >> 20) fprintf(fp,"Alarm 21:");
		//if ((alarm_daily & (1 << 21)) >> 21) printf("\tAlarm 22\n");
		if ((alarm_daily & (1 << 21)) >> 21) fprintf(fp,"Alarm 22:");
		//if ((alarm_daily & (1 << 22)) >> 22) printf("\tAlarm 23\n");
		if ((alarm_daily & (1 << 22)) >> 22) fprintf(fp,"Alarm 23:");
		//if ((alarm_daily & (1 << 23)) >> 23) printf("\tAlarm 24\n");
		if ((alarm_daily & (1 << 23)) >> 23) fprintf(fp,"Alarm 24");
        fprintf(fp,",");
	}
	
	vb_min=data[43]*100.0/32768.0;
	//printf("vb_min = %.2f V\n",vb_min);
	fprintf(fp,"%.2f V,",vb_min);
	
	vb_max=data[44]*100.0/32768.0;
	//printf("vb_max = %.2f V\n",vb_max);
	fprintf(fp,"%.2f V",vb_max);
    fprintf(fp,"\n");
	
	return(0);
}
/*$Log: sunsaver.c,v $
 *Revision 1.7  2011/06/21 23:11:22  wschaub
 *replace ASCII degree symbol with the HTML equiv of the same.
 *
 *Revision 1.6  2011/06/21 00:52:13  wschaub
 *Open/close the file every time we write to it, this should allow logrotate to do its thing
 *correctly.
 *
 *Revision 1.5  2011/06/21 00:29:26  wschaub
 *move timestamp fprintf to after the part that checks if the modbus was read successfully or not.
 *this keeps us from fucking up the log file with empty records.
 *
 *Revision 1.4  2011/06/20 21:33:19  wschaub
 *Changed the interval to 15 minutes and redirected the debug printf to stderr
 *
 *Revision 1.3  2011/06/20 21:03:13  wschaub
 *fixed fprintf that added an extra comma
 *
 *Revision 1.2  2011/06/20 06:36:22  wschaub
 *modified to write to a CSV file instead of stdout.
 **/
