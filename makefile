CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra

INCLUDES := \
	-Iluau/Ast/include \
	-Iluau/Config/include \
	-Iluau/VM/include \
	-Iluau/Compiler/include \
	-Iluau/Analysis/include

SRC := $(wildcard src/**/*.cpp) \
	src/main.cpp

OUT := BlockEngine

LUAU_LIBDIR := luau/build
LUAU_LIBS := \
	$(LUAU_LIBDIR)/libLuau.Ast.a \
	$(LUAU_LIBDIR)/libLuau.Config.a \
	$(LUAU_LIBDIR)/libLuau.Analysis.a \
	$(LUAU_LIBDIR)/libLuau.Compiler.a \
	$(LUAU_LIBDIR)/libLuau.VM.a

LDFLAGS := -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(OUT) $(LUAU_LIBS) $(LDFLAGS)

clean:
	rm -f $(OUT)
