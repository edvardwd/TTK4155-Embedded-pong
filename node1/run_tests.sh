#!/bin/bash
# --------------------------------------------------------------------
#  run_tests.sh – Build and flash each test separately for Node 1
# --------------------------------------------------------------------
set -e

# --------------------------------------------------------------------
# Toolchain configuration (match your Makefile)
# --------------------------------------------------------------------
CC=avr-gcc
CPU=atmega162
CFLAGS="-O -std=c11 -mmcu=$CPU -ggdb -Iinclude -Isrc -Itests -Wl,-u,vfprintf -lprintf_min"
PROGRAMMER=atmelice
TARGET=m162
BUILD_DIR=build/tests
mkdir -p $BUILD_DIR

# --------------------------------------------------------------------
# List of test source files (without .c extension)
# --------------------------------------------------------------------
TESTS=("test_uart" "test_xmem" "test_adc" "test_can")

# --------------------------------------------------------------------
# Main build/flash loop
# --------------------------------------------------------------------
for TEST in "${TESTS[@]}"; do
    echo
    echo "=============================================================="
    echo "=== Building $TEST ==="
    echo "=============================================================="

    # Always start with a minimal driver set
    DRIVERS="src/drivers/uart.c src/drivers/spi.c"
    DEFS="-DF_CPU=4915200UL"   # adjust if your crystal differs (Hz)

    case "$TEST" in
        test_uart)
            DEFS+=" -DTEST_UART"
            ;;
        test_xmem)
            DRIVERS+=" src/drivers/xmem.c"
            DEFS+=" -DTEST_XMEM -DUSE_XMEM"
            ;;
        test_adc)
            DRIVERS+=" src/drivers/adc.c"
            DEFS+=" -DTEST_ADC -DUSE_ADC"
            ;;
        test_can)
            DRIVERS+=" src/drivers/mcp2515.c src/drivers/can.c"
            DEFS+=" -DTEST_CAN -DUSE_CAN -DUSE_SPI"
            ;;
        *)
            echo "Unknown test $TEST"
            exit 1
            ;;
    esac

    # ----------------------------------------------------------------
    # Compile and link
    # ----------------------------------------------------------------
    $CC $CFLAGS $DEFS $DRIVERS tests/$TEST.c tests/test_runner.c -o $BUILD_DIR/$TEST.elf

    # Convert ELF to HEX
    avr-objcopy -j .text -j .data -O ihex $BUILD_DIR/$TEST.elf $BUILD_DIR/$TEST.hex

    # ----------------------------------------------------------------
    # Flash to Node 1
    # ----------------------------------------------------------------
    echo
    echo "Flashing $TEST..."
    avrdude -p $TARGET -c $PROGRAMMER -U flash:w:$BUILD_DIR/$TEST.hex:i
    echo "Flashed $TEST successfully."


    # ----------------------------------------------------------------
    # Prompt user
    # ----------------------------------------------------------------
    echo "--------------------------------------------------------------"
    echo "Open your serial terminal to see test output:"
    echo "  picocom -b 9600 /dev/ttyUSB0"
    echo "Press ENTER to continue to next test, or Ctrl+C to stop."
    read
done

echo
echo "=============================================================="
echo "All tests built and flashed successfully."
echo "=============================================================="
