# Lupa LSP Makefile

LUA = lupe
LSP_SRC = src
LSP_MAIN = $(LSP_SRC)/lupa_lsp.lua

all: test

test: $(LSP_MAIN)
	@echo "Running LSP server test..."
	@$(LUA) $(LSP_MAIN) < /dev/null || true

run: $(LSP_MAIN)
	@echo "Starting Lupa LSP server..."
	@echo "Connect your LSP client to this process via stdin/stdout"
	$(LUA) $(LSP_MAIN)

.PHONY: all test run
