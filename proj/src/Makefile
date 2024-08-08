# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c graphic.c menu.c utils.c RTC.c keyboard.c VideoCodec.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIBLM}
LDADD += -llcf -llm -lm -lavutil -lavformat -lavcodec -lswscale -lz -lm
FDADD += liblm.a
FSADD += libxpm.a

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

run: ${proj} ./${proj}