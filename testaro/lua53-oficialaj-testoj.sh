#!/bin/sh
# Opcie rulu oficialajn Lua 5.3 testojn kontraŭ Lupa.
set -eu

if [ "${LUPA_RUN_LUA53_OFICIALA:-0}" != "1" ]; then
  echo "[SALTITA] lua53-oficialaj-testoj.sh: agordu LUPA_RUN_LUA53_OFICIALA=1 por ruli"
  exit 0
fi

UZI_MISE_LUA=0
if command -v lua >/dev/null 2>&1; then
  :
elif command -v mise >/dev/null 2>&1 && mise exec lua@5.3 -- lua -v >/dev/null 2>&1; then
  UZI_MISE_LUA=1
  echo "[INFO] Uzas 'mise exec lua@5.3 -- lua' kiel Lua 5.3 provizanton"
else
  echo "[SALTITA] lua53-oficialaj-testoj.sh: 'lua' ne disponeblas en PATH kaj 'mise lua@5.3' ne pretas"
  exit 0
fi

ruli_lua() {
  (
    unset LUA_INIT LUA_INIT_5_3 LUA_PATH LUA_CPATH
    if [ "$UZI_MISE_LUA" -eq 1 ]; then
      mise exec lua@5.3 -- lua "$@"
    else
      lua "$@"
    fi
  )
}

ruli_lupe() {
  (
    unset LUA_INIT LUA_INIT_5_3 LUA_PATH LUA_CPATH
    "$LUPE_BIN" "$@"
  )
}

LUA_VER="$(ruli_lua -v 2>&1 || true)"
case "$LUA_VER" in
  *"Lua 5.3"*) ;;
  *)
    echo "[SALTITA] lua53-oficialaj-testoj.sh: bezonata Lua 5.3 (ricevita: $LUA_VER)"
    exit 0
    ;;
esac

if ! command -v curl >/dev/null 2>&1; then
  echo "[SALTITA] lua53-oficialaj-testoj.sh: 'curl' ne disponeblas"
  exit 0
fi
if ! command -v tar >/dev/null 2>&1; then
  echo "[SALTITA] lua53-oficialaj-testoj.sh: 'tar' ne disponeblas"
  exit 0
fi

SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
ROOT_DIR="$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)"
LUPE_BIN="$ROOT_DIR/fontaro/lupe"

if [ ! -x "$LUPE_BIN" ]; then
  echo "[MALSUKCESIS] lua53-oficialaj-testoj.sh: ne trovita ruligebla $LUPE_BIN"
  exit 1
fi

WORK_DIR="${TMPDIR:-/tmp}/lupa-lua53-oficialaj-testoj"
LUA_VERSIO="5.3.6"
ARCHIVE="$WORK_DIR/lua-$LUA_VERSIO.tar.gz"
TEST_DIR="$WORK_DIR/lua-$LUA_VERSIO/testes"
mkdir -p "$WORK_DIR"

if [ ! -d "$TEST_DIR" ]; then
  echo "[INFO] Elŝutante Lua $LUA_VERSIO testaron el lua/lua GitHub-arkivo..."
  rm -rf "$WORK_DIR/lua-$LUA_VERSIO"
  curl -fsSL -o "$ARCHIVE" "https://github.com/lua/lua/archive/refs/tags/v$LUA_VERSIO.tar.gz"
  tar -xzf "$ARCHIVE" -C "$WORK_DIR"
fi

if [ ! -f "$TEST_DIR/all.lua" ]; then
  echo "[MALSUKCESIS] lua53-oficialaj-testoj.sh: ne trovita $TEST_DIR/all.lua"
  exit 1
fi

LUA_OUT="$WORK_DIR/lua53.out"
LUPA_OUT="$WORK_DIR/lupa53.out"

echo "[INFO] Rulante oficialajn testojn per Lua 5.3..."
if (cd "$TEST_DIR" && ruli_lua all.lua >"$LUA_OUT" 2>&1); then
  LUA_OK=1
else
  LUA_OK=0
fi

if [ "$LUA_OK" -ne 1 ]; then
  echo "[MALSUKCESIS] lua53-oficialaj-testoj.sh: Lua 5.3 oficialaj testoj malsukcesis"
  sed -n '1,120p' "$LUA_OUT"
  exit 1
fi

echo "[INFO] Rulante oficialajn testojn per Lupa..."
if (cd "$TEST_DIR" && ruli_lupe all.lua >"$LUPA_OUT" 2>&1); then
  LUPA_OK=1
else
  LUPA_OK=0
fi

if [ "$LUPA_OK" -ne 1 ]; then
  echo "[MALSUKCESIS] lua53-oficialaj-testoj.sh: Lupa malsukcesis en oficialaj testoj"
  sed -n '1,120p' "$LUPA_OUT"
  exit 1
fi

if ! grep -q "OK" "$LUA_OUT"; then
  echo "[MALSUKCESIS] lua53-oficialaj-testoj.sh: Lua 5.3 ne raportis sukcesan finon"
  sed -n '1,120p' "$LUA_OUT"
  exit 1
fi

if ! grep -q "OK" "$LUPA_OUT"; then
  echo "[MALSUKCESIS] lua53-oficialaj-testoj.sh: Lupa ne raportis sukcesan finon"
  sed -n '1,120p' "$LUPA_OUT"
  exit 1
fi

echo "[PASIS] lua53-oficialaj-testoj.sh"
