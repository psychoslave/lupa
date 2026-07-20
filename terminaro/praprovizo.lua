return {
  lex = {
    "and", "break", "do", "else", "elseif",
    "end", "false", "for", "function", "goto", "if",
    "in", "local", "nil", "not", "or", "repeat",
    "return", "then", "true", "until", "while",
    "//", "..", "...", "==", ">=", "<=", "~=",
    "<nombra>", "<indukta>", "<noma>", "<ĉena>",
  },

  base = {
    "assert", "collectgarbage", "dofile", "error", "getmetatable", "ipairs",
    "loadfile", "load", "loadstring", "next", "pairs", "pcall", "print",
    "rawequal", "rawlen", "rawget", "rawset", "select", "setmetatable",
    "tonumber", "tostring", "type", "xpcall",
  },

  package = {
    "loadlib", "searchpath", "seeall",
    "preload", "cpath", "path", "searchers", "loaded",
  },

  globals = {
    "module",
    "require",
  },
}
