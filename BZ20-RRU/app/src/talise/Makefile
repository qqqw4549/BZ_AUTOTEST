NAME := talise

PWD := $(shell pwd)
APP := $(PWD)/../..
TOP := $(APP)/..
INCS := -I$(APP)/inc -I$(TOP)/libclock/inc -I$(TOP)/libbzsdk/inc
SRCDIR := $(PWD)
LIBDIR := $(APP)/lib
LIBNAME := $(LIBDIR)/lib$(NAME).a


CSRC := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst %.c,%.o,$(CSRC))

CPPS := $(wildcard $(SRCDIR)/*.cpp)
CPPOBJS := $(patsubst %.cpp,%.o,$(CPPS))

all: $(OBJS) $(CPPOBJS)
	$(AR) -rcD $(LIBNAME) $(OBJS) $(CPPOBJS)  

$(OBJS): %.o:%.c
	$(CC) -c $(CFLAGS) $(INCS) $< -o $@

$(CPPOBJS): %.o:%.cpp
	$(CXX) -c $(CFLAGS) $(INCS) $< -o $@

.PHNOY:clean
clean:
	@rm -f $(OBJS)
	@rm -f $(CPPOBJS)
	@rm -f $(LIBNAME)
install: all
