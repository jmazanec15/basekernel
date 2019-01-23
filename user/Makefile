
include ../Makefile.config

USER_PROGRAMS=saver.exe ball.exe shell.exe snake.exe manager.exe clock.exe mandelbrot.exe copy.exe

all: $(USER_PROGRAMS) $(TESTS)

%.o: %.c
	${CC} ${KERNEL_CCFLAGS} -I ../include $< -o $@

%.exe: %.o ../library/user-start.o ../library/baselib.a
	${LD} ${KERNEL_LDFLAGS} -Ttext 0x80000000 ../library/user-start.o $< ../library/baselib.a -o $@

clean:
	rm -rf *.exe *.o
