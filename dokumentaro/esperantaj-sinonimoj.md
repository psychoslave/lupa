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
- `==` -> `baŭ`, `egalas`, `samas`
- `>=` -> `sur`, `suras`, `almenaŭas`, `almenauxas`, `malsubas`
- `<=` -> `sub`, `subas`, `maksimumas`, `malsuras`, `malalmenaŭas`, `malalmenauxas`
- `~=` -> `zaŭ`, `zaux`, `malsamas`, `neegalas`, `nesamas`
- `~` (unuargumenta bitinverso) -> `nee`
- `~` (duargumenta bit-XOR) -> `disaŭe`, `disauxe`
- `>` -> `sup`, `super`, `superas`, `malinfraas`
- `<` -> `en`, `suf`, `infraas`, `malsuperas`
- `&` -> `kaje`
- `|` -> `aŭe`, `auxe`
- `>>` -> `sobŝove`, `sobsxove`
- `<<` -> `sorŝove`, `sorsxove`
- `+` -> `plus`
- `*` -> `oble`, `multiplike`
- `-` (unuargumenta minuso) -> `mal`, `kontraŭ`, `kontraux`
- `-` (duargumenta subtraho) -> `minus`
- `/` -> `disige`, `divide`, `ozle`
- `%` -> `laŭ`, `laux`, `module`, `kongrue`
- `^` -> `alt`, `potencige`
- `:` -> `sin`
- `=` -> `iĝu`, `igxu`, `iĝe`, `igxe`
- `,` -> `tuj`, `plie`

La parsero jam distingas la kuntekstojn de `,` (ekz. disigo de esprimoj, nomlistoj, argumentoj); `tuj` kaj `plie` mapigas al la sama komo-signo.

### Notoj pri leksema simetrion

**Baŭ** por `==` (egaleco) kaj **zaŭ** por `~=` (neegaleco) kreigas etimologian simetrion:
- **`baŭ`**: Unua konsonanto de la esperanta alfabeto (b)
- **`zaŭ`**: Lasta konsonanto de la esperanta alfabeto (z)

Simile, **oble** por `*` (multiplikado) kaj **ozle** por `/` (dividado) kreigas lekseman simetrion:
- **`oble`**: Enhavas "b" (unua konsonanto) - konstruita analoge al "duplo" kaj "triplo"
- **`ozle`**: Enhavas "z" (lasta konsonanto) - konstruita kiel spegulo al "oble"

Ĉi tiu simetrio reflektas la operacian simetrion de multiplikado kaj dividado, kaj evitas kolizion kun PIV-aj vortoj.

### Notoj pri mallongaj rilatsignoj

Ĉiuj rilataj operatoroj havas almenaŭ unu mallongan (maksimume 3-signa) sinonimo:

- **`sup`** por `>` (mallongigo de *supra* = supra/super, kiu parigas bone kun *sub*). 
  Atestita en [tekstaro.com](https://tekstaro.com/t?nomo=ondo-de-esperanto&uzistreketojn=0&tipo=&sekcio=LOdE-1999-10&antauasekcio=1&postasekcio=1):
  > "SVI aperas averaĝe ĉiun trian semajnon, sup aperis kvinfoje en la jaro."
  
  Verŝajne apokopio de "supra", kaj ĝi eĉ bone respondas al "sub" laŭ terminaloga formo.

- **`sur`** por `>=` (de *supra* + direkto)
- **`sub`** por `<=` (de *sub* = sub/malsupra, kiu parigas bone kun *sup*)
- **`suf`** por `<` (mallonga membro de la sama `su-` familio; postfakta etimologia subteno eblas per formoj kiel *sufiĉa* < lat. *subfacere*, *sufikso* < lat. *subfigere*, k.a.)
- **`en`** por `<` (semantike: "3 estas *en* 5" = 3 estas *interne de* 5, ĉar 3 < 5)

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
