CXX = gcc
# CXXFLAGS =  ''

TARGET = build/main

# Cherche tous les fichiers .c
SRCS = $(shell find src -name "*.c")

# Transforme src/foo/bar.c → build/foo/bar.o
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# Règle par défaut
all:  build_dirs compile run

# Crée les sous-dossiers dans build/ en miroir de src/
build_dirs:
	@mkdir -p $(dir $(OBJS))


compile:  $(TARGET)

# Lien final
$(TARGET): $(OBJS)
	@echo "[!] Linking into $(TARGET)"
	@$(CXX) -o $@ $^ && echo "[+] Build successful" || echo "[-] Linking failed"

# Compilation .c → .o
build/%.o: src/%.c
	@echo "[*] Compiling $<"
	@$(CXX) -c $< -o $@

# Exécution
run: $(TARGET)
	@echo "[!] Running $(TARGET)"
	@./$(TARGET) && echo "[+] Done running" || echo "[-] Runtime error"


test: clean all
# Nettoyage
clean:
	@echo "[!] Cleaning build files"
	@rm -rf build $(TARGET)
