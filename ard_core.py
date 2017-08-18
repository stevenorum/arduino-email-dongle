#!/usr/bin/env python3

import json
import serial.tools.list_ports
import serial

def dump_port_info(port):
    print(dir(port))
    attrs = ['apply_usb_info', 'description', 'device', 'hwid', 'interface', 'location', 'manufacturer', 'name', 'pid', 'product', 'serial_number', 'usb_description', 'usb_info', 'vid']
    for attr in attrs:
        if hasattr(port, attr):
            print("{attr} : {val}".format(attr=attr, val=getattr(port, attr)))
            pass
        pass
    pass

def find_port():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        # dump_port_info(p)
        if getattr(p, "description", None) and "arduino" in p.description.lower():
            return p.device
        pass
    pass

MODIFIERS_BY_NAME = {
    # Built-in Arduino modifier keys
    "KEY_LEFT_CTRL": 0x80, # 128
    "KEY_LEFT_SHIFT": 0x81, # 129
    "KEY_LEFT_ALT": 0x82, # 130
    "KEY_LEFT_GUI": 0x83, # 131
    "KEY_RIGHT_CTRL": 0x84, # 132
    "KEY_RIGHT_SHIFT": 0x85, # 133
    "KEY_RIGHT_ALT": 0x86, # 134
    "KEY_RIGHT_GUI": 0x87, # 135
    "KEY_UP_ARROW": 0xDA, # 218
    "KEY_DOWN_ARROW": 0xD9, # 217
    "KEY_LEFT_ARROW": 0xD8, # 216
    "KEY_RIGHT_ARROW": 0xD7, # 215
    "KEY_BACKSPACE": 0xB2, # 178
    "KEY_TAB": 0xB3, # 179
    "KEY_RETURN": 0xB0, # 176
    "KEY_ESC": 0xB1, # 177
    "KEY_INSERT": 0xD1, # 209
    "KEY_DELETE": 0xD4, # 212
    "KEY_PAGE_UP": 0xD3, # 211
    "KEY_PAGE_DOWN": 0xD6, # 214
    "KEY_HOME": 0xD2, # 210
    "KEY_END": 0xD5, # 213
    "KEY_CAPS_LOCK": 0xC1, # 193
    "KEY_F1": 0xC2, # 194
    "KEY_F2": 0xC3, # 195
    "KEY_F3": 0xC4, # 196
    "KEY_F4": 0xC5, # 197
    "KEY_F5": 0xC6, # 198
    "KEY_F6": 0xC7, # 199
    "KEY_F7": 0xC8, # 200
    "KEY_F8": 0xC9, # 201
    "KEY_F9": 0xCA, # 202
    "KEY_F10": 0xCB, # 203
    "KEY_F11": 0xCC, # 204
    "KEY_F12": 0xCD, # 205
    # My custom commands
    "KBD_RELEASE": 0xE0,
    "KBD_RELEASE": 0xE0,
    "KBD_RELEASE_ALL": 0xE1,
    "KBD_AUTORELEASE_ON": 0xE2,
    "KBD_AUTORELEASE_OFF": 0xE3,
    "KBD_CLICK_LENGTH": 0xE4,
    "CMD_DELAY": 0xF0,
    "CMD_DELAY_TEN": 0xF1,
    "CMD_EXEC": 0xF4,
    "CMD_PROGRAM": 0xF5,
    "CMD_DUMP": 0xF6,
    "CMD_NOOP": 0xFD,
    "CMD_BEGIN": 0xFE,
    "CMD_END": 0xFF
}
MODIFIERS_BY_BYTE = {MODIFIERS_BY_NAME[name]:name for name in MODIFIERS_BY_NAME}

def convert_command_string(command_string):
    i = 0
    command_bytes = []
    while i < len(command_string):
        if command_string[i] == '<' and (i + 5 < len(command_string)):
            next_4 = command_string[i+1:i+5]
            if next_4 in ["KBD_","CMD_","KEY_"]:
                j = i
                while j < len(command_string) and command_string[j] != '>':
                    j = j + 1
                    pass
                command = command_string[i+1:j]
                if command in MODIFIERS_BY_NAME:
                    command_bytes.append(MODIFIERS_BY_NAME[command])
                    i = j + 1
                    continue
                pass
            pass
        command_bytes.append(ord(command_string[i]))
        i += 1
        pass
    return command_bytes

def print_command(i, b, comment="", value=None):
    value = value if value else MODIFIERS_BY_BYTE.get(b, json.dumps(chr(b)))
    print("{}: 0x{} : {} {}".format(str(i).zfill(3), format(b, '02x'), value, comment))

def dump_command_bytes(command_bytes):
    i = 0
    last_b = 0xFE
    while i < len(command_bytes):
        b = command_bytes[i]
        comment = ""
        if last_b == MODIFIERS_BY_NAME['CMD_DELAY']:
            comment = "(sleep {} ms)".format(b)
            pass
        if last_b == MODIFIERS_BY_NAME['CMD_DELAY_TEN']:
            comment = "(sleep {} ms)".format(10*b)
            pass
        print_command(i, b, comment=comment)
        i += 1
        last_b = b
            

        pass
    pass
