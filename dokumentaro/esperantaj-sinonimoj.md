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
- `break` -> `eksterŝalte`, `ekstersxalte`, `rompe`
- `do` -> `fare`
- `else` -> `alie`

### Noto pri `~=` (neegala) kaj `zaŭ` neologismo

Antaŭe, `alie` servis kaŭ sinonimo de ambeŭ `else` (ŝlosilvortoj) kaj `~=` (operatoro). Tio kreis semantikan konflikon. Pro tio, ni revenis `alie` al eksklusiva rolo kiel `else`.

Por la operatoro `~=` (neegala), ni adoptis la neologismon **`zaŭ`** (kaj ĝiajn `-x` formon **`zaux`**) kiel mallongan alternativon al `malsamas`.

*Zaŭ* estas libere inspirata de *kvazaŭ* (= Latina *quasi*) por providi mallongan terminon. Kvankam etimologie neĉiela, ĝi ŝajnas ege taŭga opcio, konsiderante la celon de klareco kaj breveco.

- `elseif` -> `alise`
- `end` -> `hop`
- `false` -> `falsa`
- `for` -> `por`
- `function` -> `funkcie`, `tie`, `verbe`, `age`
- `goto` -> `ŝalte`, `sxalte`
- `if` -> `se`
- `in` -> `el`
- `local` -> `loka`, `loke`, `ĉi`, `cxi`
- `nil` -> `nenio`, `neo`
- `not` -> `ne`
- `or` -> `aŭ`, `aux`
- `repeat` -> `cikle`
- `return` -> `reŝalte`, `resxalte`, `ren`
- `self` (nomo de metoda ricevilo) -> `sia`
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
- `>>` -> `sob`, `sobŝove`, `sobsxove`
- `<<` -> `sor`, `sorŝove`, `sorsxove`
- `+` -> `plus`
- `*` -> `oble`, `multiplike`
- `#` -> `pese`, `kiom`, `kvante`, `amplekse`
- `-` (unuargumenta minuso) -> `mal`, `kontraŭ`, `kontraux`
- `-` (duargumenta subtraho) -> `minus`
- `/` -> `disige`, `divide`, `ozle`
- `%` -> `laŭ`, `laux`, `module`, `kongrue`
- `^` -> `alt`, `potencige`
- `.` -> `propra`, `ie`, `ties`
- `:` -> `sin`, `ke`
- `::` -> `ho`
- `=` -> `iĝu`, `igxu`, `iĝe`, `igxe`
- `;` -> `nu`
- `,` -> `tuj`, `plie`

La sintaksanalizilo jam distingas la kuntekstojn de `,` (ekz. disigo de esprimoj, nomlistoj, argumentoj); `tuj` kaj `plie` mapigas al la sama komo-signo.

Por `::`, la sinonimo `ho` baziĝas sur la vokativa interjekcio en Esperanto: ĝi semantike markas alvokon/alparolon al etikedo-celo (`ho etikedo ho`).

### Klarigoj pri la realigitaj formoj

- `break` (`eksterŝalte`, `ekstersxalte`, `rompe`) estas la elireja ordono por tuj forlasi ciklon aŭ blokon; la ŝalt-derivaĵoj emfazas subitan ŝanĝon de stato.
- `function` (`funkcie`, `tie`, `verbe`, `age`) markas funkciodifinon; la proponoj celas nomi agon aŭ funkcion sen perdi la funkcio-noman rolon.
- `local` (`loka`, `loke`, `ĉi`, `cxi`) markas lokan amplekson; la tradukoj emfazas ke la sekva nomo restas limigita al la nuna kodo.
- `return` (`reŝalte`, `resxalte`, `ren`) estas la kunulo de `break`: ĝi “ŝaltas reen” kun valoro aŭ sen ĝi. `ren` sekvas la jam proponitan wiki-formon, estas same mallonga kiel la antaŭa varianto, kaj restas natura Esperanto-vorto sen nova neologismo. Kompreneble la `r-` komenco ankaŭ donas mneman proksimon kun *r*eŝalte kaj *r*eturn(ig)/i.
- `==` (`baŭ`, `egalas`, `samas`) estas egaleco; la mallonga formo `baŭ` tenas la saman b/ z-simetrian ideon kiel `zaŭ`.
- `~=` (`zaŭ`, `zaux`, `malsamas`, `neegalas`, `nesamas`) estas neegaleco; `zaŭ` estas la spegulo de `baŭ`.
- `>`/`<`/`>=`/`<=` uzas la mallongajn familiojn `sup`/`sub`/`sur`/`suf` por konservi la ord-rilatan semantikon sen tro longa vortumo.
- `*` (`oble`, `multiplike`) kaj `/` (`disige`, `divide`, `ozle`) sekvas la saman lekseman simetrion kiel `baŭ`/`zaŭ`.
- `//` (`onige`, `parte`, `pece`, `kvociente`) estas la kvocienta formo de divido.
- `%` (`laŭ`, `laux`, `module`, `kongrue`) estas la resta/modula formo; `laŭ` substrekas “proporcie al”.
- `#` (`pese`, `kiom`, `kvante`, `amplekse`) mezuras la amplekson aŭ longon de la valoro.
- `^` (`alt`, `potencige`) estas la potenciga operatoro.
- `.` (`propra`, `ie`, `ties`) estas la membra aliro; `ties` donas la mallongan tabelan aliron.
- `:` (`sin`, `ke`) estas la refleksiva metodvoko; la ricevilo restas la sama objekto.
- `::` (`ho`) markas etikedon por `ŝalte`/`sxalte`.
- `...` (`ktp`) estas la variadika/“variloka” esprimo.
- `,` (`tuj`, `plie`) apartigas esprimojn, nomlistojn kaj argumentojn; la sintaksanalizilo elektas la ĝustan rolon laŭ kunteksto.
- `;` (`nu`) estas la propozicia disigilo.

`>>`/`<<` uzas `sob`/`sor` kiel la plej mallongajn ŝovajn formojn; la pli longaj `sobŝove`/`sobsxove` kaj `sorŝove`/`sorsxove` restas pliaj stilaj variantoj.


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
