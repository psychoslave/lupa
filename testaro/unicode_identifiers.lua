#!/usr/bin/env lua
-- Unit tests for Unicode identifier support
-- Run with: lupe testaro/unicode_identifiers.lua

local function test_unicode_variable_assignment()
  -- Test basic Unicode variable assignment
  local mesaĝo = 'Saluton, mondo!'
  assert(mesaĝo == 'Saluton, mondo!', 'Unicode variable assignment failed')
  print('[PASS] test_unicode_variable_assignment')
end

local function test_unicode_numbers()
  -- Test Unicode identifiers with numeric values
  local ĉifro = 42
  local pi = 3.14159
  assert(ĉifro == 42, 'Unicode number assignment failed')
  assert(pi > 3.1 and pi < 3.2, 'Unicode float assignment failed')
  print('[PASS] test_unicode_numbers')
end

local function test_unicode_in_tables()
  -- Test Unicode identifiers as table keys
  local vorto = {}
  vorto.saluton = 'Hello'
  vorto.ĝojo = 'Joy'
  assert(vorto.saluton == 'Hello', 'Table field access failed')
  assert(vorto.ĝojo == 'Joy', 'Unicode table field access failed')
  print('[PASS] test_unicode_in_tables')
end

local function test_unicode_in_functions()
  -- Test Unicode identifiers in function definitions and calls
  local function kvadrato(ĉifro)
    return ĉifro * ĉifro
  end
  
  local rezultato = kvadrato(5)
  assert(rezultato == 25, 'Function with Unicode parameter failed')
  print('[PASS] test_unicode_in_functions')
end

local function test_unicode_loop_variables()
  -- Test Unicode identifiers in loops
  local sumo = 0
  for ĉapelo = 1, 5 do
    sumo = sumo + ĉapelo
  end
  assert(sumo == 15, 'Loop with Unicode variable failed')
  print('[PASS] test_unicode_loop_variables')
end

local function test_mixed_ascii_unicode()
  -- Test mixing ASCII and Unicode identifiers
  local day1 = 1
  local ĵaŭdo = 4
  local total = day1 + ĵaŭdo
  assert(total == 5, 'Mixed ASCII/Unicode arithmetic failed')
  print('[PASS] test_mixed_ascii_unicode')
end

local function test_unicode_in_conditionals()
  -- Test Unicode identifiers in if statements
  local mesaĝo = 'test'
  if mesaĝo == 'test' then
    assert(true, 'Unicode conditional should pass')
  else
    assert(false, 'Unicode conditional failed')
  end
  print('[PASS] test_unicode_in_conditionals')
end

local function test_unicode_string_operations()
  -- Test Unicode identifiers with string operations
  local tekstoA = 'Saluton'
  local tekstoB = 'mondo'
  local rezulto = tekstoA .. ', ' .. tekstoB .. '!'
  assert(rezulto == 'Saluton, mondo!', 'Unicode string concatenation failed')
  print('[PASS] test_unicode_string_operations')
end

local function test_latin_extended_characters()
  -- Test various Latin Extended characters used in European languages
  local ĉ = 'c-circumflex'  -- Esperanto
  local ĝ = 'g-circumflex'  -- Esperanto
  local ĥ = 'h-circumflex'  -- Esperanto
  local ĵ = 'j-circumflex'  -- Esperanto
  local ŝ = 's-circumflex'  -- Esperanto
  local ŭ = 'u-breve'       -- Esperanto
  
  assert(ĉ == 'c-circumflex', 'ĉ character failed')
  assert(ĝ == 'g-circumflex', 'ĝ character failed')
  assert(ĥ == 'h-circumflex', 'ĥ character failed')
  assert(ĵ == 'j-circumflex', 'ĵ character failed')
  assert(ŝ == 's-circumflex', 'ŝ character failed')
  assert(ŭ == 'u-breve', 'ŭ character failed')
  print('[PASS] test_latin_extended_characters')
end

local function test_unicode_local_scope()
  -- Test Unicode identifiers respect scope
  local ĉapelo_eksterno = 'outer'
  do
    local ĉapelo_eksterno = 'inner'
    assert(ĉapelo_eksterno == 'inner', 'Inner scope failed')
  end
  assert(ĉapelo_eksterno == 'outer', 'Outer scope failed')
  print('[PASS] test_unicode_local_scope')
end

-- Run all tests
local tests = {
  test_unicode_variable_assignment,
  test_unicode_numbers,
  test_unicode_in_tables,
  test_unicode_in_functions,
  test_unicode_loop_variables,
  test_mixed_ascii_unicode,
  test_unicode_in_conditionals,
  test_unicode_string_operations,
  test_latin_extended_characters,
  test_unicode_local_scope,
}

print('\n=== Running Unicode Identifier Tests ===')
local passed = 0
local failed = 0

for _, test in ipairs(tests) do
  local status, error = pcall(test)
  if not status then
    print('[FAIL] ' .. test .. ': ' .. tostring(error))
    failed = failed + 1
  else
    passed = passed + 1
  end
end

print('\n=== Test Results ===')
print('Passed: ' .. passed)
print('Failed: ' .. failed)
print('Total:  ' .. (passed + failed))

if failed > 0 then
  os.exit(1)
end
