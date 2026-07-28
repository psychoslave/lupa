# Lupa LSP

Language Server Protocol implementation for Lupa.

## Status

This is a minimal LSP server implementation that provides:
- Basic syntax checking via `lupe -l`
- Keyword completion
- Hover information
- Document symbol navigation (basic)

## Requirements

- Lua 5.3+ or Lupa
- lupe executable in PATH

## Installation

```bash
# Clone Lupa repository
cd lupa/lsp

# The LSP server is a single Lua script
```

## Usage

### With VS Code

1. Install the [LSP extension](https://marketplace.visualstudio.com/items?itemName=vscode.lsp-sample)
2. Create a configuration file:

```json
{
  "lupa-lsp.server.command": "lupe",
  "lupa-lsp.server.args": ["lsp/src/lupa_lsp.lua"],
  "lupa-lsp.server.workingDirectory": "path/to/lupa/lsp"
}
```

### With Neovim

```lua
-- ~/.config/nvim/init.lua
local lspconfig = require('lspconfig')

lspconfig.lupa_lsp.setup({
  cmd = {'lupe', 'lsp/src/lupa_lsp.lua'},
  filetypes = {'lua', 'lupa'},
  root_dir = function(fname)
    return vim.fn.getcwd()
  end
})
```

### With Emacs (lsp-mode)

```elisp
(use-package lsp-mode
  :ensure t
  :hook ((lua-mode . lsp-deferred))
  :commands lsp)

(setq lsp-lua-server-command '("lupe" "lsp/src/lupa_lsp.lua"))
```

## Features

### Implemented
- `initialize` - Server capabilities
- `textDocument/didOpen` - Document opened
- `textDocument/didChange` - Document changed
- `textDocument/didClose` - Document closed
- `textDocument/completion` - Keyword completion
- `textDocument/hover` - Hover information
- `textDocument/documentSymbol` - Document symbols
- `textDocument/definition` - Go to definition (basic)

### Not yet implemented
- Semantic analysis
- Type checking
- Full symbol database
- Code actions
- Formatting
- Rename

## Development

Run the server manually:
```bash
lupe lsp/src/lupa_lsp.lua
```

Then connect an LSP client to it via stdin/stdout.

## License

MIT License, same as Lupa.
