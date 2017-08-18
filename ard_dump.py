#!/usr/bin/env python3

from ard_core import *
import serial

# program = "<KBD_AUTORELEASE_OFF><KEY_LEFT_GUI> <KBD_RELEASE_ALL><CMD_DELAY_TEN>" + chr(10) +\
#           "<KBD_AUTORELEASE_ON>ou<KEY_RETURN><CMD_DELAY_TEN>" + chr(25) +\
#           "<KBD_AUTORELEASE_OFF><KEY_LEFT_GUI>n<KBD_RELEASE_ALL><CMD_DELAY_TEN>" + chr(40) +\
#           "<KBD_AUTORELEASE_ON>aws-crypto-i<KEY_RETURN><CMD_DELAY_TEN>" + chr(30) +\
#           "<KEY_RETURN><KEY_RETURN><CMD_DELAY_TEN>" + chr(30) +\
#           "I love unicorns!<CMD_DELAY_TEN>" + chr(40) +\
#           "<KEY_RETURN>I really, really love unicorns!\n\n-Someone who forgot to lock their computer\n" +\
#           "<KBD_AUTORELEASE_OFF><KEY_LEFT_GUI><KEY_RETURN><KBD_RELEASE_ALL>"

# command = convert_command_string(program)

# dump_command_bytes(command)

with serial.Serial(find_port(), 9600) as ser:
    # ser.write(MODIFIERS_BY_NAME['CMD_PROGRAM'])
    # ser.write(command)
    # ser.write(MODIFIERS_BY_NAME['CMD_END'])
    ser.write(bytearray([MODIFIERS_BY_NAME['CMD_DUMP']]))
    bytes_read = []
    b = MODIFIERS_BY_NAME['CMD_BEGIN']
    while b != MODIFIERS_BY_NAME['CMD_END']:
        b = ser.read(1)
        # if b != bytes([MODIFIERS_BY_NAME['CMD_NOOP']]):
        #     print(b)
        #     pass
        
        bytes_read.append(b)
        pass
    dump_command_bytes(bytes_read)
    pass
