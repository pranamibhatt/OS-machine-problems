# Sony CONFIDENTIAL
#
# Copyright 2010 Sony Corporation
#
# DO NOT COPY AND/OR REDISTRIBUTE WITHOUT PERMISSION.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
# 
# Author : Pranami Bhattacharya
#

CROSS = arm-linux-gnueabi-
CC      = $(CROSS)gcc

LIB_MAJ = 0.0.0
LIB_MIN = 0

LIB_HEADERS = lib/memdrip.h
LIB_SRC = lib/memdrip.c
LIB_OBJS = lib/memdrip.o

CFLAGS  += -pipe -g

all: libmemdrip utils 

lib/%.o: $(LIB_SRC)
	$(CC) $(CFLAGS) -fPIC -o $@ -c lib/$*.c

                                                                              
libmemdrip.so.$(LIB_MAJ): $(LIB_OBJS)
	$(CC) -shared $(CFLAGS) $(LDFLAGS) -o $@ \
                $(LDFLAGS) -Wl,-soname,libmemdrip.so.$(LIB_MIN) $(LIB_OBJS)
	@ln -sf $@ libmemdrip.so
	 @ln -sf $@ libmemdrip.so.$(LIB_MIN)

libmemdrip: libmemdrip.so.$(LIB_MAJ)


mem%: $(UTILS_SRC)
	$(CC) $(CFLAGS) -I. -I./lib/ -c -o utils/$@.o utils/$*.c	
	 $(CC) $(CFLAGS) $(LDFLAGS) -L. -o $@ utils/$@.o -lmemdrip

utils: memproc
clean:
	rm lib/*.o *.so* memproc
       


