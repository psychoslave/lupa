#!/usr/bin/env lua
-- Kongrua testo inter Lua 5.3 kaj Lupa por kanona Lua-kodo.

local function run(cmd)
  local p = io.popen(cmd .. " 2>&1")
  if not p then return false, "" end
  local out = p:read("*a") or ""
  local ok = p:close()
  if ok == true then return true, out end
  if type(ok) == "number" then return ok == 0, out end
  return false, out
end

local function has_lua53()
  local ok, out = run("lua -v")
  if not ok then return false end
  return out:find("Lua 5%.3", 1, false) ~= nil
end

if not has_lua53() then
  print("[SALTITA] lua53-kongruo.lua: Lua 5.3 ne disponeblas en PATH")
  os.exit(0)
end

local src = [[
local t = {1, 2, 3, 4}
local s = 0
for _, v in ipairs(t) do
  s = s + v
end
local f = function(x) return (x * 3) // 2 end
print("sumo=" .. s)
print("f(9)=" .. f(9))
print("tipo=" .. type(t))
]]

local tmp = os.tmpname() .. ".lua"
local f = assert(io.open(tmp, "w"))
f:write(src)
f:close()

local ok_lua, out_lua = run("lua " .. tmp)
local ok_lupa, out_lupa = run("./fontaro/lupe " .. tmp)
os.remove(tmp)

if not ok_lua then
  io.stderr:write("[MALSUKCESIS] lua53-kongruo.lua: Lua 5.3 malsukcesis\n" .. out_lua)
  os.exit(1)
end

if not ok_lupa then
  io.stderr:write("[MALSUKCESIS] lua53-kongruo.lua: Lupa malsukcesis\n" .. out_lupa)
  os.exit(1)
end

if out_lua ~= out_lupa then
  io.stderr:write("[MALSUKCESIS] lua53-kongruo.lua: Eligoj malkongruas\n")
  io.stderr:write("--- Lua 5.3 ---\n" .. out_lua)
  io.stderr:write("--- Lupa ---\n" .. out_lupa)
  os.exit(1)
end

print("[PASIS] lua53-kongruo.lua")

