#!/bin/bash

java -jar /home/pi/NetBeansProjects/mainProject/dist/mainProject.jar
sleep 1
/home/pi/NetBeansProjects/mainProject/pythonScripts/Pi_PIC_UART_Connect.py
sleep 1
/home/pi/NetBeansProjects/mainProject/pythonScripts/ADC_MainsVerify.py
sleep 1
/home/pi/NetBeansProjects/mainProject/pythonScripts/SendTestParam.py
sleep 1 
java -jar /home/pi/NetBeansProjects/mainProject/dist/DatabaseInput.jar
