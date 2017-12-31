#Copyright (c) 2017 xiethon@126.com
TARGET = $(notdir $(CURDIR))

CROSS_COMPILE = gcc
COMPILE.c = $(CROSS_COMPILE)  -c
LINK.c = $(CROSS_COMPILE)
RM =rm

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard inc/*.h)
OBJFILES = $(SOURCES:%.c=%.o)

.PHONY:clean all install

all:$(TARGET)
	@echo builded target:$^
	@echo
$(TARGET): $(OBJFILES)
	@echo
	@echo Linking $@ from $^...
	$(LINK.c) -o $@ $^ -lpthread
	@echo Link finished

$(OBJFILES): %.o:%.c $(HEADERS)
	@echo
	@echo Compiling $@ from $<...
	$(COMPILE.c) -o $@ $<
	@echo Compile finished

clean:
	@echo Removing generated files...
	@ -$(RM) -rf $(OBJFILES) $(TARGET) $(EXEC_DIR) *~ *.d *.o
