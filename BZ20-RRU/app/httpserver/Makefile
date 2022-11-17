NAME := httpserver

PWD := $(shell pwd)
APP := $(PWD)/..
TOP := $(APP)/..
INCS := -I$(APP)/inc -I$(TOP)/libbzsdk/inc
SRCDIR := $(PWD)
LIBS := $(APP)/lib/libtalise.a  $(APP)/lib/libfft.a $(TOP)/libclock/lib/libclock.a $(TOP)/libbzsdk/lib/libbzsdk.a
APPNAME := $(APP)/bin/$(NAME)


CSRC := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst %.c,%.o,$(CSRC))

CPPS := $(wildcard $(SRCDIR)/*.cpp)
CPPOBJS := $(patsubst %.cpp,%.o,$(CPPS))

all: $(APPNAME)
	
$(APPNAME): $(OBJS) $(CPPOBJS)  $(LIBS)
	$(CXX)  $(OBJS) $(CPPOBJS) $(LIBS) -lhistory -lreadline -lncurses -lpthread -o  $(APPNAME)

$(OBJS): %.o:%.c
	$(CC)  -c $(CFLAGS) $(INCS) $< -o $@

$(CPPOBJS): %.o:%.cpp
	$(CXX) -c $(CFLAGS) $(INCS) $< -o $@

install: $(APPNAME)
	$(STRIP) -s $(APPNAME)
	sudo cp $(APPNAME) /var/lib/tftpboot

.PHNOY:clean
clean:
	@rm -f $(OBJS)
	@rm -f $(CPPOBJS)
	@rm -f $(APPNAME)