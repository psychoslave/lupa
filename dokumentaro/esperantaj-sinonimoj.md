# Esperantaj sinonimoj en Lupa

Tiu ĉi dokumento estas la fonto de vero pri la Esperantaj (kaj `-x`) formoj en Lupa.

- Leksilo: `fontaro/llex.c`
- Bazbiblioteko: `fontaro/lbaselib.c`
- Pakaĵa/ŝarga biblioteko: `fontaro/loadlib.c`

## Noto pri `-x` retrodefaŭlta

Kiam klavaro ne facile tajpas diakritojn, Lupa subtenas `-x` formojn (ekz. `ŝargu`/`sxargu`, `ŝirmvoku`/`sxirmvoku`).

## Leksilaj sinonimoj (`llex`)

### Ŝlosilvortoj

- `and` -> `kaj`
- `break` -> `eksterŝalte`, `ekstersxalte`
- `do` -> `fare`
- `else` -> `alie`

### Noto pri `~=` (neegala) kaj `zaŭ` neologismo

Antaŭe, `alie` servis kaŭ sinonimo de ambeŭ `else` (ŝlosilvortoj) kaj `~=` (operatoro). Tio kreis semantikan konflikon. Pro tio, ni revenis `alie` al eksklusiva rolo kiel `else`.

Por la operatoro `~=` (neegala), ni adoptis la neologismon **`zaŭ`** (kaj ĝiajn `-x` formon **`zaux`**) kiel mallongan alternativon al `malsamas`.

*Zaŭ* estas libere inspirata de *kvazaŭ* (= Latina *quasi*) por providi mallongan terminon. Kvankam etimologie neĉiela, ĝi ŝajnas ege taŭga opsiono, konsiderante la celon de klareco kaj breveco.

- `elseif` -> `alise`
- `end` -> `hop`
- `false` -> `falsa`
- `for` -> `por`
- `function` -> `funkcie`, `tie`
- `goto` -> `ŝalte`, `sxalte`
- `if` -> `se`
- `in` -> `el`
- `local` -> `loka`, `loke`, `ĉi`, `cxi`
- `nil` -> `nilo`
- `not` -> `ne`
- `or` -> `aŭ`, `aux`
- `repeat` -> `cikle`
- `return` -> `reŝalte`, `resxalte`
- `then` -> `tiam`
- `true` -> `vera`
- `until` -> `ĝis`, `gxis`
- `while` -> `dum`

### Operatoroj kaj signoj

- `//` -> `onige`, `parte`, `pece`, `kvociente`
- `..` -> `lige`, `kroĉe`, `krocxe`
- `...` -> `ktp`
- `==` -> `egalas`, `samas`
- `>=` -> `almenaŭas`, `almenauxas`, `suras`, `malsubas`
- `<=` -> `maksimumas`, `subas`, `malsuras`, `malalmenaŭas`, `malalmenauxas`
- `~=` -> `zaŭ`, `zaux`, `malsamas`, `neegalas`, `nesamas`
- `~` (unuargumenta bitinverso) -> `nee`
- `~` (duargumenta bit-XOR) -> `disaŭe`, `disauxe`
- `>` -> `superas`, `malinfraas`
- `<` -> `infraas`, `malsuperas`
- `&` -> `kaje`
- `|` -> `aŭe`, `auxe`
- `>>` -> `sobŝove`, `sobsxove`
- `<<` -> `sorŝove`, `sorsxove`
- `+` -> `plus`
- `-` (unuargumenta minuso) -> `mal`, `kontraŭ`, `kontraux`
- `-` (duargumenta subtraho) -> `minus`
- `/` -> `disige`, `divide`, `ozle`
- `%` -> `laŭ`, `laux`, `module`, `kongrue`
- `^` -> `alt`, `potencige`
- `:` -> `sin`
- `=` -> `iĝu`, `igxu`, `iĝe`, `igxe`

## Bazbibliotekaj sinonimoj (`lbaselib`)

- `assert` -> `asertu`
- `collectgarbage` -> `senrubigu`
- `dofile` -> `dosierrulu`
- `error` -> `eraravizu`
- `getmetatable` -> `metatabelodonu`
- `ipairs` -> `induktparodonu`
- `loadfile` -> `dosierŝargu`, `dosiersxargu`
- `load` -> `ŝargu`, `sxargu`
- `loadstring` -> `ĉenŝargu`, cxensxargu`
- `next` -> `sekvdonu`
- `pairs` -> `parodonu`
- `pcall` -> `ŝirmvoku`, `sxirmvoku`
- `print` -> `printe`, `printu`
- `rawequal` -> `rektegalu`
- `rawlen` -> `rektpesu`
- `rawget` -> `rektdonu`
- `rawset` -> `rektloku`
- `select` -> `selektu`
- `setmetatable` -> `metatabelloku`
- `tonumber` -> `nombrigu`
- `tostring` -> `ĉenigu`, `cxenigu`
- `type` -> `tipdonu`
- `xpcall` -> `traktŝirmvoku`, `traktsxirmvoku`

## Pakaĵaj kaj ŝargaj sinonimoj (`loadlib`)

- `loadlib` -> `tekŝargu`, `teksxargu`
- `searchpath` -> `dosierindikosercxu`, `dosierindikoserĉu`
- `seeall` -> `plenvidu` (nur en `LUA_COMPAT_MODULE`)
- `require` -> `necesu`

## Testa kovrado

La rilataj testoj troviĝas en:

- `testaro/sinonimoj-leksilo.lupa`
- `testaro/sinonimoj-bibliotekoj.lupa`
- `testaro/lua53-kongruo.lua`

La tria testdosiero provas kondutan kongruon inter Lua 5.3 kaj Lupa por kanona Lua-kodo (sen Lupa-specifaj sinonimoj).

