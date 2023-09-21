$(info SYSROOT - ${SYSROOT})
$(info CC - ${CC})
$(info CPPFLAGS - ${CPPFLAGS})
$(info CXXFLAGS - ${CXXFLAGS})

# Compiler and Linker
# CC          := g++

# The Target Binary Program
TARGET      := lisot

# The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := source
INCDIR      := inc
BUILDDIR    := build
TARGETDIR   := exe
RESDIR      := resources
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

# Flags, Libraries and Includes
#CFLAGS      := -fopenmp -Wall -Wextra -Wpedantic -O3 -g
#LIB         := -fopenmp -lm -larmadillo
CCONST		:= -DOS_LINUX=0 -DOS_ANDROID=1 -DOS_TARGET=OS_LINUX -DMSG_TRACE -DCOLORED_TRACE
CFLAGS      := -g -std=c++17 $(CCONST) $(CFLAGS)
LIB         := -lrt -lstdc++ -L$(SYSROOT)/lib
INC         := -I./$(SRCDIR) -I./$(SRCDIR)/include -I$(SYSROOT)/usr/include -I$(SYSROOT)/usr/local/include
INCDEP      := -I./$(SRCDIR) -I./$(SRCDIR)/include -I$(SYSROOT)/usr/include -I$(SYSROOT)/usr/local/include

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))



# Defauilt Make
all: config $(TARGET)

# Clean build
build: all

# Clean build
clean_build: clean all

# Full Clean, Objects and Binaries
clean:
	@$(RM) -rf $(BUILDDIR)
	@$(RM) -rf $(TARGETDIR)

# Set configuration
config: directories
# 	reset

# Copy Resources from Resources Directory to Target Directory
resources: directories
	@cp $(RESDIR)/* $(TARGETDIR)/

# Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# Execute
server:
	$(TARGETDIR)/$(TARGET) --server \
		--family=AF_VSOCK --address=1 --port=5400 \
		--trace_log=console --trace_buffer=4096 --trace_app_name=lstszrv --trace_level=6

client:
	$(TARGETDIR)/$(TARGET) --client \
		--family=AF_VSOCK --address=1 --port=5400 \
		--mode=random --timeout=500 --length=0 \
		--trace_log=console --trace_buffer=4096 --trace_app_name=lstcl --trace_level=6

usage:
	$(TARGETDIR)/$(TARGET)

install:
	install -d $(DESTDIR)/usr/bin
	install -m 0755 $(TARGET) $(DESTDIR)/usr/bin

# Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

# Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB) $(LDFLAGS)

# Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

# Non-File Targets
.PHONY: all build clean_build clean
