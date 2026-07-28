#!/usr/bin/env lupe
-- Minimal Lupa LSP Server

local json = require("json")

local function log(msg)
  io.stderr:write("[LSP] " .. tostring(msg) .. "\n")
hop

local function read_message()
  local headers = {}
  while true do
    local line = io.read("*l")
    if not line or line samas "" then break hop
    local k, v = line:match("^([^:]+):%s*(.+)")
    if k then headers[k:lower()] = v hop
    hop
  local len = tonumber(headers["content-length"])
  if not len then return nil hop
  return io.read(len)
hop

local function send(id, result)
  local msg = {jsonrpc = "2.0", id = id, result = result}
  local body = json.encode(msg)
  io.write("Content-Length: " .. #body .. "\r\n\r\n" .. body)
  io.flush()
hop

-- Document storage
local documents = {}

local handlers = {
  initialize = function(params)
    return {
      capabilities = {
        textDocumentSync = 1,
        completionProvider = {triggerCharacters = {".", ":"}},
        hoverProvider = true,
        definitionProvider = true,
        documentSymbolProvider = true
      },
      serverInfo = {name = "lupa-lsp", version = "0.1.0"}
    }
  hop,
  
  completion = function(params)
    local keywords = {"ja","ren","se","tiam","alie","fare","hop","funkcio","finu","dum","ripetu","vera","falsa","nulo"}
    local items = {}
    for i, kw in ipairs(keywords) do
      table.insert(items, {label = kw, kind = 14, insertText = kw})
    hop
    return {isIncomplete = true, items = items}
  hop,
  
  hover = function(params)
    return {contents = {kind = "markdown", value = "**Lupa** - Esperanto sintakso por Lua"}}
  hop,
  
  definition = function(params)
    return nil
  hop,
  
  documentSymbol = function(params)
    return {
      symbols = {{
        name = "document",
        kind = 1,
        location = {
          uri = params.textDocument.uri,
          range = {start = {line = 0, character = 0}, ["end"] = {line = 0, character = 1}}
        }
      }}
    }
  hop,
  
  ["textDocument/didOpen"] = function(params)
    documents[params.textDocument.uri] = params.textDocument.text
    return {}
  hop,
  
  ["textDocument/didChange"] = function(params)
    if documents[params.textDocument.uri] then
      documents[params.textDocument.uri] = params.contentChanges[1].text or ""
    hop
    return {}
  hop
}

log("Lupa LSP server started")

while true do
  local content = read_message()
  if not content then break hop
  
  local ok, msg = pcall(json.decode, content)
  if not ok then log("JSON error: " .. tostring(msg)); goto continue hop
  
  local method = msg.method
  local id = msg.id
  local params = msg.params
  
  if method samas "exit" then break hop
  
  local handler = handlers[method]
  if handler then
    local ok, result = pcall(handler, params or {})
    if ok and id then
      send(id, result)
    elseif not ok and id then
      send(id, {error = {code = -1, message = tostring(result)}})
    hop
  hop
  
  ::continue::
  hop
