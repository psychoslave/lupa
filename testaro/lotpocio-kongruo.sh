#!/bin/sh
set -eu

if ! command -v lua >/dev/null 2>&1; then
  echo "[MISE] lotpocio-kongruo.sh: Lua ne disponeblas en PATH"
  exit 0
fi

if [ ! -x "./fontaro/lupe" ]; then
  echo "[MISE] lotpocio-kongruo.sh: mankas ./fontaro/lupe"
  exit 1
fi

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

matrix_lua="$tmpdir/matrix_lua.lua"
matrix_cit="$tmpdir/matrix_cit.lua"
lua_out="$tmpdir/lua.out"
lupe_out="$tmpdir/lupe.out"
cit_out="$tmpdir/cit.out"
cit_expected="$tmpdir/cit.expected"

cat >"$matrix_lua" <<'EOF'
local cases = {
  {"raw-empty-quote", "\"\""},
  {"raw-unfinished-quote", "\""},
  {"raw-open-long", "[["},
  {"raw-empty-long", "[[\n]]"},
  {"raw-text-long", "[[\ntest\n]]"},
  {"raw-open-eq-long", "[=["},
  {"raw-empty-eq-long", "[=[\n]=]"},
  {"return-empty-quote", "return \"\""},
  {"return-text-long", "return [[test]]"},
  {"return-text-eq-long", "return [=[test]=]"},
}
for _, case in ipairs(cases) do
  local name, src = case[1], case[2]
  local fn = load(src, "=(lotpocio)")
  if not fn then
    print(name .. "|ERR")
  else
    local ok, value = pcall(fn)
    if ok then
      print(name .. "|OK|" .. tostring(value))
    else
      print(name .. "|RUNERR")
    end
  end
end
EOF

cat >"$matrix_cit" <<'EOF'
local cases = {
  {"cit-open-only", "cit"},
  {"return-cit-space", "return cit saluton malcit"},
  {"return-cit-punct", "return cit¡saluton!ĉit"},
  {"return-cit-double-punct", "return cit¡¡saluton!!ĉit"},
}
for _, case in ipairs(cases) do
  local name, src = case[1], case[2]
  local fn = load(src, "=(lotpocio)")
  if not fn then
    print(name .. "|ERR")
  else
    local ok, value = pcall(fn)
    if ok then
      print(name .. "|OK|" .. tostring(value))
    else
      print(name .. "|RUNERR")
    end
  end
end
EOF

lua "$matrix_lua" >"$lua_out"
./fontaro/lupe "$matrix_lua" >"$lupe_out"

if ! diff -u "$lua_out" "$lupe_out" >/dev/null; then
  echo "[MISE] lotpocio-kongruo.sh: Lua/Lupa malsamas por \"\", [[ ]], [=[ ]=] en LoTPoCIo-similaj enigoj"
  diff -u "$lua_out" "$lupe_out" || true
  exit 1
fi

./fontaro/lupe "$matrix_cit" >"$cit_out"
cat >"$cit_expected" <<'EOF'
cit-open-only|ERR
return-cit-space|OK|saluton
return-cit-punct|OK|saluton
return-cit-double-punct|OK|¡saluton!
EOF

if ! diff -u "$cit_expected" "$cit_out" >/dev/null; then
  echo "[MISE] lotpocio-kongruo.sh: cit-konduto ne kongruas kun atendataj LoTPoCIo-similaj rezultoj"
  diff -u "$cit_expected" "$cit_out" || true
  exit 1
fi

echo "[BONE] lotpocio-kongruo.sh"
