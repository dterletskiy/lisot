# Compiler and Linker
CC          := g++

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
#CFLAGS      := -fopenmp -Wall -O3 -g
#LIB         := -fopenmp -lm -larmadillo
CCONST		:= -DOS_LINUX=0 -DOS_ANDROID=1 -DOS_TARGET=OS_LINUX -DMSG_TRACE -DCOLORED_TRACE
CFLAGS      := -g -std=c++17 $(CCONST)
LIB         := -lrt
INC         := -I./$(SRCDIR) -I./$(SRCDIR)/include -I/usr/local/include
INCDEP      := -I./$(SRCDIR) -I./$(SRCDIR)/include

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
	reset

# Copy Resources from Resources Directory to Target Directory
resources: directories
	@cp $(RESDIR)/* $(TARGETDIR)/

# Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# Execute
server:
	reset
	$(TARGETDIR)/$(TARGET) --server \
		--family=AF_VSOCK --address=1 --port=5400 \
		--trace_log=console --trace_buffer=4096 --trace_app_name=lstszrv --trace_level=6

client:
	reset
	$(TARGETDIR)/$(TARGET) --client \
		--family=AF_VSOCK --address=1 --port=5400 \
		--mode=random --timeout=500 --length=0 \
		--trace_log=console --trace_buffer=4096 --trace_app_name=lstcl --trace_level=6

usage:
	reset
	$(TARGETDIR)/$(TARGET)

# Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

# Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

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
