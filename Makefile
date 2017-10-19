RM = rm -f
MKDIR = mkdir -p

CXX = g++ 
BUILDDIR = build
TARGET = hash
LIB = crypto
SOURCES = *.cpp

FLAGS = -lm

all: 
		$(MKDIR) $(BUILDDIR)
		$(CXX) $(FLAGS) $(SOURCES) -o $(BUILDDIR)/$(TARGET) -l$(LIB)

clean:
		$(RM) $(TARGET)
