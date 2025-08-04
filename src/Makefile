CPU = msp430g2553
CFLAGS = -mmcu=${CPU} -Os -I../h
LDFLAGS = -L../lib -L/opt/ti/msp430_gcc/include/
CC = msp430-elf-gcc
AS = msp430-elf-gcc -mmcu=${CPU} -c

all: project.elf

COMMON_OBJECTS =
PROJECT_OBJECTS = main.o pokeball.o pokemon_theme.o buzzer.o wdt_handler.o

project.elf: ${COMMON_OBJECTS} ${PROJECT_OBJECTS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load: project.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
