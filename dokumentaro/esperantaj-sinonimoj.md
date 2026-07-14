# Esperantaj sinonimoj en Lupa

Tiu ĉi dokumento estas la fonto de vero pri la Esperantaj (kaj `-x`) formoj en Lupa.

- Leksilo: `fontaro/llex.c`
- Bazbiblioteko: `fontaro/lbaselib.c`
- Pakaĵa/ŝarga biblioteko: `fontaro/loadlib.c`

## Noto pri `-x` fallback

Kiam klavaro ne facile tajpas diakritojn, Lupa subtenas `-x` formojn (ekz. `ŝargu`/`sxargu`, `ŝirmvoku`/`sxirmvoku`).

## Leksilaj sinonimoj (`llex`)

### Ŝlosilvortoj

- `and` -> `kaj`
- `break` -> `ekstersxalte`, `eksterŝalte`
- `do` -> `fare`
- `else` -> `alie`
- `elseif` -> `alise`
- `end` -> `hop`
- `false` -> `falsa`
- `for` -> `por`
- `function` -> `funkcie`, `tie`
- `goto` -> `sxalte`, `ŝalte`
- `if` -> `se`
- `in` -> `el`
- `local` -> `loka`, `loke`, `ĉi`
- `nil` -> `nilo`
- `not` -> `ne`
- `or` -> `aux`, `aŭ`
- `repeat` -> `cikle`
- `return` -> `reŝalte`, `resxalte`
- `then` -> `tiam`
- `true` -> `vera`
- `until` -> `gxis`
- `while` -> `dum`

### Operatoroj kaj signoj

- `//` -> `onige`, `parte`, `pece`, `kvociente`
- `..` -> `lige`, `krocxe`, `kroĉe`
- `...` -> `ktp`
- `==` -> `egalas`, `samas`
- `>=` -> `almenauxas`, `almenaŭas`, `suras`, `malsubas`
- `<=` -> `maksimumas`, `subas`, `malsuras`, `malalmenauxas`, `malalmenaŭas`
- `~=` -> `malegalas`, `malsamas`, `neegalas`, `nesamas`
- `~` (unuargumenta bitinverso) -> `nee`
- `~` (duargumenta bit-XOR) -> `disauxe`, `disaŭe`
- `>` -> `superas`, `malinfraas`
- `<` -> `infraas`, `malsuperas`
- `&` -> `kaje`
- `|` -> `auxe`, `aŭe`
- `>>` -> `sobsxove`, `sobŝove`
- `<<` -> `sorsxove`, `sorŝove`
- `+` -> `plus`
- `-` (unuargumenta minuso) -> `mal`, `kontraŭ`
- `-` (duargumenta subtraho) -> `minus`
- `/` -> `disige`, `divide`, `ozle`
- `%` -> `module`, `kongrue`
- `^` -> `alt`, `potencige`
- `:` -> `sin`, `:`

## Bazbibliotekaj sinonimoj (`lbaselib`)

- `assert` -> `asertu`
- `collectgarbage` -> `senrubigu`
- `dofile` -> `dosierrulu`
- `error` -> `eraravizu`
- `getmetatable` -> `metatabelodonu`
- `ipairs` -> `induktparodonu`
- `loadfile` -> `dosiersxargu`, `dosierŝargu`
- `load` -> `sxargu`, `ŝargu`
- `loadstring` -> `cxensxargu`, `ĉenŝargu`
- `next` -> `sekvdonu`
- `pairs` -> `parodonu`
- `pcall` -> `sxirmvoku`, `ŝirmvoku`
- `print` -> `printe`, `printu`
- `rawequal` -> `rektegalu`
- `rawlen` -> `rektpesu`
- `rawget` -> `rektdonu`
- `rawset` -> `rektloku`
- `select` -> `selektu`
- `setmetatable` -> `metatabelloku`
- `tonumber` -> `nombrigu`
- `tostring` -> `cxenigu`, `ĉenigu`
- `type` -> `tipdonu`
- `xpcall` -> `traktsxirmvoku`, `traktŝirmvoku`

## Pakaĵaj kaj ŝargaj sinonimoj (`loadlib`)

- `loadlib` -> `teksxargu`, `tekŝargu`
- `searchpath` -> `dosierindikosercxu`, `dosierindikoserĉu`
- `seeall` -> `plenvidu` (nur en `LUA_COMPAT_MODULE`)
- `require` -> `necesu`

## Testa kovrado

La rilataj testoj troviĝas en:

- `testaro/sinonimoj-leksilo.lupa`
- `testaro/sinonimoj-bibliotekoj.lupa`
- `testaro/lua53-kongruo.lua`

La tria testdosiero provas kondutan kongruon inter Lua 5.3 kaj Lupa por kanona Lua-kodo (sen Lupa-specifaj sinonimoj).

