NAME := bz20test

PWD := $(shell pwd)
APP := $(PWD)/..
TOP := $(APP)/..
INCS := -I$(APP)/inc -I$(TOP)/libbzsdk/inc -I$(TOP)/libclock/inc
SRCDIR := $(PWD)
LIBS := $(APP)/lib/libcommand.a  $(TOP)/libclock/lib/libclock.a $(TOP)/libbzsdk/lib/libbzsdk.a
APPNAME := $(APP)/bin/$(NAME)


CSRC := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst %.c,%.o,$(CSRC))

CPPS := $(wildcard $(SRCDIR)/*.cpp)
CPPOBJS := $(patsubst %.cpp,%.o,$(CPPS))

all: $(APPNAME) 
	
$(APPNAME): $(OBJS) $(CPPOBJS) $(LIBS)
	$(CXX) $(OBJS) $(CPPOBJS) $(LIBS) -lhistory -lreadline -lncurses -o  $(APPNAME)

$(OBJS): %.o:%.c
	$(CC) -c $(CFLAGS) $(INCS) $< -o $@

$(CPPOBJS): %.o:%.cpp
	$(CXX) -c $(CFLAGS) $(INCS) $< -o $@

install: $(APPNAME)
	sudo cp $(APPNAME) /var/lib/tftpboot

.PHNOY:clean
clean:
	@rm -f $(OBJS)
	@rm -f $(CPPOBJS)
	@rm -f $(APPNAME)