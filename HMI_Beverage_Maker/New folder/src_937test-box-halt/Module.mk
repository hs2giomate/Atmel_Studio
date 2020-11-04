BEVMAKERHMI_DIR	:= BevMakerHmi

BEVMAKERHMI_CFLAGS	:= -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		   -W -Wundef -Wmissing-prototypes -Iinclude
#BEVMAKERHMI_LDFLAGS	:= -Llib -li2c
BEVMAKERHMI_LDFLAGS	:= -lwiringPi

BEVMAKERHMI_TARGETS	:= BevMakerHmiHalt

#
# Programs
#

$(BEVMAKERHMI_DIR)/BevMakerHmiHalt: $(BEVMAKERHMI_DIR)/main.o $(BEVMAKERHMI_DIR)/BevMakerHmiTest.o $(BEVMAKERHMI_DIR)/i2cbusses.o $(BEVMAKERHMI_DIR)/mainTimer.o  $(BEVMAKERHMI_DIR)/keyscan.o $(BEVMAKERHMI_DIR)/log.o $(BEVMAKERHMI_DIR)/led.o $(BEVMAKERHMI_DIR)/hmiManualReset.o
	$(CC) $(LDFLAGS) -o $@ $^ $(BEVMAKERHMI_LDFLAGS)



#
# Objects
#
$(BEVMAKERHMI_DIR)/main.o: $(BEVMAKERHMI_DIR)/main.c $(BEVMAKERHMI_DIR)/i2cbusses.h version.h $(INCLUDE_DIR)/i2c/smbus.h
	$(CC) $(CFLAGS) $(BEVMAKERHMI_CFLAGS) -c $< -o $@	

$(BEVMAKERHMI_DIR)/BevMakerHmiTest.o: $(BEVMAKERHMI_DIR)/BevMakerHmiTest.c $(BEVMAKERHMI_DIR)/i2cbusses.h version.h $(INCLUDE_DIR)/i2c/smbus.h
	$(CC) $(CFLAGS) $(BEVMAKERHMI_CFLAGS) -c $< -o $@

$(BEVMAKERHMI_DIR)/mainTimer.o: $(BEVMAKERHMI_DIR)/mainTimer.c
	$(CC) $(CFLAGS) $(BEVMAKERHMI_CFLAGS) -c $< -o $@	

$(BEVMAKERHMI_DIR)/hmiManualReset.o: $(BEVMAKERHMI_DIR)/hmiManualReset.c
	$(CC) $(CFLAGS) $(BEVMAKERHMI_CFLAGS) -c $< -o $@	

$(BEVMAKERHMI_DIR)/keyscan.o: $(BEVMAKERHMI_DIR)/keyscan.c
	$(CC) $(CFLAGS) $(BEVMAKERHMI_CFLAGS) -c $< -o $@	

$(BEVMAKERHMI_DIR)/log.o: $(BEVMAKERHMI_DIR)/log.c
	$(CC) $(CFLAGS) $(BEVMAKERHMI_CFLAGS) -c $< -o $@	

$(BEVMAKERHMI_DIR)/led.o: $(BEVMAKERHMI_DIR)/led.c
	$(CC) $(CFLAGS) $(BEVMAKERHMI_CFLAGS) -c $< -o $@	

$(BEVMAKERHMI_DIR)/i2cbusses.o: $(BEVMAKERHMI_DIR)/i2cbusses.c $(BEVMAKERHMI_DIR)/i2cbusses.h
	$(CC) $(CFLAGS) $(BEVMAKERHMI_CFLAGS) -c $< -o $@


#
# Commands
#

all-bevmakerhmi: $(addprefix $(BEVMAKERHMI_DIR)/,$(BEVMAKERHMI_TARGETS))
	cp $(addprefix $(BEVMAKERHMI_DIR)/,$(BEVMAKERHMI_TARGETS)) /home/pi

strip-bevmakerhmi: $(addprefix $(BEVMAKERHMI_DIR)/,$(BEVMAKERHMI_TARGETS))
	strip $(addprefix $(BEVMAKERHMI_DIR)/,$(BEVMAKERHMI_TARGETS))

clean-bevmakerhmi:
	$(RM) $(addprefix $(BEVMAKERHMI_DIR)/,*.o $(BEVMAKERHMI_TARGETS))

install-bevmakerhmi: $(addprefix $(BEVMAKERHMI_DIR)/,$(BEVMAKERHMI_TARGETS))
	$(INSTALL_DIR) $(DESTDIR)$(sbindir) $(DESTDIR)$(man8dir)
	for program in $(BEVMAKERHMI_TARGETS) ; do \
	$(INSTALL_PROGRAM) $(BEVMAKERHMI_DIR)/$$program $(DESTDIR)$(sbindir) ; \
	$(INSTALL_DATA) $(BEVMAKERHMI_DIR)/$$program.8 $(DESTDIR)$(man8dir) ; done

uninstall-bevmakerhmi:
	for program in $(BEVMAKERHMI_TARGETS) ; do \
	$(RM) $(DESTDIR)$(sbindir)/$$program ; \
	$(RM) $(DESTDIR)$(man8dir)/$$program.8 ; done

all: all-bevmakerhmi

strip: strip-bevmakerhmi

clean: clean-bevmakerhmi

install: install-bevmakerhmi

uninstall: uninstall-bevmakerhmi
