#ifndef LIBRA_JM201_H
#define LIBRA_JM201_H
/*!
 * @file jm201.h
 * Author: Amin Khozaei
 * Lib: LibraSDK
 * Inspired by: Adafruit-Fingerprint-Sensor-Library
 */

//REG_SECTION_START
////////////////////////////////////////////////////////////////////////////////////
//JM201 REG and COMMAND values
#define LIBRA_JM201_CMD_OK 0x00               //!< Command execution is complete
#define LIBRA_JM201_CMD_PACKETRECIEVEERR 0x01 //!< Error when receiving data package
#define LIBRA_JM201_CMD_NOFINGER 0x02         //!< No finger on the sensor
#define LIBRA_JM201_CMD_IMAGEFAIL 0x03        //!< Failed to enroll the finger
#define LIBRA_JM201_CMD_IMAGEMESS                                                  \
  0x06 //!< Failed to generate character file due to overly disorderly
       //!< fingerprint image
#define LIBRA_JM201_CMD_FEATUREFAIL                                                \
  0x07 //!< Failed to generate character file due to the lack of character point
       //!< or small fingerprint image
#define LIBRA_JM201_CMD_NOMATCH 0x08  //!< Finger doesn't match
#define LIBRA_JM201_CMD_NOTFOUND 0x09 //!< Failed to find matching finger
#define LIBRA_JM201_CMD_ENROLLMISMATCH                                             \
  0x0A //!< Failed to combine the character files
#define LIBRA_JM201_CMD_BADLOCATION                                                \
  0x0B //!< Addressed PageID is beyond the finger library
#define LIBRA_JM201_CMD_DBRANGEFAIL                                                \
  0x0C //!< Error when reading template from library or invalid template
#define LIBRA_JM201_CMD_UPLOADFEATUREFAIL 0x0D //!< Error when uploading template
#define LIBRA_JM201_CMD_PACKETRESPONSEFAIL                                         \
  0x0E //!< Module failed to receive the following data packages
#define LIBRA_JM201_CMD_UPLOADFAIL 0x0F  //!< Error when uploading image
#define LIBRA_JM201_CMD_DELETEFAIL 0x10  //!< Failed to delete the template
#define LIBRA_JM201_CMD_DBCLEARFAIL 0x11 //!< Failed to clear finger library
#define LIBRA_JM201_CMD_PASSFAIL                                                   \
  0x13 //!< Find whether the fingerprint passed or failed
#define LIBRA_JM201_CMD_INVALIDIMAGE                                               \
  0x15 //!< Failed to generate image because of lac of valid primary image
#define LIBRA_JM201_CMD_FLASHERR 0x18   //!< Error when writing flash
#define LIBRA_JM201_CMD_INVALIDREG 0x1A //!< Invalid register number
#define LIBRA_JM201_CMD_ADDRCODE 0x20   //!< Address code
#define LIBRA_JM201_CMD_PASSVERIFY 0x21 //!< Verify the fingerprint passed
#define LIBRA_JM201_CMD_STARTCODE                                                  \
  0xEF01 //!< Fixed falue of EF01H; High byte transferred first

#define LIBRA_JM201_CMD_COMMANDPACKET 0x1 //!< Command packet
#define LIBRA_JM201_CMD_DATAPACKET                                                 \
  0x2 //!< Data packet, must follow command packet or acknowledge packet
#define LIBRA_JM201_CMD_ACKPACKET 0x7     //!< Acknowledge packet
#define LIBRA_JM201_CMD_ENDDATAPACKET 0x8 //!< End of data packet

#define LIBRA_JM201_CMD_TIMEOUT 0xFF   //!< Timeout was reached
#define LIBRA_JM201_CMD_BADPACKET 0xFE //!< Bad packet was sent

#define LIBRA_JM201_CMD_GETIMAGE 0x01 //!< Collect finger image
#define LIBRA_JM201_CMD_IMAGE2TZ 0x02 //!< Generate character file from image
#define LIBRA_JM201_CMD_SEARCH 0x04   //!< Search for fingerprint in slot
#define LIBRA_JM201_CMD_REGMODEL                                                   \
  0x05 //!< Combine character files and generate template
#define LIBRA_JM201_CMD_STORE 0x06          //!< Store template
#define LIBRA_JM201_CMD_LOAD 0x07           //!< Read/load template
#define LIBRA_JM201_CMD_UPLOAD 0x08         //!< Upload template
#define LIBRA_JM201_CMD_DELETE 0x0C         //!< Delete templates
#define LIBRA_JM201_CMD_EMPTY 0x0D          //!< Empty library
#define LIBRA_JM201_CMD_READSYSPARAM 0x0F   //!< Read system parameters
#define LIBRA_JM201_CMD_SETPASSWORD 0x12    //!< Sets passwords
#define LIBRA_JM201_CMD_VERIFYPASSWORD 0x13 //!< Verifies the password
#define LIBRA_JM201_CMD_HISPEEDSEARCH                                              \
  0x1B //!< Asks the sensor to search for a matching fingerprint template to the
       //!< last model generated
#define LIBRA_JM201_CMD_TEMPLATECOUNT 0x1D //!< Read finger template numbers
#define LIBRA_JM201_CMD_AURALEDCONFIG 0x35 //!< Aura LED control
#define LIBRA_JM201_CMD_LEDON 0x50         //!< Turn on the onboard LED
#define LIBRA_JM201_CMD_LEDOFF 0x51        //!< Turn off the onboard LED

#define LIBRA_JM201_CMD_LED_BREATHING 0x01   //!< Breathing light
#define LIBRA_JM201_CMD_LED_FLASHING 0x02    //!< Flashing light
#define LIBRA_JM201_CMD_LED_ON 0x03          //!< Always on
#define LIBRA_JM201_CMD_LED_OFF 0x04         //!< Always off
#define LIBRA_JM201_CMD_LED_GRADUAL_ON 0x05  //!< Gradually on
#define LIBRA_JM201_CMD_LED_GRADUAL_OFF 0x06 //!< Gradually off
#define LIBRA_JM201_CMD_LED_RED 0x01         //!< Red LED
#define LIBRA_JM201_CMD_LED_BLUE 0x02        //!< Blue LED
#define LIBRA_JM201_CMD_LED_PURPLE 0x03      //!< Purple LEDpassword

#define LIBRA_JM201_CMD_REG_ADDR_ERROR 0x1A //!< shows register address error
#define LIBRA_JM201_CMD_WRITE_REG 0x0E      //!< Write system register instruction

#define LIBRA_JM201_CMD_BAUD_REG_ADDR 0x4   //!< BAUDRATE register address
#define LIBRA_JM201_CMD_BAUDRATE_9600 0x1   //!< UART baud 9600
#define LIBRA_JM201_CMD_BAUDRATE_19200 0x2  //!< UART baud 19200
#define LIBRA_JM201_CMD_BAUDRATE_28800 0x3  //!< UART baud 28800
#define LIBRA_JM201_CMD_BAUDRATE_38400 0x4  //!< UART baud 38400
#define LIBRA_JM201_CMD_BAUDRATE_48000 0x5  //!< UART baud 48000
#define LIBRA_JM201_CMD_BAUDRATE_57600 0x6  //!< UART baud 57600
#define LIBRA_JM201_CMD_BAUDRATE_67200 0x7  //!< UART baud 67200
#define LIBRA_JM201_CMD_BAUDRATE_76800 0x8  //!< UART baud 76800
#define LIBRA_JM201_CMD_BAUDRATE_86400 0x9  //!< UART baud 86400
#define LIBRA_JM201_CMD_BAUDRATE_96000 0xA  //!< UART baud 96000
#define LIBRA_JM201_CMD_BAUDRATE_105600 0xB //!< UART baud 105600
#define LIBRA_JM201_CMD_BAUDRATE_115200 0xC //!< UART baud 115200

#define LIBRA_JM201_CMD_SECURITY_REG_ADDR 0x5 //!< Security level register address
// The safety level is 1 The highest rate of false recognition , The rejection
// rate is the lowest . The safety level is 5 The lowest tate of false
// recognition, The rejection rate is the highest .
#define LIBRA_JM201_CMD_SECURITY_LEVEL_1 0X1 //!< Security level 1
#define LIBRA_JM201_CMD_SECURITY_LEVEL_2 0X2 //!< Security level 2
#define LIBRA_JM201_CMD_SECURITY_LEVEL_3 0X3 //!< Security level 3
#define LIBRA_JM201_CMD_SECURITY_LEVEL_4 0X4 //!< Security level 4
#define LIBRA_JM201_CMD_SECURITY_LEVEL_5 0X5 //!< Security level 5

#define LIBRA_JM201_CMD_PACKET_REG_ADDR 0x6 //!< Packet size register address
#define LIBRA_JM201_CMD_PACKET_SIZE_32 0X0  //!< Packet size is 32 Byte
#define LIBRA_JM201_CMD_PACKET_SIZE_64 0X1  //!< Packet size is 64 Byte
#define LIBRA_JM201_CMD_PACKET_SIZE_128 0X2 //!< Packet size is 128 Byte
#define LIBRA_JM201_CMD_PACKET_SIZE_256 0X3 //!< Packet size is 256 Byte
////////////////////////////////////////////////////////////////////////////////////
//REG_SECTION_END

#define LIBRA_JM201_TIMEOUT 1000

#endif