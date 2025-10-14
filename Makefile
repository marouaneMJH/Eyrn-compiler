CXX = gcc
# CXXFLAGS =  ''

TARGET = build/main
PARSER_TARGET = build/parser

# Cherche tous les fichiers .c
SRCS = $(shell find src -name "*.c")

# Transforme src/foo/bar.c → build/foo/bar.o
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# Objects for parser (exclude original main.c, include main_parser.c)
PARSER_OBJS = $(filter-out build/main.o, $(OBJS)) build/main_parser.o

# Objects for parser (exclude original main.c, include main_parser.c)
MAIN_OBJS = $(filter-out build/main_parser.o, $(OBJS)) build/main.o


# Règle par défaut
all:  build_dirs compile run


# Parser target
parser: build_dirs compile_parser run_parser

# Crée les sous-dossiers dans build/ en miroir de src/
build_dirs:
	@mkdir -p $(dir $(OBJS))

compile:  $(TARGET)
compile_parser: $(PARSER_TARGET)

# Lien final pour scanner
$(TARGET): $(MAIN_OBJS)
	@echo "[!] Linking into $(TARGET)"
	@$(CXX) -o $@ $^ && echo "[+] Build successful" || echo "[-] Linking failed"

# Lien final pour parser
$(PARSER_TARGET): $(PARSER_OBJS)
	@echo "[!] Linking parser into $(PARSER_TARGET)"
	@$(CXX) -o $@ $^ && echo "[+] Parser build successful" || echo "[-] Parser linking failed"

# Compilation .c → .o
build/%.o: src/%.c
	@echo "[*] Compiling $<"
	@$(CXX) -c $< -o $@

# Exécution scanner
run: $(TARGET)
	@echo "[!] Running $(TARGET)"
	@./$(TARGET) && echo "[+] Done running" || echo "[-] Runtime error"

# Exécution parser  
run_parser: $(PARSER_TARGET)
	@echo "[!] Running parser $(PARSER_TARGET)"
	@./$(PARSER_TARGET) && echo "[+] Parser done running" || echo "[-] Parser runtime error"

test: clean all
test_parser: clean parser

# Nettoyage
clean:
	@echo "[!] Cleaning build files"
	@rm -rf build $(TARGET) $(PARSER_TARGET)
