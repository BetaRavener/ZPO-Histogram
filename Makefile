OBJDIR = obj
SRCDIR = src
INCDIR = inc
OUTDIR = build

ZPO_OBJ = $(addprefix $(OBJDIR)/, \
	grayscale_image.o \
	histogram_base.o \
	full_histogram.o \
	undersampled_histogram.o \
	recursive_cross_histogram.o \
	evaluator.o \
	main_ivan.o \
	main_marek.o \
	main_matej.o)
	
CC = g++
# Executable flags
CFLAGS = -Wall -std=c++11 -I $(INCDIR) -I /usr/local/include -Werror -c
LFLAGS = -Wall -lopencv_core -lopencv_imgproc -lopencv_highgui -Wl,-rpath=./$(LDIR)

TITLE_COLOR = \033[33m
NO_COLOR = \033[0m
.SUFFIXES:

###
#
# Info on automatic variables :
# $@ : The name of the target of the rule (first part, before the colon)
# $< : The name of the first prerequisite (first dependency, after the colon)
# $^ : The names of all the prerequisites (everything after the colon)
# $* : The matched pattern by % in the target of the rule (aka the stem)
#
###

# when executing make, compile all exe's
all: mkdir zpo-histo

debug: CFLAGS += -g
debug: all

run:
	./build/zpo-histo img/lena.png 3 gauss 0.1

mkdir:
	@test -d $(OUTDIR) || mkdir $(OUTDIR)

# When trying to compile one of the executables, first look for its .o file (in the tmp folder)
# Then check if the libraries are in the lib folder
zpo-histo : $(ZPO_OBJ)
	@echo "$(TITLE_COLOR)\n***** LINKING $@ *****$(NO_COLOR)"
#when all dependencies were ok, link the executable
	$(CC) $(ZPO_OBJ) -o $(OUTDIR)/$@ $(LFLAGS)
	
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo "$(TITLE_COLOR)\n***** COMPILING $< *****$(NO_COLOR)"
	@test -d $(@D) || mkdir $(@D)
	$(CC) $(CFLAGS) -o$@ $< 

# do not look for files called clean, clean-all or *.check this will be always a target
.PHONY : all debug clean run

clean:
	rm -rf *.o $(OUTDIR) *~ $(OBJDIR)
