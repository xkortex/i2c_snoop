/** constants.h - define microcontroller pin assignments and other constants
 * Use this to assign all the pins. Don't bother with uints, god (Dennis Ritchie) gave us macros for a reason!
 */


#ifndef CONSTANTS_H
#define CONSTANTS_H


// ======================== PLATFORM ====================

///< V Only enable one of the following V
#define BOARD_ARDUINO
//#define BOARD_PHOTON
///< ^
// todo: use actual #defs for board names

#define NFFS_STARTING_ADDR     0x06D000
#define NFFS_SECTOR_SIZE       4096
#define NFFS_ERASE_SIZE        (2*NFFS_SECTOR_SIZE)

#define STARTING_ADDR     0x06D000
#define SECTOR_SIZE       4096
#define ERASE_SIZE        (7*SECTOR_SIZE)

// ======================== IO ============================
const char BLE_DEV_NAME[] = "I2C_SNOOP";

///< Serial I/O speed
#define BAUD 57600

///< Number of neopixels in display
#define N_PIXELS 1

#define ACK 0x1337BEEF

///< Serial/BLE I/O buffer
#define BUFSIZE 64

///< Max message buffer size
#define MSGARG_SIZE 20

///< Size of message stack
#define MSG_STACK_SIZE 10

///< Timeout for BLE connection, in milliseconds
#define BT_TIMEOUT_MS 500

// ====================== I2C ============================
//The 24LC1025 has a weird I2C address. The IC responds to two addresses in practice:
//0b.101.0000 (0x50) and 0b.101.0100 (0x54). There is a block select bit (bit 2) that
//allows us to read the full 17-bit address space
#define EEPROM_ADR_LOW_BLOCK 0x50 //0b.101.0000 0x50
#define EEPROM_ADR_HIGH_BLOCK 0x54 //0b.101.0100 0x54
#define MAX_I2C_WRITE 16 //32 fails for unknown reasons; only 30 bytes are written. 16 works.
#define I2C_CLOCK 100000 // the pull up resistor determines the max speed


// ======================== CONTROL LOOP ======================
///< period time of PID process, in microseconds
#define PERIOD_SIGDEL_S  0.033333333
#define VAC_CYCLE_MS 40
#define PERIOD_HEATER    200000
#define DEFAULT_PERIOD_PROCESS  200000
#define PERIOD_MINIMUM   100000
#define PERIOD_MAXIMUM 10000000

///< RELOCATED TO AUTO_ENUM.H
///< Default arguments for PID on startup
//#define DEFAULT_KC 0.015
//#define DEFAULT_KP 0.01
//#define DEFAULT_KI 0.0333
//#define DEFAULT_KD 10
//#define DEFAULT_KK 0.00075
//#define DEFAULT_IGRMAX 200
//#define DEFAULT_IGRMIN -200


///< Device will cut out if temperature goes above safety cut temp
//#define DEFAULT_SAFETYCUTT 550
///< Temp at which the device will return from an overheat fault. Set to 0 to essentially disable (require manual reset)
//#define SAFETY_RESET_TEMPC 0
///< Highest temp the device can be set to. May need to tweak this later
//const int DEFAULT_DEVMAXTSET = 500;

#define INIT_TEST_TEMPC 2
#define READY_RANGE_TEMPC 15

// ======================== PINS =============================

#ifdef BOARD_ARDUINO

    // SPI MISO, CS and Clock: DO == MISO. MOSI is N/C, not needed for TC
    #define P_DO 14
    #define P_CS 13
    #define P_CLK 12


    // colo LED output. Feather nRF52 supports PWM on all GPIO pins
    #define P_RED 27
    #define P_GRN 30
    #define P_BLU 31

    // Neopixel indicator
    #define N_LEDS 12
    #define P_PIXEL 16

#endif // BOARD_ARDUINO

#ifdef BOARD_PHOTON

    // SPI MISO, CS and Clock: DO == MISO
    ///< CS = Orange, CLK/SCK = Blue, MISO = Yellow, MOSI = Green, Periph Power = Red, Periph Ground = Black/Brown
    #define P_MOSI A5
    #define P_thermoDO A4
    #define P_thermoCS A2
    #define P_thermoCLK A3

    // Heater SSR control output
    #define P_HEAT D0

    // colo LED output
    #define P_RED D2
    #define P_GRN D3
    #define P_BLU D4
    // optional cooling fan
    #define P_FAN D1

    #define P_vccPin 8
    #define P_gndPin 2

    // Neopixel indicator
    #define N_LEDS 12
    #define P_PIXEL D6

#endif // BOARD_PHOTON

#define ADDR_SETTINGS 0x0
//#define ADDR_AUTOSET 0x0
//#define ADDR_BOOL1 0x1
//#define ADDR_BOOL2 0x2
//#define ADDR_BOOL3 0x3
//#define ADDR_KP 0x4
//#define ADDR_KI 0x8
//#define ADDR_KD 0xC
//#define ADDR_TSET 0x10




#endif //CONSTANTS_H
