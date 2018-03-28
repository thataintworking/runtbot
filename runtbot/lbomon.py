#!/usr/bin/env python3

# lbomon: Low Battery Output Monitor
#
# Checks the status of PIN 12 (BCM 18) every 5 seconds.
#   HIGH means the power board is reporting a low battery status.
#   LOW means the battery status is good.
# After 3 HIGH values (low battery) in a row, a shutdown is initiated with a 1 minute delay.
# If the script detects 3 LOW values (good battery) in a row during the shutdown delay, it cancels the shutdown.
#
# NOTE: This script should be run as root or some other priviledged user that can execute a shutdown
#
# TODO: accept parameters for the sleep time, status counts, and shutdown delay

import os
import RPi.GPIO as GPIO
from time import sleep

LBO_PIN = 25

GPIO.setmode(GPIO.BCM)
GPIO.setup(LBO_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

lbo_count = 0

shutdown_pending = False

print("Begin listening on GPIO pin", LBO_PIN)

while True:
    sleep(5)
    if not shutdown_pending:
        if GPIO.input(LBO_PIN):
            lbo_count += 1
        else:
            lbo_count = 0

        if lbo_count > 2:
            os.system('/sbin/shutdown -h +1 Low battery detected, initiating shutdown.')
            shutdown_pending = True
            lbo_count = 0

    else:
        if not GPIO.input(LBO_PIN):
            lbo_count += 1

        if lbo_count > 2:
            os.system('/sbin/shutdown -c')
            shutdown_pending = False
            lbo_count = 0
