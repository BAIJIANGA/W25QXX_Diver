#ifndef __USB_USER_report__
#define __USB_USER_report__



//全键无冲报文
static unsigned char keyboard_ReportDescriptor[73] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x04,                    //   USAGE_MINIMUM (Keyboard a and A)
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x62,                    //   REPORT_COUNT (98)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x06,                    //   REPORT_SIZE (6)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0xc0                           // END_COLLECTION
};
//#define USER_HID_REPORT_DEC keyboard_ReportDescriptor;
static unsigned char pad_ReportDescriptor[] = {
0x05, 0x01,//USAGE_PAGE (Generic Desktop)                05 01
0x09, 0x04,//USAGE (Joystick)                            09 04
0xA1, 0x01, //COLLECTION (Application)                    A1 01 
0x05, 0x02,//  USAGE_PAGE (Simulation Controls)          05 02
0x09, 0xBB,//  USAGE (Throttle)                          09 BB
0x15, 0x81, //  LOGICAL_MINIMUM (-127)                    15 81 
0x25, 0x7F ,//  LOGICAL_MAXIMUM (127)                     25 7F 
0x75, 0x08, //  REPORT_SIZE (8)                           75 08 
0x95, 0x01, //  REPORT_COUNT (1)                          95 01 
0x81, 0x02, //  INPUT (Data,Var,Abs)                      81 02 
0xA9, 0x01, //  DELIMITER (Open)                          A9 01 
0x09, 0x20,//    USAGE (Flight Control Stick)            09 20
0x0B, 0x01, 0x00, 0x01, 0x00,//    USAGE (Generic Desktop:Pointer)         0B 01 00 01 00
0xA9 ,0x00, //  DELIMITER (Close)                         A9 00 
0xA1, 0x00, //  COLLECTION (Physical)                     A1 00 
0x0B, 0x30, 0x00, 0x01, 0x00,//    USAGE (Generic Desktop:X)               0B 30 00 01 00
0x0B, 0x31, 0x00 ,0x01 ,0x00,//    USAGE (Generic Desktop:Y)               0B 31 00 01 00
0x95, 0x02, //    REPORT_COUNT (2)                        95 02 
0x81, 0x02, //    INPUT (Data,Var,Abs)                    81 02 
0xC0,//  END_COLLECTION                            C0
0xA9, 0x01, //  DELIMITER (Open)                          A9 01 
0x0B, 0x20, 0x00, 0x05, 0x00,//    USAGE (Gaming Controls:Point of View)    0B 20 00 05 00
0x0B, 0x39,0x00, 0x01, 0x00,//    USAGE (Generic Desktop:Hat switch)      0B 39 00 01 00
0xA9, 0x00, //  DELIMITER (Close)                         A9 00 
0x15, 0x00,//  LOGICAL_MINIMUM (0)                       15 00 
0x25, 0x03, //  LOGICAL_MAXIMUM (3)                       25 03 
0x35, 0x00 ,//  PHYSICAL_MINIMUM (0)                      35 00 
0x46, 0x0E,0x01 ,//  PHYSICAL_MAXIMUM (270)                    46 0E 01 
0x65, 0x14, //  UNIT (Eng Rot:Angular Pos)                65 14 
0x55 ,0x00, //  UNIT_EXPONENT (0)                         55 00 
0x75, 0x04 ,//  REPORT_SIZE (4)                           75 04 
0x95 ,0x01, //  REPORT_COUNT (1)                          95 01 
0x81, 0x02, //  INPUT (Data,Var,Abs)                      81 02 
0xA9, 0x01 ,//  DELIMITER (Open)                          A9 01 
0x0B, 0x01 ,0x00 ,0x09 ,0x00,//    USAGE (Button:Button 1)                 0B 01 00 09 00
0x09 ,0xC0,//    USAGE (Trigger)                         09 C0
0xA9, 0x00 ,//  DELIMITER (Close)                         A9 00 
0xA9, 0x01 ,//  DELIMITER (Open)                          A9 01 
0x0B, 0x02, 0x00 ,0x09 ,0x00,//    USAGE (Button:Button 2)                 0B 02 00 09 00
0x09, 0xC2,//    USAGE (Weapons Select)                  09 C2
0xA9, 0x00, //  DELIMITER (Close)                         A9 00 
0xA9, 0x01 ,//  DELIMITER (Open)                          A9 01 
0x0B, 0x03 ,0x00 ,0x09 ,0x00,//    USAGE (Button:Button 3)                 0B 03 00 09 00
0x09, 0xB7,//    USAGE (Electronic Counter Measures)     09 B7
0x09, 0xBD,//    USAGE (Flare Release)                   09 BD
0xA9, 0x00 ,//  DELIMITER (Close)                         A9 00 
0xA9, 0x01, //  DELIMITER (Open)                          A9 01 
0x0B, 0x04, 0x00 ,0x09,0x00,//    USAGE (Button:Button 4)                 0B 04 00 09 00
0x09, 0xBE,//    USAGE (Landing Gear)                    09 BE
0x09, 0xB4,//    USAGE (Chaff Release)                   09 B4
0xA9, 0x00, //  DELIMITER (Close)                         A9 00 
0x15, 0x00, //  LOGICAL_MINIMUM (0)                       15 00 
0x25,0x01, //  LOGICAL_MAXIMUM (1)                       25 01 
0x35, 0x00 ,//  PHYSICAL_MINIMUM (0)                      35 00 
0x45, 0x01,//  PHYSICAL_MAXIMUM (1)                      45 01 
0x75, 0x01, //  REPORT_SIZE (1)                           75 01 
0x95, 0x04, //  REPORT_COUNT (4)                          95 04 
0x65, 0x00, //  UNIT (None)                               65 00 
0x81, 0x02, //  INPUT (Data,Var,Abs)                      81 02 
0xC0//END_COLLECTION                              C0
};

static unsigned char HID_pad_ReportDesc[]  =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x10,                    //     REPORT_COUNT (16)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x32,                    //     Usage (Z)
    0x09, 0x33,                    //     Usage (Rx)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
    0xc0,                          //     END_COLLECTION
    0xc0                           // END_COLLECTION
};

static unsigned char HID_pad_ReportDesc1[]  =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x00,                    //   COLLECTION (Physical)

    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,              //     LOGICAL_MAXIMUM (255)                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x10,                    //     REPORT_COUNT (6)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
//    0x05, 0x0F,                    //     USAGE_PAGE (Button)
//    0x09, 0x53,
//        0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
////    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
//    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
//    0x25, 0x01,              //     LOGICAL_MAXIMUM (255)                    //     LOGICAL_MAXIMUM (1)
//    0x95, 0x10,                    //     REPORT_COUNT (6)
//    0x75, 0x01,                    //     REPORT_SIZE (1)
//    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
//    0x05, 0x0C, // USAGE_PAGE (Consumer)
//    0x09, 0xB5, // USAGE (Scan Next Track)
//    0x09, 0xB6, // USAGE (Scan Previous Track)
//    0x15, 0x00, // LOGICAL_MIN (0)
//    0x25, 0x7F, // LOGICAL_MAX (127)
//    0x75, 0x08, // REPORT_SIZE (8)
//    0x95, 0x02, // REPORT_COUNT (2)
//    0x81, 0x02, // INPUT (Data,Var,Abs)

//    0x09, 0x36,                    //     USAGE (solder)
//    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
//    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
//    0x75, 0x08,                    //     REPORT_SIZE (8)
//    0x95, 0x01,                    //     REPORT_COUNT (1)
//    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x32,                    //     Usage (Z)
    0x09, 0x33,                    //     Usage (Rz)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)


    0x05, 0x02,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0xc5,                    //     USAGE (X)
    0x09, 0xc4,                    //     USAGE (Y)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)

//    0x05, 0x0E,                    //   USAGE_PAGE (LEDs)
////    0x09, 0x23,
//    0x19, 0x20,                    //   USAGE_MINIMUM (Num Lock)
//    0x29, 0x28,                    //   USAGE_MAXIMUM (Kana)
//    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//    0x26, 0xff, 0x00,                    //   LOGICAL_MAXIMUM (1)
//    0x95, 0x09,                    //   REPORT_COUNT (5)
//    0x75, 0x08,                    //   REPORT_SIZE (1)
//    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)

//    0x05, 0x0F, // USAGE_PAGE (Vendor Defined 0x0F)
//    0x09, 0x01, // USAGE (Vibration)
////    0xA1, 0x01, // COLLECTION (Application)
//    0x15, 0x00, // LOGICAL_MIN (0)
//    0x26, 0xFF, 0x00, // LOGICAL_MAX (255)
//    0x75, 0x08, // REPORT_SIZE (8)
//    0x95, 0x02, // REPORT_COUNT (2)
//    0x91, 0x02, // OUTPUT (Data,Var,Abs)
    
//    0x05, 0x0E,                    //   USAGE_PAGE (LEDs)
//    0x09, 0x23,
////    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
////    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
//    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//    0x26, 0xff, 0x00,                    //   LOGICAL_MAXIMUM (1)
//    0x95, 0x01,                    //   REPORT_COUNT (5)
//    0x75, 0x08,                    //   REPORT_SIZE (1)
//    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)

    0xc0,                          //     END_COLLECTION
    0xc0                           // END_COLLECTION
};

static unsigned char reportDesc[] = {
    // Gamepad Report Descriptor
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)

    // Button Section (14 buttons)
    0x05, 0x09,                    // USAGE_PAGE (Button)
    0x19, 0x01,                    // USAGE_MINIMUM (Button 1)
    0x29, 0x0E,                    // USAGE_MAXIMUM (Button 14)
    0x15, 0x00,                    // LOGICAL_MINIMUM (0)
    0x25, 0x01,                    // LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    // REPORT_SIZE (1)
    0x95, 0x0E,                    // REPORT_COUNT (14)
    0x81, 0x02,                    // INPUT (Data,Var,Abs)

    // Axis Section (X, Y, Z, Rz)
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    // USAGE (X)
    0x09, 0x31,                    // USAGE (Y)
    0x09, 0x32,                    // USAGE (Z)
    0x09, 0x33,                    // USAGE (Rz)
    0x16, 0x00, 0x00,              // LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0xFF,              // LOGICAL_MAXIMUM (65535 for 16-bit values)
    0x75, 0x10,                    // REPORT_SIZE (16)
    0x95, 0x04,                    // REPORT_COUNT (4)
    0x81, 0x02,                    // INPUT (Data,Var,Abs)

    // Trigger Section (Left and Right Triggers)
    0x09, 0x36,                    // USAGE (Slider) - Left Trigger
    0x46, 0xFF, 0x00,              // LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    // REPORT_SIZE (8)
    0x95, 0x01,                    // REPORT_COUNT (1)
    0x81, 0x02,                    // INPUT (Data,Var,Abs)
    
    0x09, 0x36,                    // USAGE (Slider) - Right Trigger
    0x46, 0xFF, 0x00,              // LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    // REPORT_SIZE (8)
    0x95, 0x01,                    // REPORT_COUNT (1)
    0x81, 0x02,                    // INPUT (Data,Var,Abs)

    // Output Report for Rumble (Vibration)
    0xa1, 0x02,                    // COLLECTION (Logical)
    0x09, 0x0E,                    // USAGE (Haptics)
    0x15, 0x00,                    // LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00,              // LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    // REPORT_SIZE (8)
    0x95, 0x02,                    // REPORT_COUNT (2) - For left and right motor
    0x91, 0x02,                    // OUTPUT (Data,Var,Abs)

    0xc0,                          // END_COLLECTION
    0xc0                           // END_COLLECTION
};

static unsigned char report_descriptor[] = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x04, // USAGE (Joystick)
    0xA1, 0x01, // COLLECTION (Application)

    // X, Y, Z, Rz Axes
    0x09, 0x01, // USAGE (Pointer)
    0xA1, 0x00, // COLLECTION (Physical)
    0x05, 0x09, // USAGE_PAGE (Button)
    0x19, 0x01, // USAGE_MIN (Button 1)
    0x29, 0x0E, // USAGE_MAX (Button 14)
    0x15, 0x00, // LOGICAL_MIN (0)
    0x25, 0x01, // LOGICAL_MAX (1)
    0x95, 0x0E, // REPORT_COUNT (14)
    0x75, 0x01, // REPORT_SIZE (1)
    0x81, 0x02, // INPUT (Data,Var,Abs)
    0x75, 0x08, // REPORT_SIZE (8)
    0x95, 0x04, // REPORT_COUNT (4)
    0x25, 0x7F, // LOGICAL_MAX (127)
    0x19, 0x00, // USAGE_MIN (0)
    0x29, 0x7F, // USAGE_MAX (127)
    0x81, 0x00, // INPUT (Cnst,Arr,Abs)

    // Two Triggers with Pressure
    0x05, 0x0C, // USAGE_PAGE (Consumer)
    0x09, 0xB5, // USAGE (Scan Next Track)
    0x09, 0xB6, // USAGE (Scan Previous Track)
    0x15, 0x00, // LOGICAL_MIN (0)
    0x25, 0x7F, // LOGICAL_MAX (127)
    0x75, 0x08, // REPORT_SIZE (8)
    0x95, 0x02, // REPORT_COUNT (2)
    0x81, 0x02, // INPUT (Data,Var,Abs)

    0xC0, // END_COLLECTION
};

static unsigned char HIDREPORTDESC[27] =
{
    0x05,0x0c,              //USAGE_PAGE(Consumer);
    0x09,0x01,              //USAGE(Consumer Control);
    0xa1,0x01,              //COLLECTION(Application);
    0x15,0x00,              //  LOGICAL_MINIMUM(0);
    0x25,0xff,              //  LOGICAL_MAXIMUM(255);
    0x75,0x08,              //  REPORT_SIZE(8);
    0x95,0x40,              //  REPORT_COUNT(64);
    0x09,0x01,              //  USAGE(Consumer Control);
    0xb1,0x02,              //  FEATURE(Data,Variable);
    0x09,0x01,              //  USAGE(Consumer Control);
    0x81,0x02,              //  INPUT(Data,Variable);
    0x09,0x01,              //  USAGE(Consumer Control);
    0x91,0x02,              //  OUTPUT(Data,Variable);
    0xc0,                   //END_COLLECTION;
};

static unsigned char HID_GamePad_ReportDesc2[] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x00,                    //   COLLECTION (Physical)

    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 14)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x10,                    //     REPORT_COUNT (14)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x32,                    //     USAGE (Z)
    0x09, 0x35,                    //     USAGE (Rz)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
//    0x05, 0x02,                    //     USAGE_PAGE (Generic Desktop)
//    0x09, 0xc5,                    //     USAGE (X)
//    0x09, 0xc4,                    //     USAGE (Y)
//    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
//    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
//    0x75, 0x08,                    //     REPORT_SIZE (8)
//    0x95, 0x02,                    //     REPORT_COUNT (4)
//    0x81, 0x02,                    //     INPUT (Data,Var,Abs)

//    // Left Trigger and Right Trigger
//    0x09, 0x36,                    //     USAGE (Z)
////    0x09, 0x37,                    //     USAGE (Rz)
//    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
//    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
//    0x75, 0x08,                    //     REPORT_SIZE (8)
//    0x95, 0x01,                    //     REPORT_COUNT (2)
//    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
//    
//    // Vibration
//    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
//    0x09, 0x70,                    //     USAGE (Vibration)
//    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
//    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
//    0x75, 0x01,                    //     REPORT_SIZE (1)
//    0x95, 0x02,                    //     REPORT_COUNT (2)
//    0x91, 0x02,                    //     INPUT (Data,Var,Abs)
    
    0xc0,                          //     END_COLLECTION
    0xc0                           // END_COLLECTION
};

static unsigned char HID_pad_ReportDesc11[120] =
{
    0x05, 0x01, 0x09, 0x05, 0xa1, 0x01, 0xa1, 0x00,
    0x09, 0x30, 0x09, 0x31, 0x15, 0x00, 0x26, 0xff, 0xff,
    0x35, 0x00, 0x46, 0xff, 0xff, 0x95, 0x02, 0x75, 0x10,
    0x81, 0x02, 0xc0, 0xa1, 0x00, 0x09, 0x33, 0x09, 0x34,
    0x15, 0x00, 0x26, 0xff, 0xff, 0x35, 0x00, 0x46, 0xff,
    0xff, 0x95, 0x02, 0x75, 0x10, 0x81, 0x02, 0xc0, 0xa1,
    0x00, 0x09, 0x32, 0x15, 0x00, 0x26, 0xff, 0xff, 0x35,
    0x00, 0x46, 0xff, 0xff, 0x95, 0x01, 0x75, 0x10, 0x81,
    0x02, 0xc0, 0x05, 0x09, 0x19, 0x01, 0x29, 0x0a, 0x95,
    0x0a, 0x75, 0x01, 0x81, 0x02, 0x05, 0x01, 0x09, 0x39,
    0x15, 0x01, 0x25, 0x08, 0x35, 0x00, 0x46, 0x3b, 0x10,
    0x66, 0x0e
};

#endif

